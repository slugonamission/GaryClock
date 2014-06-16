#ifndef __CLOCK_H__
#define __CLOCK_H__

#include "DS1307RTC.h"
#include "SevenSeg.h"
#include "Voltmeter.h"
#include "Leds.h"
#include "Programmer.h"


#define RTC_ADDR 0x68

#define PROG_ADDR 0x28

#define SEGL_ADDR 0x70
#define SEGM_ADDR 0x71
#define SEGR_ADDR 0x72

#define BTN_PIN 3
  
#define METER_LEFT_PIN  9
#define METER_MID_PIN   10
#define METER_RIGHT_PIN 11

//#define LEDS_PIN 6 // (actually defined in Leds.h)

#endif



