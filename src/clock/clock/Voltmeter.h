#ifndef __VOLTMETER_H__
#define __VOLTMETER_H__

#include "Arduino.h"

// Damping parameters
#define USE_DAMPING   1
#define DAMP_DELAY    125
#define DAMP_AMOUNT   72
#define DAMP_DURATION (DAMP_DELAY + DAMP_AMOUNT)

class Voltmeter {
	public:
		Voltmeter(void);
		void begin(int pin);
                void move(int desiredPosition);
		void moveDamped(int desiredPosition);
		void moveDamped(int desiredPosition, int currentPosition);
		//void moveMultipleDamped(int pins[], int count, int currentPosition, int desiredPosition);

	private:
  		uint8_t pwmPin;
  		int position;
};

#endif
