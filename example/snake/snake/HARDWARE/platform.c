/*
 * File: platform.c 
 * Change Logs:
 * Date           Author       Notes
 * 2013-01-02     dzcn      first version
 */
#include "platform.h"
#include "sys.h"
#include "delay.h"
static u8  fac_us=0;							//us延时倍乘数			   
static u16 fac_ms=0;							//ms延时倍乘数,在ucos下,代表每个节拍的ms数
unsigned int platform_time_delay;
RCC_ClocksTypeDef RCC_Clocks;
void platform_sys_tick_init(void)
{ 
 SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//选择外部时钟  HCLK/8
	fac_us=SystemCoreClock/8000000;				//为系统时钟的1/8  

	fac_ms=(u16)fac_us*1000;					//非OS下,代表每个ms需要的systick时钟数   
}
void platform_delay_ms(unsigned int dly)
{
u32 temp;		   
	SysTick->LOAD=(u32)dly*fac_ms;				//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;							//清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//开始倒数  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
	SysTick->VAL =0X00;       					//清空计数器	  	 
}
void platform_init(void)
{
	delay_init();
	platform_dios_init();
	//platform_sys_tick_init();
  //SD_Init();
}
