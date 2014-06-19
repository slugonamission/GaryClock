#include "Leds.h"
#include "animations.h"

Leds::Leds() {
	mode = LEDMODE_OFF;
}

void Leds::begin() {
	FastLED.addLeds<WS2812B, LEDS_PIN, GRB>(leds, NUM_LEDS);

	// Turn all LEDs off
	turnAllOff();
}

void Leds::turnAllOff() {
	for (int led = 0; led < NUM_LEDS; led++) {
		leds[led] = CRGB::Black;
	}
	FastLED.show();
}

void Leds::introAnimation() {
	// Power on animation
	for (int i = 0; i < 256; i++) {
		for (int led = 0; led < sizeof(logoLeds); led++) 
			leds[logoLeds[led]] = CHSV(0, 0, i);

		FastLED.show();
		delay(2);
	}

	delay (20);

	for (int i = 0; i <= 450; i++) {
		for (int led = 0; led < sizeof(logoLeds); led++)
			if (i < 150)
				leds[logoLeds[led]] = CHSV(0, 0, min(255, 300 - i));
			else if (i >= 150)
				leds[logoLeds[led]] = CHSV(0, 0, 225 - i/2);
		
		for (int led = 0; led < sizeof(logoOuterLeds1); led++)
			if (i < 150)
				leds[logoOuterLeds1[led]] = CHSV(0, 0, i);
			else if (i < 300)
				leds[logoOuterLeds1[led]] = CHSV(0, 0, 300 - i);

		for (int led = 0; led < sizeof(logoOuterLeds2); led++)
			if (i > 30 && i < 170)
				leds[logoOuterLeds2[led]] = CHSV(0, 0, i - 30);
			else if (i >= 170 && i < 310)
				leds[logoOuterLeds2[led]] = CHSV(0, 0, 310 - i);

		for (int led = 0; led < sizeof(logoOuterLeds3); led++)
			if (i > 60 && i < 200)
				leds[logoOuterLeds3[led]] = CHSV(0, 0, i - 60);
			else if (i >= 200 && i < 340)
				leds[logoOuterLeds3[led]] = CHSV(0, 0, 340 - i);

		FastLED.show();
		delay(2);
	}

	delay(400);

	for (int i = 0; i < 13; i++) {
		if (i > 2) {
			leds[top[i-3]] = CRGB::Black;
			leds[mid[i-3]] = CRGB::Black;
		}

		if (i > 1 && i < 10) {
			leds[top[i-2]] = CHSV((i-2) * 25, 200, 50);
			leds[mid[i-2]] = CHSV((i-2) * 25, 200, 50);
		}

		if (i > 0 && i < 11) {
			leds[top[i-1]] = CHSV((i-1) * 25, 200, 130);
			leds[mid[i-1]] = CHSV((i-1) * 25, 200, 130);
		}

		if (i < 10) {
			leds[top[i]] = CHSV(i * 25, 200, 210);
			leds[mid[i]] = CHSV(i * 25, 200, 210);
		}

		FastLED.show();
		delay(16);
	}

	delay(120);

	for (int i = 0; i < 13; i++) {
		if (i > 2) {
			leds[bot[(9-i)+3]] = CRGB::Black;
			leds[mid[(9-i)+3]] = CRGB::Black;
		}

		if (i > 1 && i < 10) {
			leds[bot[(9-i)+2]] = CHSV((i-2) * 25, 200, 50);
			leds[mid[(9-i)+2]] = CHSV((i-2) * 25, 200, 50);
		}

		if (i > 0 && i < 11) {
			leds[bot[(9-i)+1]] = CHSV((i-1) * 25, 200, 130);
			leds[mid[(9-i)+1]] = CHSV((i-1) * 25, 200, 130);
		}

		if (i < 10) {
			leds[bot[(9-i)]] = CHSV(i * 25, 200, 210);
			leds[mid[(9-i)]] = CHSV(i * 25, 200, 210);
		}

		FastLED.show();
		delay(16);
	}
}


void Leds::errorAnimation() {
	// Turn all red (will do for now)
	for (int led = 0; led < 30; led++) 
		leds[led] = CRGB(100, 0, 0);

	FastLED.show();
}


