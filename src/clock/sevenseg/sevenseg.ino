/*************************************************** 
  This is a library for our I2C LED Backpacks

  Designed specifically to work with the Adafruit LED 7-Segment backpacks 
  ----> http://www.adafruit.com/products/881
  ----> http://www.adafruit.com/products/880
  ----> http://www.adafruit.com/products/879
  ----> http://www.adafruit.com/products/878

  These displays use I2C to communicate, 2 pins are required to 
  interface. There are multiple selectable I2C addresses. For backpacks
  with 2 Address Select pins: 0x70, 0x71, 0x72 or 0x73. For backpacks
  with 3 Address Select pins: 0x70 thru 0x77

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

// Enable one of these two #includes and comment out the other.
// Conditional #include doesn't work due to Arduino IDE shenanigans.
#include <Wire.h> // Enable this line if using Arduino Uno, Mega, etc.
//#include <TinyWireM.h> // Enable this line if using Adafruit Trinket, Gemma, etc.

#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

Adafruit_7segment matrix1 = Adafruit_7segment();
Adafruit_7segment matrix2 = Adafruit_7segment();
Adafruit_7segment matrix3 = Adafruit_7segment();

static const uint8_t numbertable[] = {
  0x3F, /* 0 */
  0x06, /* 1 */
  0x5B, /* 2 */
  0x4F, /* 3 */
  0x66, /* 4 */
  0x6D, /* 5 */
  0x7D, /* 6 */
  0x07, /* 7 */
  0x7F, /* 8 */
  0x6F, /* 9 */
  0x77, /* a */
  0x7C, /* b */
  0x39, /* C */
  0x5E, /* d */
  0x79, /* E */
  0x71, /* F */
};

void setup() {
#ifndef __AVR_ATtiny85__
  Serial.begin(9600);
  Serial.println("7 Segment Backpack Test");
#endif
  matrix1.begin(0x72);
  matrix2.begin(0x71);
  matrix3.begin(0x70);

/*
  matrix1.print(0x0123, HEX);
  matrix1.writeDigitRaw(0, numbertable[0]);
  matrix1.writeDisplay();
  matrix2.print(0x4567, HEX);
  matrix2.writeDisplay();
  matrix3.print(0x89AB, HEX);
  matrix3.writeDisplay();
  */

  matrix1.writeDigitRaw(0, 0xFF);
  matrix1.writeDigitRaw(1, 0xFF);
  matrix1.writeDigitRaw(2, 0xFF);
  matrix1.writeDigitRaw(3, 0xFF);
  matrix1.writeDigitRaw(4, 0xFF);
  matrix2.writeDigitRaw(0, 0xFF);
  matrix2.writeDigitRaw(1, 0xFF);
  matrix2.writeDigitRaw(2, 0xFF);
  matrix2.writeDigitRaw(3, 0xFF);
  matrix2.writeDigitRaw(4, 0xFF);
  matrix3.writeDigitRaw(0, 0xFF);
  matrix3.writeDigitRaw(1, 0xFF);
  matrix3.writeDigitRaw(2, 0xFF);
  matrix3.writeDigitRaw(3, 0xFF);
  matrix3.writeDigitRaw(4, 0xFF);
  matrix1.writeDisplay();
  matrix2.writeDisplay();
  matrix3.writeDisplay();
}

void loop() {

/*
  for (int i = 0; i < 15; i++) {
    matrix1.setBrightness(i);
    matrix2.setBrightness(i);
    matrix3.setBrightness(i);
    delay(50);
  }

  for (int i = 15; i > 0; i--) {
    matrix1.setBrightness(i);
    matrix2.setBrightness(i);
    matrix3.setBrightness(i);
    delay(50);
  }
  */

  /*
  // try to print a number thats too long
  matrix1.print(10000, DEC);
  matrix1.writeDisplay();
  matrix2.print(10000, DEC);
  matrix2.writeDisplay();
  matrix3.print(10000, DEC);
  matrix3.writeDisplay();
  delay(500);
Serial.println("test");
  // print a hex number

  delay(500);


  // print with print/println
  for (uint16_t counter = 0; counter < 9999; counter++) {
    matrix1.println(counter);
    matrix1.writeDisplay();
    matrix2.println(counter);
    matrix2.writeDisplay();
    matrix3.println(counter);
    matrix3.writeDisplay();
    delay(10);
  }

  // method #2 - draw each digit
  uint16_t blinkcounter = 0;
  boolean drawDots = false;
  for (uint16_t counter = 0; counter < 9999; counter ++) {
    matrix1.writeDigitNum(0, (counter / 1000), drawDots);
    matrix1.writeDigitNum(1, (counter / 100) % 10, drawDots);
    matrix1.drawColon(drawDots);
    matrix1.writeDigitNum(3, (counter / 10) % 10, drawDots);
    matrix1.writeDigitNum(4, counter % 10, drawDots);
    matrix2.writeDigitNum(0, (counter / 1000), drawDots);
    matrix2.writeDigitNum(1, (counter / 100) % 10, drawDots);
    matrix2.drawColon(drawDots);
    matrix2.writeDigitNum(3, (counter / 10) % 10, drawDots);
    matrix2.writeDigitNum(4, counter % 10, drawDots);
    matrix3.writeDigitNum(0, (counter / 1000), drawDots);
    matrix3.writeDigitNum(1, (counter / 100) % 10, drawDots);
    matrix3.drawColon(drawDots);
    matrix3.writeDigitNum(3, (counter / 10) % 10, drawDots);
    matrix3.writeDigitNum(4, counter % 10, drawDots);
   
    blinkcounter+=50;
    if (blinkcounter < 500) {
      drawDots = false;
    } else if (blinkcounter < 1000) {
      drawDots = true;
    } else {
      blinkcounter = 0;
    }
    matrix1.writeDisplay();
    matrix2.writeDisplay();
    matrix3.writeDisplay();
    delay(10);
  }
  */
}
