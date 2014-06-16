#ifndef __ANIMATIONS_H
#define __ANIMATIONS_H

#include "Leds.h"

//Tick the current animation one frame. Returns true if the animation is still running
//or false if it is complete.
boolean animation_tick(Leds *leds);

//Set the current animation to play. num should be less than 
//that returned by get_num_animations()
void set_animation(Leds *leds, int num);

//Get the total number of defined animations
int get_num_animations();

//Test an animation to completion
void test_animation(Leds *leds, int anim);

#endif
