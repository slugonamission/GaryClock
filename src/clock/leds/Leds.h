#include "Arduino.h"
#include "FastLED.h"


#define LEDS_PIN 6
#define NUM_LEDS 30


static const uint8_t loopPattern[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19};

static const uint8_t logoLeds[] = {3, 4, 5, 6, 13, 14, 15, 16, 23, 24, 25, 26};
static const uint8_t logoOuterLeds1[] = {2, 7, 12, 17, 22, 27};
static const uint8_t logoOuterLeds2[] = {1, 8, 11, 18, 21, 28};
static const uint8_t logoOuterLeds3[] = {0, 9, 10, 19, 20, 29};

static const uint8_t top[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
static const uint8_t mid[] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
static const uint8_t bot[] = {29, 28, 27, 26, 25, 24, 23, 22, 21, 20};


class Leds {
	public:
		Leds(void);
		void begin(void);
		void introAnimation(void);

	private:
  		CRGB leds[NUM_LEDS];  		
};
