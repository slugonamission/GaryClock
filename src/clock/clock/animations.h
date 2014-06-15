#ifndef __ANIMATIONS_H
#define __ANIMATIONS_H

#include "Leds.h"

//Tick the current animation one frame
void animation_tick(Leds *leds);

//Set the current animation to play. num should be less than 
//that returned by get_num_animations()
void set_animation(int num);

//Get the total number of defined animations
int get_num_animations();


#endif
