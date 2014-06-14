#include "Leds.h"


Leds::Leds() {
	mode = LEDMODE_COLOUR;
	currentColourOffset = 0;
	currentColourCountdown = LEDMODE_COLOUR_COUNTDOWN;
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


void Leds::setMode(uint8_t mode)
{
	turnAllOff();

	// TODO: Do any state switching required.
  
	this->mode = mode;
}

// Called from loop(). Causes the animation to (optionally) skip to the next stage.
// This operates in this way (rather than just busy waiting) in order to
// allow loop() to query the programmer every PROGRAMMER_SKIP_NUM cycles, otherwise
// the LED routine would effectively block the programmer query.
void Leds::tick()
{
	switch(mode)
	{
		case LEDMODE_OFF:
			return;
		case LEDMODE_SMALL:
			tickSmall();
			break;
		case LEDMODE_COLOUR:
			tickColour();
			break;
		case LEDMODE_PULSE:
			tickPulse();
			break;
		case LEDMODE_BATSHIT:
			tickBatshit();
			break;
	}
}

void Leds::tickSmall()
{
}

void Leds::tickColour()
{
	if(currentColourCountdown--)
		return;
	currentColourCountdown = LEDMODE_COLOUR_COUNTDOWN;

	for(int i = 0; i < LED_WIDTH; i++)
	{
		leds[top[i]] = CHSV(currentColourOffset + i*LEDMODE_COLOUR_STEP, 255, 120);
		leds[mid[i]] = CHSV(currentColourOffset + i*LEDMODE_COLOUR_STEP, 255, 120);
		leds[bot[i]] = CHSV(currentColourOffset + i*LEDMODE_COLOUR_STEP, 255, 120);
	}
	FastLED.show();

	currentColourOffset += LEDMODE_COLOUR_TIME_STEP;
}

void Leds::tickPulse()
{
}

void Leds::tickBatshit()
{
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
