#include "clock.h"
#include "animations.h"
#include <Wire.h>

uint8_t curTime[] = {0, 0, 0};

uint8_t meterHoffset[12];
uint8_t meterMoffset[60];
uint8_t meterSoffset[60];

// 7-segment displays (left, middle, right)
//SevenSeg segL = SevenSeg();
//SevenSeg segM = SevenSeg();
//SevenSeg segR = SevenSeg();

Voltmeter meterH = Voltmeter();
Voltmeter meterM = Voltmeter();
Voltmeter meterS = Voltmeter();
Voltmeter allMeters[3];
int metersToUpdate = 0;
int meterPositions[3];

Leds leds = Leds();

Programmer programmer = Programmer();
bool gotProgrammer = 0;
bool stopWorld = 0;

// This is to allow the programmer loop to be able to skip iterations. This is because if each iteration
// of loop() caused an I2C transaction, the programmer would basically stop due to the number of incoming
// transactions. This then allows loop() to drive the LED patterns and still query the programmer.
#define PROGRAMMER_SKIP_NUM 1
volatile int programmerLoopSkip = 1;

void rtcTick()
{
	programmerLoopSkip--;
	if(stopWorld)
		return;
	// DO NOT ISSUE AN I2C TRANSACTION IN THIS ISR.
	// WE ISSUE THESE IN loop(). RACE CONDITIONS CAN ARISE.
	// Also, remember that DELAYS DO NOT WORK in ISRs.
	curTime[2]++;
	if(curTime[2] == 60)
	{
		curTime[2] = 0;
		curTime[1]++;

		if(curTime[1] == 60)
		{
			curTime[1] = 0;
			curTime[0]++;

			if(curTime[0] == 12)
			{
				curTime[0] = 0;
			}

			metersToUpdate++;
			meterPositions[2] = meterHoffset[curTime[0]];
		}

		metersToUpdate++;
		meterPositions[1] = meterMoffset[curTime[1]];
	}

	metersToUpdate++;
	meterPositions[0] = meterSoffset[curTime[2]];
}

void setup() {
	// Sanity delay
	delay(1000);

	// Set up voltmeters
	meterH.begin(METER_MID_PIN);
	meterM.begin(METER_LEFT_PIN);
	meterS.begin(METER_RIGHT_PIN);
	allMeters[0] = meterS;
	allMeters[1] = meterM;
	allMeters[2] = meterH;

	// Set up displays
	//segL.begin(SEGL_ADDR);
	//segM.begin(SEGM_ADDR);
	//segR.begin(SEGR_ADDR);

	// Set up button
	//pinMode(BTN_PIN, INPUT);
	// TODO: Set up button interrupt

	// Set up LEDs
	leds.begin();

	// Set up programmer
	programmer.begin(PROG_ADDR);

	// Set the offsets
	// Precalc to save time. Also allows setting stupid offsets later.
	for (int i = 0; i < 12; i++)
	{
		float tmp = 255.0f / 11.0f;
		tmp = tmp * (float)i;
		meterHoffset[i] = (uint8_t)tmp;
	}

	for (int i = 0; i < 60; i++)
	{
		float tmp = 255.0f / 59.0f;
		tmp = tmp * (float)i;
		meterMoffset[i] = (uint8_t)tmp;
	}

	for (int i = 0; i < 60; i++)
	{
		float tmp = 255.0f / 59.0f;
		tmp = tmp * (float)i;
		meterSoffset[i] = (uint8_t)tmp;
	}
	

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
	Voltmeter::moveMultipleDamped(allMeters, 3, 255);
	delay(1000);
	Voltmeter::moveMultipleDamped(allMeters, 3, 0);
	delay(1000);

	// Test LEDs
	//leds.introAnimation();

	/*for(int i = 0; i < get_num_animations(); i++) {
		test_animation(&leds, i);
		delay(500);
	}*/

	test_animation(&leds, 0);
	delay(500);
	test_animation(&leds, 4);
	delay(500);	
	test_animation(&leds, 9);

	// Get the current time from RTC
	time_t tz = RTC.get(); 
	curTime[0] = hour(tz) % 12;
	curTime[1] = minute(tz);
	curTime[2] = second(tz);

	// Set the voltmeters
	meterPositions[0] = meterSoffset[curTime[2]];
	meterPositions[1] = meterMoffset[curTime[1]];
	meterPositions[2] = meterHoffset[curTime[0]];
	Voltmeter::moveMultipleDamped(allMeters, 3, meterPositions);


	// Set the RTC interrupt callback
	// THE MODE MAY NOT BE CORRECT. CHECK IT
	// Actually...it shouldn't matter...
	pinMode(2, INPUT_PULLUP);
	attachInterrupt(0, rtcTick, RISING);

	// Enable the 1Hz oscillator
	if(!RTC.setTickMode(true))
		leds.errorAnimation();
}


void loop() {
	// Is the programmer there?
	leds.tick();

	// Update Voltmeters here so we can use delays to debounce
	if (metersToUpdate > 0 && !stopWorld) {
		Voltmeter::moveMultipleDamped(allMeters, metersToUpdate, meterPositions);
		metersToUpdate = 0;
	}

	if(programmerLoopSkip != 0)
		return;

	programmerLoopSkip = PROGRAMMER_SKIP_NUM;

	stopWorld = programmer.worldStop();

	if (!gotProgrammer && programmer.exists())
	{
		programmer.setLedMode(leds.getMode());
		programmer.setTime(curTime[0], curTime[1], curTime[2]);
		//delay(1000);
		gotProgrammer = true;
	}
	else if (gotProgrammer)
	{
		uint8_t nextLed = 0;

		if (stopWorld && programmer.getTime(&curTime[0], &curTime[1], &curTime[2]))
		{
			tmElements_t tz;
			RTC.read(tz);
			tz.Hour = curTime[0];
			tz.Minute = curTime[1];
			tz.Second = curTime[2];
			RTC.write(tz);

			meterPositions[0] = meterSoffset[curTime[2]];
			meterPositions[1] = meterMoffset[curTime[1]];
			meterPositions[2] = meterHoffset[curTime[0]];
			Voltmeter::moveMultipleDamped(allMeters, 3, meterPositions);
			metersToUpdate = 0;
		}
		else if (programmer.getLedMode(nextLed)) // Only need to do this if the world isn't stopped, hence there's a chance the user is on the LED page.
		{
			if (nextLed != leds.getMode())
				leds.setMode(nextLed);
		}
		else
		{
			gotProgrammer = false;
		}
	}
}



