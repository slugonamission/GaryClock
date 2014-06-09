#include "clock.h"
#include <Wire.h>

int allHands[] = {METERL_PIN, METERM_PIN, METERR_PIN};
int curTime[] = {0,0,0};

// 7-segment displays (left, middle, right)
SevenSeg segL = SevenSeg();
SevenSeg segM = SevenSeg();
SevenSeg segR = SevenSeg();

Voltmeter meterL = Voltmeter();
Voltmeter meterM = Voltmeter();
Voltmeter meterR = Voltmeter();

Leds leds = Leds();

Programmer programmer = Programmer();

void rtcTick()
{
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
    }
  }
}

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

        // Get the current time from RTC
        time_t tz = RTC.get();
        curTime[0] = hour(tz);
        curTime[1] = minute(tz);
        curTime[2] = second(tz);
        
        // Set the RTC interrupt callback
        attachInterrupt(0, rtcTick, RISING);
        
        // Enable the 1Hz oscillator
        RTC.enableTick();
}


void loop() {

}
