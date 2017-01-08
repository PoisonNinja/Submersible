# Submersible
This repository contains the code for Team 47's (The Legends) submersible for the project Beneath the Surface.

# Prerequisites
You need the following programs and libraries:
* Arduino IDE [arduino.cc](https://www.arduino.cc/en/Main/Software)
* Processing IDE [processing](https://processing.org/download/)
* ToxicLib [toxiclibs.org](http://toxiclibs.org/downloads/)
* i2cdevlib [github.com](https://codeload.github.com/jrowberg/i2cdevlib/legacy.zip/master)

In addition, you need the following hardware:
* Arduino (tested on an Uno)
* Serial cable
* Jumper wires
* MPU-6050 Gyro/Accelerometer (Any manufacturer should work)

You can follow the steps [here](http://www.instructables.com/id/MPU6050-Arduino-6-Axis-Accelerometer-Gyro-GY-521-B/?ALLSTEPS) to set up the wiring and libraries.

# Project Organization
Due to both Arduino and Processing IDE limitations, each file has to be in it's own folder with the same name. Thus, I seperated Arduino code from Processing.

Each subproject has it's own folder under those. They correspond to each other. For example, Arduino/Sub3DView and Processing/Sub3DView correspond, and need to be run together.

# Building and Running
Open each file in the corresponding IDE.

Plug in Arduino. In the bottom right corner of the Arduino IDE, it should say Arduino/Geniuno (model) on /dev/(whatever). In the code, it is set to /dev/cu.usbmodemFD121. Most likely, it will not be the case, especially if you are on Linux. In the code, find the serialPort string variable and change it to the correct serial port path.

Do the same thing in the Processing code.

The following steps must be followed in order. If you do not do so, you will get strange errors.

First, flash the Arduino code onto the Arduino. DO NOT OPEN THE SERIAL MONITOR!

Then, build and run the Processing program. Most of the code should have built-in delays, but if it does not, don't touch the chip or the computer.

Wait for the MPU6050 to stop calibrating. This should occur 20-30 seconds after startup. On programs with a built-in delay, the GUI will become usable after this time period.
