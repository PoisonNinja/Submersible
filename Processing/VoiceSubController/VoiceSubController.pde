/*
  Simple WebSocketServer example that can receive voice transcripts from Chrome
 */

import muthesius.net.*;
import org.webbitserver.*;
import processing.serial.*;

WebSocketP5 socket;

Serial port;

int motor0;
int motor1;
int motor2;
int motor3;

void setup() {
  /*
    * On Mac OS X (Macbook Air 2011), the left USB port maps to cu.usbmodemFD121.
   * The right port maps to cu.usbmodemFA???
   * This value changes depending on the model of Mac and also what device
   * you have, as the Arduino Esplora identifies as a HID device
   */
  String serialPort = "/dev/cu.usbmodemFD121";
  /*
    * Wrap in a try catch block since this will most likely fail
   */
  try {
    port = new Serial(this, serialPort, 115200);
  }
  catch (Exception e) {
    /*
        * Oh no! The port doesn't exist
     * Actually, this will probably be the case for most computers, since
     * this seems to be a Mac specific port
     */
    println(e.getMessage());
    println("Attempting autodetection for serial ports (less accurate)");
    Boolean good = false;
    int i = 0;
    /*
        * A while loop that behaves like a for loop
     * TODO: Replace with a for loop
     */
    while (good == false) {
      /*
            * Once again wrap in a try catch block. If we get an array out of bounds
       * exception, that means that there are no more ports left.
       */
      try {
        serialPort = Serial.list()[i];
      }
      catch (Exception b) {
        /* No more ports, and yet we are still invalid. Kill ourselves */
        println("Failed to get port name. Aborting...");
        /*
                * Documentation recommends not doing this, but eh, who cares
         * It's not like it's going to crash, right? Right?
         */
        System.exit(1);
      }
      /*
            * Filter out Bluetooth devices that show up as serial ports
       * Needed for Macs, which have /dev/cu.Bluetooth-Incoming-Port
       * and /dev/tty.Bluetooth-Incoming-Port. Those ports don't
       * work for us.
       */
      if (serialPort.toLowerCase().contains("bluetooth")) {
        println("Rejecting " + serialPort + " because it is a Bluetooth port.");
      } else {
        /*
                * Yay! Found a valid port (hopefully)
         * TODO: Make sure this is actually an Arduino device, not
         * just any random serial device
         */
        good = true;
      }
      i++;
    }
    try {
      port = new Serial(this, serialPort, 115200);
    }
    catch (Exception a) {
      println("Failed to autodetect port. Aborting...");
      System.exit(1);
    }
  }
  println("Port set to " + serialPort);
  /*
    * Buffer input until we get a newline before firing serialEvent()
   */
  port.bufferUntil('n');
  socket = new WebSocketP5(this, 8080);
}

void draw() {
  println(motor0 + " " + motor1 + " " + motor2 + " " + motor3);
  port.write(motor0 + " " + motor1 + " " + motor2 + " " + motor3 + '\n');
}

void stop() {
  socket.stop();
}

void websocketOnMessage(WebSocketConnection con, String msg) {
  println(msg);
  if (msg.contains("forward")) motor0 = motor1 = 255;
  if (msg.contains("backwards") || msg.contain("backward")) motor0 = motor1 = -255;
}

void websocketOnOpen(WebSocketConnection con) {
  println("A client joined");
}

void websocketOnClosed(WebSocketConnection con) {
  println("A client left");
}