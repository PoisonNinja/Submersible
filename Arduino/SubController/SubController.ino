#include <SoftwareSerial.h>
#include "libsub.h"

#define BRAKE 0
#define CW    1
#define CCW   2
#define CS_THRESHOLD 15   // Definition of safety current (Check: "1.3 Monster Shield Example").

// MOTOR 1
#define MOTOR_A1_PIN 7
#define MOTOR_B1_PIN 8

// MOTOR 2
#define MOTOR_A2_PIN 4
#define MOTOR_B2_PIN 9

// MOTOR 3
#define MOTOR_A3_PIN 11
#define MOTOR_B3_PIN 12

#define PWM_MOTOR_1 5
#define PWM_MOTOR_2 6
#define PWM_MOTOR_3 10

#define MOTOR_0 0
#define MOTOR_1 1
#define MOTOR_2 2

void setup() {
    pinMode(MOTOR_A1_PIN, OUTPUT);
    pinMode(MOTOR_B1_PIN, OUTPUT);

    pinMode(MOTOR_A2_PIN, OUTPUT);
    pinMode(MOTOR_B2_PIN, OUTPUT);

    pinMode(MOTOR_A3_PIN, OUTPUT);
    pinMode(MOTOR_B3_PIN, OUTPUT);

    pinMode(PWM_MOTOR_1, OUTPUT);
    pinMode(PWM_MOTOR_2, OUTPUT);
    pinMode(PWM_MOTOR_3, OUTPUT);

    Serial.begin(115200);
    Serial.println("Ready!");
}

void loop() {
    String input = Serial.readStringUntil('\n');
    int motor_0 = SubUtils::getValue(input, ' ', 0).toInt();
    int motor_1 = SubUtils::getValue(input, ' ', 1).toInt();
    int motor_2 = SubUtils::getValue(input, ' ', 2).toInt();
    int motor_3 = SubUtils::getValue(input, ' ', 3).toInt();
    setMotor(MOTOR_0, (motor_0 < 255) ? CW : CCW, (motor_0 < 255) ? -1 * motor_0 : motor_0);
    setMotor(MOTOR_1, (motor_1 < 255) ? CW : CCW, (motor_1 < 255) ? -1 * motor_1 : motor_1);
    setMotor(MOTOR_2, (motor_2 < 255) ? CW : CCW, (motor_2 < 255) ? -1 * motor_2 : motor_2);
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
}


