#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
 

#define __DEBUG 	//¿ªÆô´®¿Úµ÷ÊÔ

#ifdef __DEBUG

#define DEBUG(format,...) printf("\nFILE: "__FILE__", LINE: %d: "format"\n", __LINE__, ##__VA_ARGS__)

#else

#define DEBUG(format,...)

#endif

void Timerx_Init(u16 arr,u16 psc); 
 
#endif
