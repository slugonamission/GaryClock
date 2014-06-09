#include "Programmer.h"


Programmer::Programmer(void) {

}

void Programmer::begin(uint8_t addr) {
	i2c_addr = addr;
	Wire.begin();
}
