/*
 * File: platform_dio.c 
 * Change Logs:
 * Date           Author       Notes
 * 2013-01-04     dzcn      first version
 */

#include "platform_dio.h"
static GPIO_TypeDef * const dio_port[22] = {GPIOB, GPIOB, GPIOE, GPIOE, GPIOA, GPIOB, GPIOB, GPIOD, GPIOD, GPIOC,
																					GPIOC, GPIOA, GPIOE, GPIOE, GPIOC, GPIOE, GPIOB, GPIOB, GPIOA, GPIOA,
																					GPIOB, GPIOB};
static uint16_t dio_pin[22]={GPIO_Pin_5,GPIO_Pin_5,GPIO_Pin_5,GPIO_Pin_5,GPIO_Pin_1,GPIO_Pin_0,GPIO_Pin_1,GPIO_Pin_12,GPIO_Pin_13,GPIO_Pin_6,
													GPIO_Pin_7,GPIO_Pin_8,GPIO_Pin_4,GPIO_Pin_5,GPIO_Pin_13,GPIO_Pin_6,GPIO_Pin_10,GPIO_Pin_11,GPIO_Pin_2,GPIO_Pin_3,
													GPIO_Pin_9,GPIO_Pin_8};
void platform_dios_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);

  GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_8;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_1|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_13;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12|GPIO_Pin_13;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}
#include "led.h"
int platform_dio_op(char dio_num,int op)
{
	int res=0;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	LED0=!LED0;	
	switch(op)
	{
		case	PLATFORM_DIO_CLR:
			GPIO_ResetBits(dio_port[dio_num],dio_pin[dio_num]);
			break;
		case	PLATFORM_DIO_SET:
			GPIO_SetBits(dio_port[dio_num],dio_pin[dio_num]);
			break;
		case	PLATFORM_DIO_GET:
			res=GPIO_ReadInputDataBit(dio_port[dio_num],dio_pin[dio_num]);
			break;
		case PLATFORM_DIO_IN:
			GPIO_StructInit(&GPIO_InitStructure);
			GPIO_InitStructure.GPIO_Pin=dio_pin[dio_num];
			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
			GPIO_Init(dio_port[dio_num],&GPIO_InitStructure);
			break;
		case PLATFORM_DIO_OUT:
			GPIO_StructInit(&GPIO_InitStructure);
			GPIO_InitStructure.GPIO_Pin=dio_pin[dio_num];
			GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_OD;
			GPIO_Init(dio_port[dio_num],&GPIO_InitStructure);
			break;
		default:res=-1;
	}
	return res;
}
