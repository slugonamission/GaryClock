#include <Wire.h>
#include "DS1307RTC.h"
#include "Adafruit_LEDBackpack.h"

#define SEG1ADDR 0x71
#define SEG2ADDR 0x71
#define SEG3ADDR 0x72
  
#define HOUR_PIN 9
#define MIN_PIN  10
#define SEC_PIN  11

Adafruit_7segment matrix1 = Adafruit_7segment();
//Adafruit_7segment matrix2 = Adafruit_7segment();
//Adafruit_7segment matrix3 = Adafruit_7segment();

void setup()
{  
  matrix1.begin(SEG1ADDR);
  //matrix2.begin(SEG2ADDR);
  //matrix3.begin(SEG3ADDR);
  
  delay(1000);
  
  matrix1.print(1000, DEC);
  matrix1.writeDisplay();
  
  pinMode(HOUR_PIN, OUTPUT);
  pinMode(MIN_PIN, OUTPUT);
  pinMode(SEC_PIN, OUTPUT);
}

void loop()
{
}
