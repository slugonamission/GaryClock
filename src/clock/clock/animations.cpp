#include "animations.h"
#include "leds.h"

Animation::Animation(Leds *ledsToUse) {
	leds = ledsToUse;
	frame = 0;
}

Animation::~Animation() {}

//--------------------------------------------------------------------------
// Utility functions

int fadeAllLeds(Leds *leds, int speed) {
	int still_on = 0;
	for(int led = 0; led < NUM_LEDS; led++) {
		for(int i = 0; i < 3; i++) {
			if(leds->leds[led].raw[i] > speed) {
				leds->leds[led].raw[i] = leds->leds[led].raw[i] - speed;
				still_on = 1;
			} else if(leds->leds[led].raw[i] > 0) { //&& <= speed
				leds->leds[led].raw[i] = 0;
				still_on = 1;
			}
		}
	}
	FastLED.show();
	return still_on;
}


//--------------------------------------------------------------------------

RandomTwinkle::RandomTwinkle(Leds *leds) : Animation(leds) {}

boolean RandomTwinkle::tick() {
	frame++;
	if(frame < 2000) {
		//Every so often add a new lit LED
		if(frame % 3 == 0) {
			leds->leds[random(NUM_LEDS)] = CHSV(random(256), 255, 255);
		}
	}
	boolean stillOn = fadeAllLeds(leds, 1);
	FastLED.show();
	return stillOn;
}

//--------------------------------------------------------------------------

#define HUECYCLE_MAXFRAMES 256*4 

HueCycle::HueCycle(Leds *leds) : Animation(leds) {}

boolean HueCycle::tick() {
	frame++;

	int hue = (frame % 128) * 2;

	int basevalue;
	if(frame < 256) basevalue = frame;
	else if(frame > HUECYCLE_MAXFRAMES - 256) basevalue = HUECYCLE_MAXFRAMES - frame;
	else basevalue = 255;

	for(int x = 0; x < LED_WIDTH; x++) {
		int value = basevalue - (abs(5 - x) * 10);
		if (value < 0) value = 0;

		for(int y = 0; y < LED_HEIGHT; y++) {
			leds->leds[leds->ledcoord(x, y)] = CHSV(hue, 255, value);
		}
	}

	FastLED.show();

	return frame < HUECYCLE_MAXFRAMES;
}


//--------------------------------------------------------------------------

QuickSweep::QuickSweep(Leds *leds) : Animation(leds) {
	tickCount = 5;
	x = LED_WIDTH;
}

boolean QuickSweep::tick() {
	frame++;

	if(x >= 0 && x <= LED_WIDTH) {
		tickCount++;
		if(tickCount >= 5) {
			x--;
			tickCount == 0;
			leds->leds[leds->ledcoord(x, 0)] = CHSV(random(256), 255, 255);
			leds->leds[leds->ledcoord(x, 1)] = CHSV(random(256), 255, 255);
			leds->leds[leds->ledcoord(x, 2)] = CHSV(random(256), 255, 255);
		}
	}

	boolean stillOn = fadeAllLeds(leds, 10);
	FastLED.show();

	if(x >= 0 && x <= LED_WIDTH) {
		return true;
	} else return stillOn;
}
