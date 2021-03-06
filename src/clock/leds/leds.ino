#include "FastLED.h"


#define NUM_LEDS 30
#define LEDS_PIN 6


CRGB leds[NUM_LEDS];


uint8_t loopPattern[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19};

uint8_t logoLeds[] = {3, 4, 5, 6, 13, 14, 15, 16, 23, 24, 25, 26};
uint8_t logoOuterLeds1[] = {2, 7, 12, 17, 22, 27};
uint8_t logoOuterLeds2[] = {1, 8, 11, 18, 21, 28};
uint8_t logoOuterLeds3[] = {0, 9, 10, 19, 20, 29};

uint8_t top[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
uint8_t mid[] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
uint8_t bot[] = {29, 28, 27, 26, 25, 24, 23, 22, 21, 20};

int rainbowCount = 0;
int brightness = 0;


void setup() {
	delay(1000);

	FastLED.addLeds<WS2812B, LEDS_PIN, GRB>(leds, NUM_LEDS);

	// Turn all LEDs off
	for(int led = 0; led < NUM_LEDS; led++) {
		leds[led] = CRGB::Black;
	}
	FastLED.show();

	delay(1000);


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

	delay(2000);
	
}


void loop() {

	if (brightness < 120) {
		brightness++;
		LEDS.setBrightness(brightness);
	}

	// Rainbow
	for (int j = 0; j < 10; j++) {
		leds[top[j]].setHue((rainbowCount % 255) + (j * (255 / 10)));
		leds[mid[j]].setHue((rainbowCount % 255) + (j * (255 / 10)));
		leds[bot[j]].setHue((rainbowCount % 255) + (j * (255 / 10)));
	}

	FastLED.show();
	delay(2);

	rainbowCount++;

}
