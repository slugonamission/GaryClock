#include "Programmer.h"

// I'm in a lazy mood. I'm not supporting ARDUINO < 100.
#define TIME_REG 0
#define LED_REG 1
#define STOP_REG 2

Programmer::Programmer() {

}

void Programmer::begin(uint8_t addr) {
	i2c_addr = addr;
	Wire.begin();
}

bool Programmer::exists()
{
  // Send a simple request and see if there's anything there.
  Wire.beginTransmission(i2c_addr);
  Wire.write((uint8_t)TIME_REG);
  if(Wire.endTransmission() != 0)
    return false;
  return true;
}

bool Programmer::setTime(uint8_t hour, uint8_t minute, uint8_t second)
{
  Wire.beginTransmission(i2c_addr);
  Wire.write((uint8_t)TIME_REG);
  Wire.write(hour);
  Wire.write(minute);
  Wire.write(second);
  if(Wire.endTransmission() != 0)
    return false;
  return true;
}

bool Programmer::getTime(uint8_t* hour, uint8_t* minute, uint8_t* second)
{
  Wire.beginTransmission(i2c_addr);
  Wire.write((uint8_t)TIME_REG);
  if(Wire.endTransmission() != 0)
    return false;
  
  Wire.requestFrom((uint8_t)i2c_addr, (uint8_t)3);
  if (Wire.available() < 3) return false;
  
  *hour = Wire.read();
  *minute = Wire.read();
  *second = Wire.read();
  
  return true;
}

bool Programmer::setLedMode(uint8_t mode)
{
  Wire.beginTransmission(i2c_addr);
  Wire.write((uint8_t)LED_REG);
  Wire.write(mode);
  if(Wire.endTransmission() != 0)
    return false;
  return true;
}

bool Programmer::getLedMode(uint8_t& mode)
{
  Wire.beginTransmission(i2c_addr);
  Wire.write((uint8_t)LED_REG);
  if(Wire.endTransmission() != 0)
    return false;
  
  Wire.requestFrom((uint8_t)i2c_addr, (uint8_t)1);
  if (Wire.available() < 1) return false;
  
  mode = Wire.read();
  
  return true;
}

bool Programmer::worldStop()
{
  uint8_t mode = 0;
  Wire.beginTransmission(i2c_addr);
  Wire.write((uint8_t)STOP_REG);
  if(Wire.endTransmission() != 0)
    return false;
  
  Wire.requestFrom((uint8_t)i2c_addr, (uint8_t)1);
  if (Wire.available() < 1) return false;
  
  mode = Wire.read();
  
  return mode;
}
