#ifndef __PLATFORM_DIO_H
#define __PALTFORM_DIO_H
#include "stm32f10x.h"
#define NUM_DIO         22
#define PLATFORM_DIO_CLR 				0
#define PLATFORM_DIO_SET				1
#define PLATFORM_DIO_GET				2
#define PLATFORM_DIO_IN			3
#define PLATFORM_DIO_OUT		4
extern void platform_dios_init(void);
extern int platform_dio_op(char dio_num,int op);
#endif
