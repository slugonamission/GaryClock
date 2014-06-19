Building this programmer will require a little extra effort, since we're starting to hit the limits of what will actually fit on the Arduino.

Perform the following before attempting to build this project:

* Disable USB HID and CDC for the Arduino Leonardo. Go into Java/hardware/arduino/cores/arduino/USBDesc.h and comment out `#define CDC_ENABLED` and `#define HID_ENABLED`
* The SD card library needs replacing in order to use software SPI. Remove (or rename) Java/libraries/SD and instead use the AdaFruit version from https://learn.adafruit.com/adafruit-micro-sd-breakout-board-card-tutorial/library
* Since CDC is disabled, the Arduino can no longer be reset by the PC. Hit reset on the Arduino for the serial port to be detectable, and reset before attempting to program (the L LED should be pulsing in this period). The Arduino requires a power cycle to boot
        * Even with the HID and CDC libs, it needed power cycling anyway to detect the SD card again for some reason.
