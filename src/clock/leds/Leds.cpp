#include "Leds.h"


Leds::Leds(void) {

}

void Leds::begin(void) {
	FastLED.addLeds<WS2812B, LEDS_PIN, GRB>(leds, NUM_LEDS);

	// Turn all LEDs off
	for (int led = 0; led < NUM_LEDS; led++) {
		leds[led] = CRGB::Black;
	}
	FastLED.show();
}

void Leds::introAnimation(void) {
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
