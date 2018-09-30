
#include "bsp_led.h"

/**********************************************************
 * 函数名 : LED_Init
 * 输入   ：none
 * 输出   ：none 
 * 功能   ：LED接口初始化
 **********************************************************/
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	//开启GPIOC外设时钟	
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_3 | GPIO_Pin_0| GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_SetBits(GPIOE ,GPIO_Pin_3);
	PWR_BackupAccessCmd(ENABLE);//允许修改RTC和后备寄存器
  RCC_LSEConfig(RCC_LSE_OFF); //关闭外部低速时钟信号
	BKP_TamperPinCmd(DISABLE);  //关闭入侵检测功能
	BKP_ITConfig(DISABLE);      //备份区写保护
}

