#include "Arduino.h"
#include "Adafruit_LEDBackpack.h"


class SevenSeg : public Adafruit_7segment {
	public:
		SevenSeg(void);
		void clearDisplay(void);

	private:

};
