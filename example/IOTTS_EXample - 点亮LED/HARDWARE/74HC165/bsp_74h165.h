#ifndef __BSP_74H165_H__
#define __BSP_74H165_H__

#include "stm32f10x.h"

#define RCC_HC165_CP_PORT                     RCC_APB2Periph_GPIOD
#define HC165_CP_PIN                          GPIO_Pin_9
#define HC165_CP_PORT                         GPIOD

#define RCC_HC165_DATA_PORT                   RCC_APB2Periph_GPIOD
#define HC165_DATA_PIN                        GPIO_Pin_8
#define HC165_DATA_PORT                       GPIOD

#define RCC_HC165_PL_PORT                     RCC_APB2Periph_GPIOD
#define HC165_PL_PIN                          GPIO_Pin_10
#define HC165_PL_PORT                         GPIOD

#define RCC_HC165_CE_PORT                     RCC_APB2Periph_GPIOD
#define HC165_CE_PIN                          GPIO_Pin_11
#define HC165_CE_PORT                         GPIOD

#define HC165_DAT     (HC165_DATA_PORT->IDR&HC165_DATA_PIN)

#define HC165_CE_H()  HC165_CE_PORT->BSRR=HC165_CE_PIN 
#define HC165_CE_L()  HC165_CE_PORT->BRR=HC165_CE_PIN 

#define HC165_CP_H()  HC165_CP_PORT->BSRR=HC165_CP_PIN
#define HC165_CP_L()  HC165_CP_PORT->BRR=HC165_CP_PIN

#define HC165_PL_H()  HC165_PL_PORT->BSRR=HC165_PL_PIN  
#define HC165_PL_L()  HC165_PL_PORT->BRR=HC165_PL_PIN

void H165_Init(void);
//uint8_t read_smg(void);
void H165_ReadData(u8 num,u8 *value);
#endif
