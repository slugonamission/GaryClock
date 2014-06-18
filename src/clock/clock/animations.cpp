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
boolean colour(Leds *leds, int frame);
boolean batshit(Leds *leds, int frame);
boolean singlepulse_lowpower(Leds *leds, int frame);
boolean twinkle(Leds *leds, int frame);
boolean huecycle(Leds *leds, int frame);
boolean quicksweep(Leds *leds, int frame);
boolean singlepulse(Leds *leds, int frame);
boolean whitetwinkle(Leds *leds, int frame);
boolean wipe(Leds *leds, int frame);
boolean wave1(Leds *leds, int frame);
boolean wave2(Leds *leds, int frame);
boolean slowtwinkle(Leds *leds, int frame);
boolean scrolltext(Leds *leds, int frame);
boolean blinds_fast(Leds *leds, int frame);
boolean blinds_slow(Leds *leds, int frame);
boolean colourwipe(Leds *leds, int frame);
boolean cylon(Leds *leds, int frame);

//Array of pointers to animations
//Wastes some memory, but forms a jump table for the interface functions
animptr animations[ANIMS_NUM] = {
	colour, //0
	batshit, //1
	singlepulse_lowpower, //2
	twinkle, //3
	huecycle, //4
	quicksweep, //5
	singlepulse, //6
	whitetwinkle, //7
	wipe, //8
	wave1, //9
	wave2, //10
	slowtwinkle, //11
	scrolltext, //12
	blinds_fast, //13
	blinds_slow, //14
	colourwipe, //15
	cylon, //16
};


//--------------------------------------------------------------------------
// Interface functions

int current_frame = 0; 
int current_animation = -1;

boolean animation_tick(Leds *leds) {
	if(current_animation >= 0 && current_animation < ANIMS_NUM) {
		boolean stillRunning = animations[current_animation](leds, current_frame);

		if(stillRunning) {
			current_frame++;
			return true;
		} else {
			//The animation reported it was finished, so no sense in running a dead animation any more.
			current_animation = -1;
			return false;
		}
	}
	return false;
}


void set_animation(Leds *leds, int num) {
	current_animation = num;
	current_frame = 0;
	leds->turnAllOff();
}

void test_animation(Leds *leds, int anim) {
	leds->turnAllOff();
	set_animation(leds, anim);
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
	return still_on;
}

//--------------------------------------------------------------------------

#define COLOUR_TIME_STEP 1
#define COLOUR_COUNTDOWN 2000
#define COLOUR_STEP 16

boolean colour(Leds *leds, int frame) {
	static int offset;
	if(frame == 0) offset = 0;

	if(frame % COLOUR_COUNTDOWN == 0) {
		for(int i = 0; i < LED_WIDTH; i++) {
			leds->leds[top[i]] = CHSV(offset + i*COLOUR_STEP, 255, 120);
			leds->leds[mid[i]] = CHSV(offset + i*COLOUR_STEP, 255, 120);
			leds->leds[bot[i]] = CHSV(offset + i*COLOUR_STEP, 255, 120);
		}
		offset += COLOUR_TIME_STEP;
	}
	FastLED.show();
	return true;
}

//--------------------------------------------------------------------------

#define BATSHITMODES 4
#define BATSHITMODECHANGE 760


boolean batshit(Leds *leds, int frame) {
	static int bsmode;
	static int frameToChangeAt;
	static int mode2progress;
	static int currenthue;

	if(frame == 0) {
		bsmode = random(BATSHITMODES);
		frameToChangeAt = 0;
	}

	if(frame >= frameToChangeAt) {
		frameToChangeAt = BATSHITMODECHANGE + random(BATSHITMODECHANGE / 4);
		int currentMode = bsmode;
		while(bsmode == currentMode) {
			bsmode = random(BATSHITMODES);
			mode2progress = -3;
			currenthue = random(255);
		}
	}

	switch(bsmode) {
		case 0:
			if(frame % 50 == 0) {
				int hue = random(256);
				for(int x = 0; x < LED_WIDTH; x++) {
					if(x % 2 == 0) hue = random(256);
					for(int y = 0; y < 3; y++) leds->setLed(x, y, CHSV(hue, 255, 255));
				}
			}
			break;
		case 1:
			if(frame % 50 == 0) {
				for(int y = 0; y < LED_HEIGHT; y++) {
					int hue = random(256);
					for(int x = 0; x < LED_WIDTH; x++) {
						leds->setLed(x, y, CHSV(hue, 255, 255));
					}
				}
			}
			break;
		case 2:
			if(mode2progress <= LED_WIDTH) {
				if(frame % 2 == 0) mode2progress++;
				for(int x = -1; x < mode2progress; x++) {
					leds->setLed(x+1, 0, CHSV(currenthue, 255, 255));
					leds->setLed(x, 1, CHSV(currenthue, 255, 255));
					leds->setLed(x-1, 2, CHSV(currenthue, 255, 255));
				}
			} else{
				mode2progress = -3;
				currenthue = random(255);
			}
			break;
		case 3:
			if(frame % 20 == 0) {
				for(int i = 0; i < NUM_LEDS; i++) leds->leds[i] = CHSV(currenthue, 255, 255);
			} else {
				fadeAllLeds(leds, 10);
			}
			break;
	}

	FastLED.show();
	return true;
}

//--------------------------------------------------------------------------

