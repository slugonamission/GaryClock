#include "clock.h"


void setup() {
	// Sanity delay
	delay(1000);

	// Set up voltmeters
	meterL.begin(METERL_PIN);
	meterM.begin(METERM_PIN);
	meterR.begin(METERR_PIN);

	// Set up displays
	//segL.begin(SEGL_ADDR);
	//segM.begin(SEGM_ADDR);
	//segR.begin(SEGR_ADDR);

	// Set up LEDs
	leds.begin();

	// Set up button
	//pinMode(BTN_PIN, INPUT);
	// TODO: Set up button interrupt

	// Set up RTC
	// TODO: Set up RTC

	// Set up programmer
	programmer.begin(PROG_ADDR);


	// Test displays
	//segL.print(1111, DEC);
	//segM.print(2222, DEC);
	//segR.print(3333, DEC);
	//segL.writeDisplay();
	//segM.writeDisplay();
	//segR.writeDisplay();
	//delay(1000);
	//segL.clearDisplay();
	//segM.clearDisplay();
	//segR.clearDisplay();
	//delay(1000);

	// Test voltmeters
	meterL.moveDamped(255);
	meterM.moveDamped(255);
	meterR.moveDamped(255);
	delay(1000);
	meterL.moveDamped(0);
	meterM.moveDamped(0);
	meterR.moveDamped(0);
	delay(1000);

	// Test LEDs
	leds.introAnimation();
}


void loop() {

}
