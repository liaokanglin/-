#ifndef __KEY_H__
#define __KEY_H__

#include "config.h"
#include "GPIO.h"

#define KEY		P05
#define GPIO_KEY	GPIO_P0, GPIO_Pin_5

void Key_init();

void Key_scan();

extern void Key_on_keyup();
extern void Key_on_keydown();


#endif