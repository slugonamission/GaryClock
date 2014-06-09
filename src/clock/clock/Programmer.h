#include "Arduino.h"
#include <Wire.h>


class Programmer {
	public:
		Programmer();
		void begin(uint8_t addr);

	private:
		uint8_t i2c_addr;
};
