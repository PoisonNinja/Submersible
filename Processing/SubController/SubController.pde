import org.gamecontrolplus.gui.*;
import org.gamecontrolplus.*;
import net.java.games.input.*;
import processing.serial.*;

ControlIO control;
ControlDevice stick;

Serial port;

int x;
int y;
int z;

public void setup() {
  String serialPort = "/dev/cu.usbmodemFD121"; // Serial.list()[0];
  try {
    port = new Serial(this, serialPort, 115200);
  } 
  catch (Exception e) {
    // Oh no! The port doesn't exist
    // Actually, this will probably be the case for most computers, since
    // this seems to be a Mac specific port
    println(e.getMessage());
    println("Attempting autodetection for serial ports (less accurate)");
    Boolean good = false;
    int i = 0;
    while (good == false) {
      try {
        serialPort = Serial.list()[i];
      } 
      catch (Exception b) {
        // No more ports, and yet we are still invalid. Kill ourselves
        println("Failed to get port name. Aborting...");
        // Documentation recommends not doing this, but eh, who cares
        // It's not like it's going to crash, right?
        System.exit(0);
      }
      // Filter out Bluetooth devices that show up as serial ports
      // Needed for Macs, which have /dev/cu.Bluetooth-Incoming-Port
      // and /dev/tty.Bluetooth-Incoming-Port
      if (serialPort.toLowerCase().contains("bluetooth")) {
        println("Rejecting " + serialPort + " because it is a Bluetooth port.");
      } else {
        good = true;
      }
      i++;
    }
    try {
      port = new Serial(this, serialPort, 115200);
    } 
    catch (Exception a) {
      println("Failed to autodetect port. Aborting...");
      System.exit(0);
    }
  }
  println("Port set to " + serialPort);
  port.bufferUntil('\n');
  // Initialise the ControlIO
  control = ControlIO.getInstance(this);
  // Find a device that matches the configuration file
  stick = control.getMatchedDevice("sub_controller");
  if (stick == null) {
    println("No suitable device configured");
    System.exit(-1); // End the program NOW!
  }
}

public void draw() {
  x = int(stick.getSlider("X-Axis").getValue() * 255);
  y = int(-stick.getSlider("Y-Axis").getValue() * 255);
  z = int(-stick.getSlider("Z-Axis").getValue() * 255);
  // Deadzone processing
  if (y <= 20) y = 0;
  port.write(x + " " + y + " " + z + '\n');
}

void serialEvent(Serial port) //Reading the datas by Processing.
{
}