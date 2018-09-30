#ifndef __CD4066_H
#define __CD4066_H
#include "stm32f10x.h"

typedef enum
{ BLUETOOTH_INPUT = 0,
  AUX_INPUT
}VoiceCH;

void CD4066_Init(void);
uint8_t CD4066_Choose(VoiceCH i); 
void CD4066_Read(void);
#endif
