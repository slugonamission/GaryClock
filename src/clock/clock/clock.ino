#include <Wire.h>
#include "ds3231/ds3231.h"

#define RTC_ADDR 0x68
#define HOUR_PIN 9
#define MIN_PIN  10
#define SEC_PIN  11

// IMPORTANT: PINS 4 AND 5 ARE RESERVED FOR I2C

void setup()
{
  Wire.begin();
  DS3231_init(0);
  
  pinMode(HOUR_PIN, OUTPUT);
  pinMode(MIN_PIN, OUTPUT);
  pinMode(SEC_PIN, OUTPUT);
}

void loop()
{
}
