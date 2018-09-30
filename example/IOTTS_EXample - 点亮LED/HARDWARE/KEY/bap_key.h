#ifndef __key_H
#define __key_H
#include "stm32f10x.h"

extern __IO u8 Keyboard_Val;

void KEY_Init(void);
uint8_t KEY_Read(void);
void KEY_Interrupt(void);
void KEY_Control(u8 i);
#endif
