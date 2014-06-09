#ifndef __SEVENSEG_H__
#define __SEVENSEG_H__

#include "Arduino.h"
#include "Adafruit_LEDBackpack.h"


class SevenSeg : public Adafruit_7segment {
	public:
		SevenSeg(void);
		void clearDisplay(void);

	private:

};

#endif
