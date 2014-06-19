#include "clock.h"
#include "DS1307RTC.h"
#include "SevenSeg.h"
#include "Voltmeter.h"
#include "Leds.h"
#include "Programmer.h"
#include "animations.h"
#include <EEPROM.h>
#include <Wire.h>

uint8_t curTime[] = {0, 0, 0};

uint8_t meterHoffset[] = {15,37,60,82,104,125,146,164,185,206,227,247};
uint8_t meterMoffset[] = {9,14,18,23,27,32,37,41,46,50,55,60,64,69,73,78,82,87,91,96,100,104,108,112,116,120,124,128,132,136,140,144,148,152,156,161,165,169,173,177,181,185,188,192,195,199,202,206,209,213,216,220,224,227,231,235,239,243,246,250,254};
uint8_t meterSoffset[] = {16,20,24,27,31,35,39,43,46,50,54,58,62,66,70,74,77,81,85,89,93,97,100,104,108,112,115,119,123,126,130,134,138,141,145,149,153,157,160,164,168,172,176,180,184,188,191,195,199,203,207,211,215,218,222,226,230,234,237,241};

// 7-segment displays (left, middle, right)
//SevenSeg segL = SevenSeg();
//SevenSeg segM = SevenSeg();
//SevenSeg segR = SevenSeg();

Voltmeter meterH = Voltmeter();
Voltmeter meterM = Voltmeter();
Voltmeter meterS = Voltmeter();
Voltmeter shitMeters[2];
volatile int metersToUpdate = 0;
uint8_t meterPositions[3];

Leds leds = Leds();

Programmer programmer = Programmer();
bool gotProgrammer = 0;
bool stopWorld = 0;

// This is to allow the programmer loop to be able to skip iterations. This is because if each iteration
// of loop() caused an I2C transaction, the programmer would basically stop due to the number of incoming
// transactions. This then allows loop() to drive the LED patterns and still query the programmer.
#define PROGRAMMER_SKIP_NUM 1
volatile int programmerLoopSkip = 1;

// Forward prototypes
uint8_t make12Hour(uint8_t hour24);
uint8_t makeMeterTime(uint8_t hour24);
void rtcTick();


uint8_t make12Hour(uint8_t hour24)
{
  if(hour24 == 0)
    return 12;
  if(hour24 > 12)
    return hour24 - 12;
  return hour24;
}

uint8_t makeMeterTime(uint8_t hour24)
{
  return make12Hour(hour24) - 1;
}

void rtcTick()
{
	programmerLoopSkip--;
	if(stopWorld)
		return;
	// DO NOT ISSUE AN I2C TRANSACTION IN THIS ISR.
	// WE ISSUE THESE IN loop(). RACE CONDITIONS CAN ARISE.
	// Also, remember that DELAYS DO NOT WORK in ISRs.
	curTime[SECS]++;
	if(curTime[SECS] == 60)
	{
		curTime[SECS] = 0;
		curTime[MINS]++;

		if(curTime[MINS] == 60)
		{
			curTime[MINS] = 0;
			curTime[HOUR]++;
                        
                        if(curTime[HOUR] == 23)
                          curTime[HOUR] = 0;
                          
                        metersToUpdate++;
                        
                        meterPositions[HOUR] = meterHoffset[makeMeterTime(curTime[HOUR])];
		}

		metersToUpdate++;
		meterPositions[MINS] = meterMoffset[curTime[MINS]];

		//We just ticked the minute, so we may also need to issue a new animation
		if(leds.getMode() == LEDMODE_SMALL) {
			set_animation(&leds, random(ANIM_SMALL_NUM) + ANIM_SMALL_START);
		}
	}

	//We just ticked the second, so we may also need to issue a new animation
	if(leds.getMode() == LEDMODE_PULSE) {
		set_animation(&leds, ANIM_ID_PULSE);
	}

	metersToUpdate++;
	meterPositions[SECS] = meterSoffset[curTime[SECS]];
}


//Interrupt handler for timer2. Used to tick LED animations.
ISR(TIMER2_COMPA_vect) {
	animation_tick(&leds);
}


