#include <SoftwareSerial.h>
#include "libsub.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

MPU6050 mpu;
bool dmpReady = false;  // set true if DMP init was successful
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];         // [psi, theta, phi]    Euler angle container
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

int enablePins[4] = { 9, 11, 3, 0 };
int pin1[4] = { 2, 12, 7, 0 };
int pin2[4] = { 4, 8, 6, 0 };

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}

void setup() {
    pinMode(3, OUTPUT);
    for (int i = 0; i < 4; i++) {
        pinMode(pin1[i], OUTPUT);
        pinMode(pin2[i], OUTPUT);
        pinMode(enablePins[i], OUTPUT);
    }
    Serial.begin(115200);
    // join I2C bus (I2Cdev library doesn't do this automatically)
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
    TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
#endif
    mpu.initialize();
    devStatus = mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        mpu.setDMPEnabled(true);

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        //Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }
}

void loop() {
    String input = Serial.readStringUntil('\n');
    int motor_0 = SubUtils::getValue(input, ' ', 0).toInt();
    int motor_1 = SubUtils::getValue(input, ' ', 1).toInt();
    int motor_2 = SubUtils::getValue(input, ' ', 2).toInt();
    int motor_3 = SubUtils::getValue(input, ' ', 3).toInt();
    setMotor(0, (motor_0 < 0) ? -1 * motor_0 : motor_0, (motor_0 < 0) ? 1 : 0);
    setMotor(1, (motor_1 < 0) ? -1 * motor_1 : motor_1, (motor_1 < 0) ? 1 : 0);
    setMotor(2, (motor_2 < 0) ? -motor_2 : motor_2, (motor_2 < 0) ? 1 : 0);
//    setMotor(3, (motor_3 < 0) ? -motor_3 : motor_3, (motor_3 < 0) ? 1 : 0);
    if (!dmpReady) {
        return;
    } else {
        while (fifoCount < packetSize) {
            fifoCount = mpu.getFIFOCount();
        }
        if (fifoCount == 1024) {
            mpu.resetFIFO();
        } else {
            fifoCount = mpu.getFIFOCount();
            mpu.getFIFOBytes(fifoBuffer,packetSize);
            mpu.resetFIFO();

            fifoCount -= packetSize ;

            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
            Serial.print(ypr[0] * 180/M_PI);
            Serial.print(" ");
            Serial.print(ypr[1] * 180/M_PI);
            Serial.print(" ");
            Serial.println(ypr[2] * 180/M_PI);
        }
    }
}

void setMotor(int motorID, int speed, boolean reverse)
{
    analogWrite(enablePins[motorID], speed);
    digitalWrite(pin1[motorID], ! reverse);
    digitalWrite(pin2[motorID], reverse);
}
