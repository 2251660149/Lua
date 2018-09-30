#ifndef __BSP_74H595_H__
#define __BSP_74H595_H__

#include "sys.h"

#define RCC_HC595_CLK_PORT                    RCC_APB2Periph_GPIOE
#define HC595_CLK_PIN                         GPIO_Pin_10
#define HC595_CLK_PORT                        GPIOE

#define RCC_HC595_DATA_PORT                   RCC_APB2Periph_GPIOE
#define HC595_DATA_PIN                        GPIO_Pin_8
#define HC595_DATA_PORT                       GPIOE

#define RCC_HC595_RCLK_PORT                   RCC_APB2Periph_GPIOE
#define HC595_RCLK_PIN                        GPIO_Pin_9
#define HC595_RCLK_PORT                       GPIOE

#define RCC_HC595_OE_PORT                     RCC_APB2Periph_GPIOE
#define HC595_OE_PIN                          GPIO_Pin_12
#define HC595_OE_PORT                         GPIOE

#define HC595_DAT_H() HC595_DATA_PORT->BSRR=HC595_DATA_PIN
#define HC595_DAT_L() HC595_DATA_PORT->BRR=HC595_DATA_PIN

#define HC595_OE_H()  HC595_OE_PORT->BSRR=HC595_OE_PIN 
#define HC595_OE_L()  HC595_OE_PORT->BRR=HC595_OE_PIN 

#define HC595_CLK_H() HC595_CLK_PORT->BSRR=HC595_CLK_PIN
#define HC595_CLK_L() HC595_CLK_PORT->BRR=HC595_CLK_PIN

#define HC595_RCLK_H() HC595_RCLK_PORT->BSRR=HC595_RCLK_PIN  
#define HC595_RCLK_L() HC595_RCLK_PORT->BRR=HC595_RCLK_PIN

void H595_Init(void);
void H595_WriteData(u8 *temp);
void H595_OutputEn(void);


#endif
