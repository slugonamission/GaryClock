#ifndef __VOLTMETER_H__
#define __VOLTMETER_H__

#include "Arduino.h"

// Damping parameters
#define USE_DAMPING   1
#define DAMP_DELAY    120
#define DAMP_AMOUNT   69
#define DAMP_DURATION (DAMP_DELAY + DAMP_AMOUNT)

class Voltmeter {
	public:
		Voltmeter(void);
		void begin(int pin);
		void move(int desiredPosition);
		void moveDamped(int desiredPosition);
		void moveDamped(int desiredPosition, int currentPosition);
		static void moveMultipleDamped(Voltmeter meters[], int count, int desiredPosition);
		static void moveMultipleDamped(Voltmeter meters[], int count, int desiredPositions[]);
		uint8_t getPosition(void) { return position; }
	private:
		uint8_t pwmPin;
		uint8_t position;
};

#endif



