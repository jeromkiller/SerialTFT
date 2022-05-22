# SerialTFT
An arduino library for controlling TFT displays over serial

---

This library is currently still in a development state.
The aim of this project is to implement the basic drawing commands defined in the [Adafruit gfx library](https://github.com/adafruit/Adafruit-GFX-Library)

This library currenlty only supports drawing basic shapes,

⚠ This library is currently in a development stage, breaking changes will be made in the future as I work with this library.

---

## What this library supports

 - Control up to 8 tft displays over the serial line
 - Drawing the basic shapes defined in the [Adafruit gfx library](https://github.com/adafruit/Adafruit-GFX-Library)
 - Drawing text in the standard font
 - Drawing bitmap images from an sd card (with added transparancy color)
 - Performing a batch of pre-serialized commands from an sd card

## Dependencies
⚠ this list of dependencies may not be completely correct and/or change in the future
 - Adafruit GFX Library
 - Adafruit Imagereader Library
 - SdFat library

### ⚠ The code is currently thuroughly untested on arduino ⚠
Testing is being performed on a [Teensy 4.0](https://www.pjrc.com/store/teensy40.html) so performance may be different on an actual arduino.
But for readabillity (and because i'd like this library to be usable on an arduino) I'll be reffering to the Teensy as an arduino.
 
### Further developments
List of developments I'd like to work on next:
 - Creating some example programs to illustrate the features of this library
 - Change the folder structure to comply with the arduino library spec
 - Creating a more generic version of the deserialization handler without sd card support
 - Improving the primitive packet to use less data (breaking change)
 - Possibly add a yield command to add a delay to the execution of commands in a macro file
 - Possibly add support for other display types that are supported by the Adafruit GFX library
