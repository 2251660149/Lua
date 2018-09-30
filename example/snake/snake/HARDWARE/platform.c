/*
 * File: platform.c 
 * Change Logs:
 * Date           Author       Notes
 * 2013-01-02     dzcn      first version
 */
#include "platform.h"
#include "sys.h"
#include "delay.h"
static u8  fac_us=0;							//us��ʱ������			   
static u16 fac_ms=0;							//ms��ʱ������,��ucos��,����ÿ�����ĵ�ms��
unsigned int platform_time_delay;
RCC_ClocksTypeDef RCC_Clocks;
void platform_sys_tick_init(void)
{ 
 SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//ѡ���ⲿʱ��  HCLK/8
	fac_us=SystemCoreClock/8000000;				//Ϊϵͳʱ�ӵ�1/8  

	fac_ms=(u16)fac_us*1000;					//��OS��,����ÿ��ms��Ҫ��systickʱ����   
}
void platform_delay_ms(unsigned int dly)
{
u32 temp;		   
	SysTick->LOAD=(u32)dly*fac_ms;				//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;							//��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//��ʼ����  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
	SysTick->VAL =0X00;       					//��ռ�����	  	 
}
void platform_init(void)
{
	delay_init();
	platform_dios_init();
	//platform_sys_tick_init();
  //SD_Init();
}
