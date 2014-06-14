#ifndef __ANIMATIONS_H
#define __ANIMATIONS_H

#include "Leds.h"

class Animation {
public:
	Animation(Leds *ledsToUse);
	virtual ~Animation();
	/*
	* Called by the main loop to advance to the next tick of the animation. 
	* This function should return true if there is more animation to come.
	* False if the animation has completed.
	*/
	virtual boolean tick() = 0;
protected:
	Leds *leds;
	unsigned int frame;
};

//------------------------------------------------------------------------------------------

class RandomTwinkle : public Animation {
public:
	RandomTwinkle(Leds *leds);
	boolean tick();
};

class HueCycle : public Animation {
public:
	HueCycle(Leds *leds);
	boolean tick();
};

class QuickSweep : public Animation {
public:
	QuickSweep(Leds *leds);
	boolean tick();
private:
	int tickCount;
	int x;
};



#endif
