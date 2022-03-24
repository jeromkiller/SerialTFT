# SerialTFT
An arduino library for controlling TFT displays over serial

---

This library is currently still in a development state.
The aim of this project is to implement the basic drawing commands defined in the [Adafruit gfx library](https://github.com/adafruit/Adafruit-GFX-Library)

This library currenlty only supports drawing basic shapes,

### The code is currently thuroughly untested
Testing is being performed on a [Teensy 4.0](https://www.pjrc.com/store/teensy40.html) so performance may be different on an actual arduino.
But for readabillity (and because i'd like this library to be usable on an arduino) I'll be reffering to the Teensy as an arduino.
This will be more appearent when implementing the streaming of bitmap images.

  - Serializing and Deserializing has not been extensively tested yet.
  - shapes have only been drawn from teensy memory and not over serial yet.
 
### Further developments
List of developments I'm working on next
  - Create a desktop app to send serial commands to the arduino
  - Implement text writing with default font
  - expand control packets
    - No support for rotating the screen (I think)

List of developments I'm planning to implement later
  - Implementing commands to paint bitmaps from the sd card
    - For this i'll have to add support for the sd card
  - Implementing support for serialized batch commands to easilly draw set layouts to the screen
  - Implementing drawing bitmaps over serial
    - Regular arduinos do not have that much ram, so buffering the entire bitmap might not be feasable
    - Either a special drawing routine has to be created to allow drawing the bitmap as it gets streamed in.
    - Or a method could be added to save the incomming bitmap to the sdcard, and then loading it from there
  - idk, maybe some other stuff?
