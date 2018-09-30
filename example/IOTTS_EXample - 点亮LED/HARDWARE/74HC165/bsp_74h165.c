/**
  ******************************************************************************
  * @file    bsp_74h165.c
  * @author  Jack Lu
  * @version V1.0
  * @date    2017-12-28
  * @brief   
  ******************************************************************************
	*/
	
#include "bsp_74h165.h"
#include "delay.h"

/**********************************************************
 * 函数名 : H165_Init
 * 输入   ：none
 * 输出   ：none 
 * 功能   ：74hc165初始化
 **********************************************************/
void H165_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_HC165_CP_PORT | RCC_HC165_DATA_PORT | RCC_HC165_PL_PORT | RCC_HC165_CE_PORT,ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = HC165_CP_PIN ;         //74HC165串行输入时钟引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(HC165_CP_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = HC165_DATA_PIN ;       // 74HC165串行数据输出引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(HC165_DATA_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = HC165_PL_PIN ;         // 74HC165并行输入使能
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(HC165_PL_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = HC165_CE_PIN ;         // 74HC165串行输出使能引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(HC165_CE_PORT, &GPIO_InitStructure);
}

/**********************************************************
 * 函数名 : H165_ReadData
 * 输入   ：data_counter 需要读取数据通道
 * 输出   ：8位串行数据 
 * 功能   ：读取74hc165串行输出数据
 **********************************************************/
void H165_ReadData(u8 num,u8 *value)
{
	u8 i,j;
	u8 hc165_data=0;
	
	HC165_CE_L();
  HC165_PL_H();
	for(j=0;j<num;j++)
	{
		for(i=0;i<8;i++)
		{
			hc165_data<<=1;
			if(HC165_DAT == 0x100)			
				hc165_data+=1;
			HC165_CP_H(); //移位输入时钟，上升沿输入
			delay_us(10);
			HC165_CP_L();
			delay_us(10);
		}
		*value=hc165_data;
		value++;
	}
	HC165_PL_L();
}

/*****************************************************************************************************************/