void setup() {
	// Sanity delay
	pinMode(ARDUINO_LED_PIN, OUTPUT);
	digitalWrite(ARDUINO_LED_PIN, HIGH);
	delay(500);
	digitalWrite(ARDUINO_LED_PIN, LOW);

	// Set up voltmeters
	meterH.begin(METER_MID_PIN);
	meterM.begin(METER_LEFT_PIN);
	meterS.begin(METER_RIGHT_PIN);
	shitMeters[SECS] = meterS;
	shitMeters[MINS] = meterM;

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
	/*while(true) {
		meterH.move(255);
		Voltmeter::moveMultipleDamped(shitMeters, 2, 255);
		delay(1000);
		meterH.move(0);
		Voltmeter::moveMultipleDamped(shitMeters, 2, 0);
		delay(1000);
	}*/

	// Test LEDs
	leds.introAnimation();
	//leds.chargeNeutrinos(NULL);

	// Attach timer2 interrupt for animation frames (100Hz)
	set_animation(&leds, -1);
	cli(); // Globally disable interrupts
	TCCR2A = 0; // set entire TCCR2A register to 0
	TCCR2B = 0; // same for TCCR2B
	TCNT2  = 0; //initialize counter value to 0
	OCR2A = 155; // set compare match register for 100Hz increments
	TCCR2A |= (1 << WGM21); // turn on CTC mode
	TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20); // Set bits for 1024 prescaler
	TIMSK2 |= (1 << OCIE2A); // enable timer compare interrupt
	sei(); // Globally enable interrupts

	// Set LED mode from EEPROM
	uint8_t ledMode = EEPROM.read(EEPROM_LEDMODE);
	leds.setMode(ledMode);

	/*for(int i = ANIM_SMALL_START; i < ANIM_SMALL_START+ANIM_SMALL_NUM; i++) {
		test_animation(&leds, i);
		delay(500);
		test_animation(&leds, i);
		delay(1000);
	}*/

	//test_animation(&leds, 13);
	//delay(500);
	//test_animation(&leds, 5);
	//delay(500);	

	// Get the current time from RTC
	time_t tz = RTC.get(); 
	curTime[HOUR] = hour(tz);
	curTime[MINS] = minute(tz);
	curTime[SECS] = second(tz);

	long seed = second(tz) + 10 * minute(tz) + 100 * hour(tz) + 1000 * day(tz) + 10000 * month(tz) + 100000 * year(tz);
	randomSeed(seed);

	// Set the voltmeters
	meterPositions[SECS] = meterSoffset[curTime[SECS]];
	meterPositions[MINS] = meterMoffset[curTime[MINS]];
	meterPositions[HOUR] = meterHoffset[makeMeterTime(curTime[HOUR])];
	meterH.move(meterPositions[HOUR]);
	Voltmeter::moveMultipleDamped(shitMeters, 2, meterPositions);

	// Set the RTC interrupt callback
	pinMode(2, INPUT_PULLUP);
	attachInterrupt(0, rtcTick, RISING);

	// Enable the 1Hz oscillator
	if(!RTC.setTickMode(true))
		leds.errorAnimation();
}


void loop() {
	// Update Voltmeters here so we can use delays to debounce
	if (metersToUpdate > 0 && !stopWorld) {
		if (metersToUpdate == 3) {
			meterH.move(meterPositions[HOUR]);
			metersToUpdate = 2;
		}
		Voltmeter::moveMultipleDamped(shitMeters, metersToUpdate, meterPositions);
		metersToUpdate = 0;
	}

	if(programmerLoopSkip != 0)
		return;

	programmerLoopSkip = PROGRAMMER_SKIP_NUM;

	stopWorld = programmer.worldStop();

	if (!gotProgrammer && programmer.exists())
	{
		programmer.setLedMode(leds.getMode());
		programmer.setTime(curTime[HOUR], curTime[MINS], curTime[SECS]);
		//delay(1000);
		gotProgrammer = true;
	}
	else if (gotProgrammer)
	{
		uint8_t nextLed = 0;

		if (stopWorld && programmer.getTime(&curTime[HOUR], &curTime[MINS], &curTime[SECS]))
		{
			tmElements_t tz;
			RTC.read(tz);
			tz.Hour = curTime[HOUR];
			tz.Minute = curTime[MINS];
			tz.Second = curTime[SECS];
			RTC.write(tz);

			meterPositions[SECS] = meterSoffset[curTime[SECS]];
			meterPositions[MINS] = meterMoffset[curTime[MINS]];
			meterPositions[HOUR] = meterHoffset[makeMeterTime(curTime[HOUR])];
			meterH.move(meterPositions[HOUR]);
			Voltmeter::moveMultipleDamped(shitMeters, 2, meterPositions);
			metersToUpdate = 0;
		}
		else if (programmer.getLedMode(&nextLed)) // Only need to do this if the world isn't stopped, hence there's a chance the user is on the LED page.
		{
			if (nextLed != leds.getMode()) {
				leds.setMode(nextLed);
				EEPROM.write(EEPROM_LEDMODE, nextLed);
			}
		}
		else
		{
			gotProgrammer = false;
		}
	}
}
