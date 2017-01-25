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

float bank;
float pitch;
float azimuth;

ControlSlider x_axis;
ControlSlider y_axis;
ControlSlider z_axis;

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
    stick = control.getMatchedDevice("sub_controller");
    if (stick == null) {
        println("No suitable device configured");
        System.exit(1);
    }
    x_axis = stick.getSlider("X-Axis");
    y_axis = stick.getSlider("Y-Axis");
    z_axis = stick.getSlider("Z-Axis");
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
    x = int(x_axis.getValue() * 255);
    y = int(-(y_axis.getValue() * 255));
    z = int(-(z_axis.getValue() * 255));
    /*
    * Add a sort of deadzone. When the stick is in neutral, the values we read
    * aren't actually 0. Usually it ranges from 12 - 18 for y. Thus, we have
    * y values less than 20 and greater than -20, we just set it to 0. Otherwise,
    * the motors would constantly be running at a low speed
    */
    if (y <= 20 && y >= -20) y = 0;
    /*
    * Send the stick data over to the Arduino in the format "x y z" + a newline
    * only at the end. No carriage returns;
    */
    port.write(x + " " + y + " " + z + 'n');
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
