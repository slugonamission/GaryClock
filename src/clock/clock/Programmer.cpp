#include "Programmer.h"

// I'm in a lazy mood. I'm not supporting ARDUINO < 100.

Programmer::Programmer(void) {

}

void Programmer::begin(uint8_t addr) {
	i2c_addr = addr;
	Wire.begin();
}

bool Programmer::exists()
{
  // Send a simple request and see if there's anything there.
  
}

bool Programmer::setTime(uint8_t hour, uint8_t minute, uint8_t second)
{
}

bool Programmer::getTime(uint8_t& hour, uint8_t& minute, uint8_t& second)
{
}

bool Programmer::setLedMode(uint8_t mode)
{
}

bool Programmer::getLedMode(uint8_t& mode)
{
}
