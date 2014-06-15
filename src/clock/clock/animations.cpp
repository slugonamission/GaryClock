#include "animations.h"
#include "leds.h"

/*
All animations must follow this prototype
First argument is a pointer to an Leds object that it can use for LED output.
Second argument is the current frame number. This should be set back to 0 to
reinitialise the animation.
*/

typedef boolean (*animptr)(Leds *, int);

//Currently defined animation prototypes
#define NUM_ANIMS 3
boolean twinkle_tick(Leds *leds, int frame);
boolean huecycle_tick(Leds *leds, int frame);
boolean quicksweep_tick(Leds *leds, int frame);


//Array of pointers to animations
animptr animations[NUM_ANIMS] = {
	twinkle_tick, huecycle_tick, quicksweep_tick
};


//--------------------------------------------------------------------------
// Interface functions

int current_frame;
int current_animation;

boolean animation_tick(Leds *leds) {
	boolean stillRunning = animations[current_animation](leds, current_frame);
	current_frame++;
	return stillRunning;
}


void set_animation(int num) {
	if(num < NUM_ANIMS) {
		current_animation = num;
	} else {
		current_animation = 0;
	}

	current_frame = 0;
}

int get_num_animations() {
	return NUM_ANIMS;
}


void test_animation(Leds *leds, int anim) {
	set_animation(anim);
	while(animation_tick(leds));
}

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

boolean twinkle_tick(Leds *leds, int frame) {
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

boolean huecycle_tick(Leds *leds, int frame) {
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

boolean quicksweep_tick(Leds *leds, int frame) {
	static int tickCount;
	static int x;

	if(frame == 0) {
		tickCount = 5;
		x = LED_WIDTH;
	}

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
