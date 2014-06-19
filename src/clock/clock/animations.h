#ifndef __ANIMATIONS_H
#define __ANIMATIONS_H

#include "Leds.h"

//Tick the current animation one frame. Returns true if the animation is still running
//or false if it is complete.
boolean animation_tick(Leds *leds);

//Set the current animation to play. num should be less than 
//that returned by get_num_animations()
//If -1 (or any other invalid ID) then animations are turned off.
void set_animation(Leds *leds, int num, int delay = 0);

//Test an animation to completion
void test_animation(Leds *leds, int anim);

//IDs for certain special animations

#define ANIMS_NUM 17
#define ANIM_SMALL_START 3
#define ANIM_SMALL_NUM 14

#define ANIM_ID_COLOUR 0
#define ANIM_ID_BATSHIT 1
#define ANIM_ID_PULSE 2

#endif
