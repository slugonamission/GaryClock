#include <Wire.h>

#define RTC_ADDR 0x68
#define HOUR_PIN 9
#define MIN_PIN  10
#define SEC_PIN  11

// IMPORTANT: PINS 4 AND 5 ARE RESERVED FOR I2C

void setup()
{
  Wire.begin();
  pinMode(HOUR_PIN, OUTPUT);
  pinMode(MIN_PIN, OUTPUT);
  pinMode(SEC_PIN, OUTPUT);
}

void loop()
{
}
