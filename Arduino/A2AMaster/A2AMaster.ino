#include <SoftwareSerial.h>
#include <Esplora.h>
#include "libsub.h"

/*
   Use digital pin 8 (port 6 on the expansion pins) for rx. Any
   pin from 8 - 12 can be used for RX, since they support
   interrupts. Use pin 1 (port 8 on the expansion pins) for TX,
   since it is the built in TX port.
*/
#define rxPin 8
#define txPin 1

float Bank = 0;
float Pitch = 0;
float Azimuth = 0;

#define joystickToLEDMultiplier 0.498

SoftwareSerial mySerial(rxPin, txPin);

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void setup() {
    Serial.begin(115200);
    while (!Serial) {}
    mySerial.begin(115200);
}

void loop() {
    /*
       If data is available in the serial FIFO buffer...
    */
    if (mySerial.available()) {
        String input = mySerial.readStringUntil('\n');
        if (input != NULL) {
            float theta = SubUtils::getValue(input, ' ', 0).toFloat();
            float phi = SubUtils::getValue(input, ' ', 1).toFloat();
            float psi = SubUtils::getValue(input, ' ', 2).toFloat();
            Bank = (-phi/56);
            Pitch = (theta * 52);
            Azimuth = (psi);
//            Serial.print("Bank: ");
//            Serial.print(Bank);
//            Serial.print(" Pitch: ");
//            Serial.print(Pitch);
//            Serial.print(" Azimuth: ");
//            Serial.println(Azimuth);
            Serial.println(input);
            float y = Esplora.readJoystickY();
            y *= joystickToLEDMultiplier;
            if (y > 255)
                y = 255;
            else if (y < 0)
                y = 0;
            mySerial.println(y);
        }
    }
}
