#include <SoftwareSerial.h>
#include "libsub.h"

/*
 * Macros for motor commands
 */
#define BRAKE 0
#define CW    1
#define CCW   2

/*
 * Motor 1
 */
#define MOTOR_A1_PIN 7
#define MOTOR_B1_PIN 8

/*
 * Motor 2
 */
#define MOTOR_A2_PIN 4
#define MOTOR_B2_PIN 9

/*
 * Motor 3
 */
#define MOTOR_A3_PIN 11
#define MOTOR_B3_PIN 12

/*
 * Motor 4
 * Uses an analog port because we are out of digital ports
 */
#define MOTOR_A4_PIN 2
#define MOTOR_B4_PIN A0

#define PWM_MOTOR_1 5
#define PWM_MOTOR_2 6
#define PWM_MOTOR_3 3
#define PWM_MOTOR_4 10

/*
 * Macros for identifying motors
 */
#define MOTOR_0 0
#define MOTOR_1 1
#define MOTOR_2 2
#define MOTOR_3 3

void setup() {
    /*
     * Initialize the pins for output.
     */
    pinMode(MOTOR_A1_PIN, OUTPUT);
    pinMode(MOTOR_B1_PIN, OUTPUT);

    pinMode(MOTOR_A2_PIN, OUTPUT);
    pinMode(MOTOR_B2_PIN, OUTPUT);

    pinMode(MOTOR_A3_PIN, OUTPUT);
    pinMode(MOTOR_B3_PIN, OUTPUT);

    pinMode(MOTOR_A4_PIN, OUTPUT);
    pinMode(MOTOR_B4_PIN, OUTPUT);

    pinMode(PWM_MOTOR_1, OUTPUT);
    pinMode(PWM_MOTOR_2, OUTPUT);
    pinMode(PWM_MOTOR_3, OUTPUT);
    pinMode(PWM_MOTOR_4, OUTPUT);

    Serial.begin(115200);
    /*
     * Send a message to kickstart communication. Otherwise, both
     * Processing and Arduino will wait forever for one of them
     * to send a message.
     */
    Serial.println("Ready!");
}

void loop() {
    /*
     * Wait for an entire line of input
     */
    String input = Serial.readStringUntil('\n');
    /*
     * Convert the space seperated line into individual variables.
     * The getValue function is part of libsub.
     */
    int motor_0 = SubUtils::getValue(input, ' ', 0).toInt();
    int motor_1 = SubUtils::getValue(input, ' ', 1).toInt();
    int motor_2 = SubUtils::getValue(input, ' ', 2).toInt();
    int motor_3 = SubUtils::getValue(input, ' ', 3).toInt();
    /*
     * Command the motors to the proper value. The second parameter
     * of setMotor takes the direction. Here, we parse the motor_*
     * values that can range from -255 to 255. If it is less than
     * 0, we assume that they want to go backwards. Thus, we
     * reverse the direction and multiply by -1 (since the motor
     * can't do negative current)
     */
    setMotor(MOTOR_0, (motor_0 < 0) ? CW : CCW, (motor_0 < 0) ? -1 * motor_0 : motor_0);
    setMotor(MOTOR_1, (motor_1 < 0) ? CW : CCW, (motor_1 < 0) ? -1 * motor_1 : motor_1);
    setMotor(MOTOR_2, (motor_2 < 0) ? CW : CCW, (motor_2 < 0) ? -1 * motor_2 : motor_2);
    setMotor(MOTOR_3, (motor_3 < 0) ? CW : CCW, (motor_3 < 0) ? -1 * motor_3 : motor_3);
}

void setMotor(uint8_t motor, uint8_t direct, uint8_t pwm)         //Function that controls the variables: motor(0 ou 1), direction (cw ou ccw) e pwm (entra 0 e 255);
{
    if (motor == MOTOR_0)
    {
        if (direct == CW)
        {
            digitalWrite(MOTOR_A1_PIN, LOW);
            digitalWrite(MOTOR_B1_PIN, HIGH);
        }
        else if (direct == CCW)
        {
            digitalWrite(MOTOR_A1_PIN, HIGH);
            digitalWrite(MOTOR_B1_PIN, LOW);
        }
        else
        {
            digitalWrite(MOTOR_A1_PIN, LOW);
            digitalWrite(MOTOR_B1_PIN, LOW);
        }

        analogWrite(PWM_MOTOR_1, pwm);
    }
    else if (motor == MOTOR_1)
    {
        if (direct == CW)
        {
            digitalWrite(MOTOR_A2_PIN, LOW);
            digitalWrite(MOTOR_B2_PIN, HIGH);
        }
        else if (direct == CCW)
        {
            digitalWrite(MOTOR_A2_PIN, HIGH);
            digitalWrite(MOTOR_B2_PIN, LOW);
        }
        else
        {
            digitalWrite(MOTOR_A2_PIN, LOW);
            digitalWrite(MOTOR_B2_PIN, LOW);
        }

        analogWrite(PWM_MOTOR_2, pwm);
    }
    else if (motor == MOTOR_2)
    {
        if (direct == CW)
        {
            digitalWrite(MOTOR_A3_PIN, LOW);
            digitalWrite(MOTOR_B3_PIN, HIGH);
        }
        else if (direct == CCW)
        {
            digitalWrite(MOTOR_A3_PIN, HIGH);
            digitalWrite(MOTOR_B3_PIN, LOW);
        }
        else
        {
            digitalWrite(MOTOR_A3_PIN, LOW);
            digitalWrite(MOTOR_B3_PIN, LOW);
        }
        analogWrite(PWM_MOTOR_3, pwm);
    }
    else if (motor == MOTOR_3)
    {
        if (direct == CW)
        {
            digitalWrite(MOTOR_A4_PIN, LOW);
            digitalWrite(MOTOR_B4_PIN, HIGH);
        }
        else if (direct == CCW)
        {
            digitalWrite(MOTOR_A4_PIN, HIGH);
            digitalWrite(MOTOR_B4_PIN, LOW);
        }
        else
        {
            digitalWrite(MOTOR_A4_PIN, LOW);
            digitalWrite(MOTOR_B4_PIN, LOW);
        }
        analogWrite(PWM_MOTOR_4, pwm);
    }
}


