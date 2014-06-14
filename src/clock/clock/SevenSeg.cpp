#include "SevenSeg.h"


SevenSeg::SevenSeg(void) : Adafruit_7segment() {

}

void SevenSeg::clearDisplay(void) {
	writeDigitRaw(0, 0);
	writeDigitRaw(1, 0);
	writeDigitRaw(2, 0);
	writeDigitRaw(3, 0);
	writeDigitRaw(4, 0);
	writeDisplay();
}


