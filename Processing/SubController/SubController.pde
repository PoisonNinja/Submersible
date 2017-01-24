import org.gamecontrolplus.gui.*;
import org.gamecontrolplus.*;
import net.java.games.input.*;

ControlIO control;
ControlDevice stick;

public void setup() {
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
  println("X: " + stick.getSlider("X-Axis").getValue());
  println("Y: " + -stick.getSlider("Y-Axis").getValue());
  println("Z: " + -stick.getSlider("Z-Axis").getValue());
}