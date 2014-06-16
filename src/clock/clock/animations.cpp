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
#define NUM_ANIMS 9
boolean twinkle(Leds *leds, int frame);
boolean huecycle(Leds *leds, int frame);
boolean quicksweep(Leds *leds, int frame);
boolean singlepulse(Leds *leds, int frame);
boolean whitetwinkle(Leds *leds, int frame);
boolean wipe(Leds *leds, int frame);
boolean wave1(Leds *leds, int frame);
boolean wave2(Leds *leds, int frame);
boolean slowtwinkle(Leds *leds, int frame);

//Array of pointers to animations
animptr animations[NUM_ANIMS] = {
	twinkle, //0
	huecycle, //1
	quicksweep, //2
	singlepulse, //3
	whitetwinkle, //4
	wipe, //5
	wave1, //6
	wave2, //7
	slowtwinkle, //8
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
	while(animation_tick(leds)) {
		delay(10);
	}
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

boolean twinkle_int(Leds *leds, int frame, boolean randcol, boolean slow) {
	if(frame < 150) {
		if(randcol) leds->leds[random(NUM_LEDS)] = CHSV(random(256), 255, 255);
		else leds->leds[random(NUM_LEDS)] = CRGB(255, 255, 255);
	}
	boolean stillOn;
	if(!slow) stillOn = fadeAllLeds(leds, 10);
	else stillOn = fadeAllLeds(leds, 1);
	FastLED.show();
	return stillOn;
}

boolean twinkle(Leds *leds, int frame) {
	twinkle_int(leds, frame, true, false);
}

boolean whitetwinkle(Leds *leds, int frame) {
	twinkle_int(leds, frame, false, false);
}

boolean slowtwinkle(Leds *leds, int frame) {
	twinkle_int(leds, frame, false, true);
}

//--------------------------------------------------------------------------

#define HUECYCLE_MAXFRAMES 256

boolean huecycle(Leds *leds, int frame) {
	int hue = (frame % 128) * 2;

	int basevalue;
	if(frame < 64) basevalue = frame * 4;
	else if(frame > HUECYCLE_MAXFRAMES - 64) basevalue = (HUECYCLE_MAXFRAMES - frame) * 4;
	else basevalue = 255;

	for(int x = 0; x < LED_WIDTH; x++) {
		int value = basevalue - (abs(6 - x) * 10);
		if (value < 0) value = 0;

		for(int y = 0; y < LED_HEIGHT; y++) {
			leds->leds[leds->ledcoord(x, y)] = CHSV(hue, 255, value);
		}
	}

	FastLED.show();

	return frame < HUECYCLE_MAXFRAMES;
}

//--------------------------------------------------------------------------

boolean quicksweep(Leds *leds, int frame) {
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

//--------------------------------------------------------------------------

boolean singlepulse(Leds *leds, int frame) {
	static int hue;
	if(frame == 0) hue = random(256);

	int value = frame < 128 ? constrain(255-(abs(64 - frame) * 4), 0, 255) : 0;

	for(int i = 0; i < NUM_LEDS; i++) leds->leds[i] = CHSV(hue, 255, value);
	FastLED.show();
	return (frame < 128);
}

//--------------------------------------------------------------------------

boolean wipe(Leds *leds, int frame) {
	static int progress;
	static int hue;
	if(frame == 0) {
		progress = -3;
		hue = random(256);
	}

	if(progress <= LED_WIDTH) {
		if(frame % 2 == 0) progress++;
		for(int x = -1; x < progress; x++) {
			leds->setLed(x+1, 0, CHSV(hue, 255, 255));
			leds->setLed(x, 1, CHSV(hue, 255, 255));
			leds->setLed(x-1, 2, CHSV(hue, 255, 255));
		}
	}

	boolean stillOn = fadeAllLeds(leds, 2);
	FastLED.show();
	return (frame < 100 || stillOn);
}

//--------------------------------------------------------------------------

#define WAVE_MAXPHASE 22.5

boolean wave_int(Leds* leds, int frame, boolean antiphase) {
	static float phase;
	if(frame == 0) {
		phase = 0.0;
	}

	phase = phase + PI/22.5;

	if(phase < WAVE_MAXPHASE) {
		for(int x = 0; x < 10; x++) {
			int y = round(sin(((float)x/9.0 * 2.0 * PI) + phase));
			
			if(antiphase && x%2 ==0)
				y = -y;

			leds->setLed(x, y+1, CHSV(round((phase/WAVE_MAXPHASE)*255), 255, 255));
		}
	}

	boolean stillOn = fadeAllLeds(leds, 15);
	FastLED.show();
	return (frame < 100 || stillOn);
}

boolean wave1(Leds* leds, int frame) {
	wave_int(leds, frame, false);
}

boolean wave2(Leds* leds, int frame) {
	wave_int(leds, frame, true);
}

