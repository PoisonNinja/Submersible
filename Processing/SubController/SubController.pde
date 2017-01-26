import org.gamecontrolplus.gui.*;
import org.gamecontrolplus.*;
import net.java.games.input.*;
import processing.serial.*;

ControlIO control;
ControlDevice stick;

Serial port;

int LeftMotorValue;
int RightMotorValue;

float bank;
float pitch;
float azimuth;

ControlSlider LeftMotorAxis;
ControlSlider RightMotorAxis;
ControlSlider z_axis;

int motor0;
int motor1;
int motor2;
int motor3;

public void setup() {
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
  // Initialise the ControlIO
  control = ControlIO.getInstance(this);
  // Find a device that matches the submarine controller file
  stick = control.getMatchedDevice("sub_controller_v2");
  if (stick == null) {
    println("No suitable device configured");
    System.exit(1);
  }
  LeftMotorAxis = stick.getSlider("LeftMotorAxis");
  RightMotorAxis = stick.getSlider("RightMotorAxis");
}

public void draw() {
  /*
    * Read controller stick values. The values we get back range from -1 to 1.
   * The reason why y and z axis are inverted are because moving the stick up
   * for y gives a negative value, which is counter intuitive. Same for the z
   * axis. We then multiply this by 255 to convert that into a value suitable
   * for analogWrite, since the Arduino doesn't do any processing for speed
   * purposes.
   */
  LeftMotorValue = int(LeftMotorAxis.getValue() * 255);
  RightMotorValue = int(-(RightMotorAxis.getValue() * 255));
  /*
   * Add a sort of deadzone. When the stick is in neutral, the values we read
   * aren't actually 0. Usually it ranges from 12 - 18 for y. Thus, we have
   * y values less than 20 and greater than -20, we just set it to 0. Otherwise,
   * the motors would constantly be running at a low speed
   */
  if (LeftMotorValue <= 20 && LeftMotorValue >= -20) LeftMotorValue = 0;
  if (RightMotorValue <= 20 && RightMotorValue >= -20) RightMotorValue = 0;
  /*
   * Convert the x and y data into two motor control. If something is weird with
   * steering, this is probably the code that broke it.
   */
  motor0 = LeftMotorValue;
  motor1 = RightMotorValue;
  /*
    * Send the stick data over to the Arduino in the format "x y z" + a newline
   * only at the end. No carriage returns;
   */
  println(motor0 + " " + motor1 + " " + 0 + " " + 0 + '\n');
  port.write(motor0 + " " + motor1 + " " + 0 + " " + 0 + '\n');
}

void serialEvent(Serial port)
{
  /*
    * This code is pretty much identical to the ArtificialHorizon code, which
   * is where the original code was taken from. A couple of modifications
   * were made to optimize the code
   */
  String input = port.readStringUntil('n');
  if (input != null) {
    input = trim(input);
    String[] values = split(input, " ");
    if (values.length == 3) {
      float theta = float(values[0]);
      float phi = float(values[1]);
      float psi = float(values[2]);
      bank = (-phi/56);
      pitch = (theta * 52);
      azimuth = (psi);
      println("Bank: " + bank);
      println("Pitch: " + pitch);
      println("Azimuth: " + azimuth);
    }
  }
}