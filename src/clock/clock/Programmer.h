#ifndef __PROGRAMMER_H__
#define __PROGRAMMER_H__
#include "Arduino.h"
#include <Wire.h>


class Programmer {
	public:
		Programmer(void);
		void begin(uint8_t addr);
		bool exists(void);
		bool setTime(uint8_t hour, uint8_t minute, uint8_t second);
		bool getTime(uint8_t* hour, uint8_t* minute, uint8_t* second);
		bool setLedMode(uint8_t mode);
		bool getLedMode(uint8_t *mode);
		bool worldStop(void);

	private:
		uint8_t i2c_addr;
};

#endif



