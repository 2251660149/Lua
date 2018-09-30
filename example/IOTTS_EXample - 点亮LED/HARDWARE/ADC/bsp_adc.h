/**
  ******************************************************************************
  * @file    bsp_adc.h
  * @author  Jack Lu
  * @version V1.0
  * @date    2017-12-28
  * @brief   ADC相关函数头文件
  ******************************************************************************
	*/

#ifndef __BSP_ADC_H_
#define __BSP_ADC_H_

/**********************************************************
                     外部函数头文件
         应用到不同的外设头文件请在这里修改即可                        
**********************************************************/

#include "sys.h"

#define ADC1_DR_Address    ((u32)0x40012400+0x4c)

/*
 * ADC输入参数结构体
 */
typedef struct
{
  uint8_t  ADC_ChannelInput_En;      /*!<   输入通道使能 */ 
   
	uint16_t  ADC_H_lim;                /*!<   输入通道上限值 上限为0表示不开启上下限控制 */

	uint16_t  ADC_L_lim;                /*!<   输入通道下限值 */
	
}SOFT_Adc_Module_Input;//模块输入参数结构体

/*
 * ADC输出参数结构体
 */
typedef struct
{
	uint8_t  ADC_Channel_Module_Status;    /*!<   模块工作状态 */
	
  uint16_t  ADC_Channel_Data;            /*!<   ADC实际数值  */    
   
  signed char  ADC_Out_Status;           /*!<   阈值状态输出 */
	
}SOFT_Adc_Module_Output;

typedef enum {
		ADC_0=0,
    ADC_1,
    ADC_2,
    ADC_3,
    ADC_4,
    ADC_5,
		ADC_6,
    ADC_7,
}ADC_Channel_Sel;/*!<   输入通道选择 */
/**********************************************************
                       外部功能函数                      
**********************************************************/
void BSP_ADC_Init(void);
uint16_t BSP_GetAdcValue(void);
SOFT_Adc_Module_Output ADC_Manage(ADC_Channel_Sel ADC_In_Sel,SOFT_Adc_Module_Input value);
#endif

