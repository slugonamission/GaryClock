#include <Wire.h>
#include "DS1307RTC.h"
#include "SevenSeg.h"
#include "Voltmeter.h"
#include "Leds.h"


#define SEGL_ADDR 0x70
#define SEGM_ADDR 0x71
#define SEGR_ADDR 0x72

#define BTN_PIN 3
  
#define METERL_PIN 11
#define METERM_PIN 10
#define METERR_PIN 9

//#define LEDS_PIN 6 // (actually defined in Leds.h)


int allHands[] = {METERL_PIN, METERM_PIN, METERR_PIN};

// 7-segment displays (left, middle, right)
SevenSeg segL = SevenSeg();
SevenSeg segM = SevenSeg();
SevenSeg segR = SevenSeg();

Voltmeter meterL = Voltmeter();
Voltmeter meterM = Voltmeter();
Voltmeter meterR = Voltmeter();

Leds leds = Leds();