void Leds::chargeNeutrinos(Voltmeter meters[]) {
	uint8_t led;
	uint8_t meterVals[3];

	FastLED.setBrightness(255);
	for (int led = 0; led < NUM_LEDS; led++)
		leds[led] = CRGB::Black;

	meterVals[0] = 0;
	meterVals[1] = 255;
	meterVals[2] = 128;
	meters[2].move(meterVals[2]);
	Voltmeter::moveMultipleDamped(meters, 2, meterVals);

	delay(2000);

	meterVals[0] = 255;
	meterVals[1] = 0;
	meterVals[2] = 128;
	meters[2].move(meterVals[2]);
	Voltmeter::moveMultipleDamped(meters, 2, meterVals);

	for (int led = 0; led < NUM_LEDS; led++)
		leds[led] = CRGB::White;

	for (int i = 0; i < 20; i++) {
		showLeds();
		delay(20);
		FastLED.showColor(CRGB::Black);
		delay(80);
	}

	meterVals[0] = 0;
	meterVals[1] = 0;
	meterVals[2] = 128;
	meters[2].move(0);
	delay(200);
	meters[0].moveDamped((uint8_t)0);
	meters[2].move(255);
	delay(200);
	meters[1].moveDamped(255);
	meters[2].move(128);
	delay(500);

	for (int led = 0; led < NUM_LEDS; led++)
		leds[led] = CRGB::Black;

	showLeds();
	for (int i = 0; i < 5; i++) {
		delay(1000);
		led = random(NUM_LEDS);
		meters[2].move(meterPos[led]);
		leds[led].setHue(random(256));
		showLeds();
		delay(100);
		leds[led] = CRGB::Black;
		showLeds();
		meters[2].move(128);
	}

	for (int led = 0; led < NUM_LEDS; led++)
		leds[led] = CRGB::Black;

	showLeds();
	for (int i = 0; i < 5; i++) {
		delay(500);
		led = random(NUM_LEDS);
		meters[2].move(meterPos[led]);
		leds[led].setHue(random(256));
		showLeds();
		delay(50);
		leds[led] = CRGB::Black;
		showLeds();
		meters[2].move(128);
	}

	for (int led = 0; led < NUM_LEDS; led++)
		leds[led] = CRGB::Black;

	showLeds();
	for (int i = 0; i < 5; i++) {
		delay(300);
		led = random(NUM_LEDS);
		meters[2].move(meterPos[led]);
		leds[led].setHue(random(256));
		showLeds();
		delay(20);
		leds[led] = CRGB::Black;
		showLeds();
		meters[2].move(128);
	}

	for (int led = 0; led < NUM_LEDS; led++)
		leds[led] = CRGB::Black;

	showLeds();
	for (int i = 0; i < 100; i++) {
		delay(304 - i*3);
		led = random(NUM_LEDS);
		meters[2].move(meterPos[led]);
		leds[led].setHue(random(256));
		showLeds();
		delay(5);
		leds[led] = CRGB::Black;
		showLeds();
		meters[2].move(128);
	}

	for (int led = 0; led < NUM_LEDS; led++)
		leds[led] = CRGB::Black;

	showLeds();
	for (int i = 0; i < 1000; i++) {
		delay(2);
		led = random(NUM_LEDS);
		meters[2].move(meterPos[led]);
		leds[led].setHue(random(256));
		showLeds();
		delay(2);
		leds[led] = CRGB::Black;
		showLeds();
		meters[2].move(128);
	}

	meterVals[0] = 0;
	meterVals[1] = 0;
	meterVals[2] = 0;
	meters[2].move(0);
	Voltmeter::moveMultipleDamped(meters, 2, (uint8_t)0);

	for (int i = 0; i < 20; i++) {
		meterVals[0]++;
		meterVals[1]++;
		meterVals[2]++;
		meters[0].move(meterVals[0]);
		meters[1].move(meterVals[1]);
		meters[2].move(meterVals[2]);
		
		for (int j = 0; j < 21; j++) {
			delay(42-(((i*j)+j)/10));
			leds[random(NUM_LEDS)].setHue(random(256));
			showLeds();
		}
	}

	for (int i = 0; i < 235; i++) {
		meterVals[0]++;
		meterVals[1]++;
		meterVals[2]++;
		meters[0].move(meterVals[0]);
		meters[1].move(meterVals[1]);
		meters[2].move(meterVals[2]);
		
		for (int j = 0; j < 17; j++) {
			delay(2);
			leds[random(NUM_LEDS)].setHue(random(256));
			showLeds();
		}
	}

	delay(800);

	for (int i = 0; i < 25; i++) {
		meterVals[0] -= 10;
		meterVals[1] -= 10;
		meterVals[2] -= 10;
		meters[0].move(meterVals[0]);
		meters[1].move(meterVals[1]);
		meters[2].move(meterVals[2]);
		showLeds();
		delay(20);
		FastLED.showColor(CRGB::Black);
		delay(80);
	}

	meters[2].move(0);
	Voltmeter::moveMultipleDamped(meters, 2, (uint8_t)0);

	delay(6000);
}

void Leds::showLeds() {
	unsigned long maxPower = 5000l;
	unsigned int total = 0;

	for (int i = 0; i < NUM_LEDS; i++) {
		total += leds[i].r;
		total += leds[i].g;
		total += leds[i].b;
	}

	// Limit power!
	if (total > maxPower) {
		unsigned int scale = (maxPower * 100l) / (long)total;

		unsigned int rScale;
		unsigned int gScale;
		unsigned int bScale;

		for (int i = 0; i < NUM_LEDS; i++) {
			rScale = leds[i].r * scale;
			gScale = leds[i].g * scale;
			bScale = leds[i].b * scale;

			leds[i].r = rScale / 100;
			leds[i].g = gScale / 100;
			leds[i].b = bScale / 100;
		}
	}

	FastLED.show();
}


void Leds::setMode(uint8_t mode)
{
	turnAllOff();

  	switch(mode) {
		case LEDMODE_OFF:
			set_animation(this, -1);
			return;
		case LEDMODE_COLOUR:
			set_animation(this, ANIM_ID_COLOUR);
			break;
		case LEDMODE_BATSHIT:
			set_animation(this, ANIM_ID_BATSHIT);
			break;

		case LEDMODE_SMALL:
		case LEDMODE_PULSE:
			set_animation(this, -1);
			//Animations are reissued every minute from clock.ino:rtcTick()
			break;
	}

	this->mode = mode;
}


//Get the ID of an LED from its coordinates
int Leds::ledcoord(int x, int y) {
	if(x >= LED_WIDTH) return 0;
	switch(y) {
		case 0: return top[x];
		case 1: return mid[x];
		default: return bot[x];
	}
}

void Leds::setLed(int x, int y, CHSV hsv) {
	if(x < 0) return;
	if(x >= LED_WIDTH) return;
	if(y < 0) return;
	if(y >= 3) return;
	leds[ledcoord(x, y)] = hsv;
}

void Leds::setLed(int x, int y, CRGB rgb) {
	if(x < 0) return;
	if(x >= LED_WIDTH) return;
	if(y < 0) return;
	if(y >= 3) return;
	leds[ledcoord(x, y)] = rgb;
}