boolean twinkle_int(Leds *leds, int frame, boolean randcol, boolean slow) {
	if(frame < 150) {
		if(randcol) leds->leds[random(NUM_LEDS)] = CHSV(random(256), 255, 255);
		else leds->leds[random(NUM_LEDS)] = CRGB(255, 255, 255);
	}
	boolean stillOn = fadeAllLeds(leds, slow ? 2 : 10);
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

#define SP_FRAMES 70

boolean singlepulse_int(Leds *leds, int frame, boolean lowpower) {
	static int hue;
	if(frame == 0) hue = random(256);

	int value = frame < SP_FRAMES
		? round(((float) (SP_FRAMES/2 - abs((SP_FRAMES / 2) - frame)) / (float) SP_FRAMES) * 255) 
		: 0;

	if(lowpower) {
		for(int i = 0; i < NUM_LEDS; i++) leds->leds[i] = CHSV(hue, 255, round((float)value / 1.5));
	} else {
		for(int i = 0; i < NUM_LEDS; i++) leds->leds[i] = CHSV(hue, 255, value);
	}

	FastLED.show();
	return (frame < SP_FRAMES);
}

boolean singlepulse(Leds *leds, int frame) {
	singlepulse_int(leds, frame, false);
}

boolean singlepulse_lowpower(Leds *leds, int frame) {
	singlepulse_int(leds, frame, true);
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

//--------------------------------------------------------------------------

char tm[3][38] = {	"*** *** * * *** * * *** *** *** *** ",
					" *  * * * * *   *** *** * * **  **  ",
					" *  *** *** *** * * * * *** * * *** "};

boolean scrolltext(Leds *leds, int frame) {
	static int slen;
	static int pos = 0;

	if(frame == 0) {
		pos = 0;
		slen = strlen(tm[0]);
	}

	for(int x = 0; x < LED_WIDTH; x++) {
		int offset = x + (pos - slen);

		if(offset >= 0 && offset < slen) {
			for(int y = 0; y < 3; y++) {
				if(tm[y][offset] != ' ') {
					leds->setLed(x, y, CHSV((offset / 4)*(255/5), 255, 255));
				} else {
					leds->setLed(x, y, CHSV(0, 0, 0));
				}
			}
		}
	}

	if(frame % 5 == 0) pos++;

	FastLED.show();
	return pos < slen * 2 + LED_WIDTH;
}

//--------------------------------------------------------------------------

boolean blinds_int(Leds *leds, int frame, boolean fast) {
	static int pos;
	static int loops;

	if(frame == 0) {
		pos = 0;
		loops = fast ? 2 : 1;
	}

	int hue = round(((float) pos / 10.0) * 255.0);
	leds->setLed(pos, 0, CHSV(hue, 255, 255));
	leds->setLed(LED_WIDTH-pos, 1, CHSV(hue, 255, 255));
	leds->setLed(pos, 2, CHSV(hue, 255, 255));

	if(fast || frame % 2 == 0) pos++;

	if(pos == LED_WIDTH && loops > 1) {
		loops = 1;
		pos = 0;
	}

	boolean stillOn = fadeAllLeds(leds, fast ? 50 : 10);
	FastLED.show();

	return (pos < 10 || stillOn);
}

boolean blinds_fast(Leds *leds, int frame) {
	blinds_int(leds, frame, true);
}

boolean blinds_slow(Leds *leds, int frame) {
	blinds_int(leds, frame, false);
}

//--------------------------------------------------------------------------

#define COLOURWIPE_INTERDELAY 10
#define COLOURWIPE_NUMWIPES 4

boolean colourwipe(Leds *leds, int frame) {
	static int wipepos;
	static int wipenum;
	boolean stillOn;

	if(frame == 0) {
		wipepos = 0;
		wipenum = 0;
	}

	if(wipenum < COLOURWIPE_NUMWIPES) {
		stillOn = true;

		if(wipepos < LED_WIDTH) {
			for(int y = 0; y < 3; y++) {
				switch(wipenum) {
					case 0:	
						leds->leds[leds->ledcoord(wipepos, y)].r += 255;
						break;
					case 1:	
						leds->leds[leds->ledcoord((LED_WIDTH - 1) - wipepos, y)].g += 255;
						break;
					case 2:
						leds->leds[leds->ledcoord(wipepos, y)].r = 0;
						leds->leds[leds->ledcoord(wipepos, y)].b += 255;
						break;
					case 3:
						leds->leds[leds->ledcoord((LED_WIDTH - 1) - wipepos, y)].r += 255;
						break;
				}
			}
		}

		if(frame % 2 == 0) wipepos++;
		if(wipepos >= LED_WIDTH + COLOURWIPE_INTERDELAY) {
			wipepos = 0;
			wipenum++;
		}		
	} else {
		stillOn = fadeAllLeds(leds, 8);
	}

	FastLED.show();
	return (wipenum < COLOURWIPE_NUMWIPES || stillOn);
}

//-------------------------------------------------------------------------

#define CYLON_LOOPFRAMES 150
#define CYLON_TOTALFRAMES 300

boolean cylon(Leds *leds, int frame) {
	if(frame < CYLON_TOTALFRAMES-1) {
		int x = round(sin((float)frame / (float)CYLON_LOOPFRAMES * 2 * PI) * (LED_WIDTH-1));
		leds->setLed(x, 1, CRGB(128, 0, 0)); 
	}
	boolean stillOn = fadeAllLeds(leds, 5);
	FastLED.show();
	return (frame < CYLON_TOTALFRAMES || stillOn);
}
