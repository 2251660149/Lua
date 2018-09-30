/**
  ******************************************************************************
  * @file    bsp_74h595.c
  * @author  Jack Lu
  * @version V1.0
  * @date    2017-12-28
  * @brief   
  ******************************************************************************
	*/
	
#include "bsp_74h595.h"
#include "delay.h"

/**********************************************************
 * 函数名 : H595_Init
 * 输入   ：none
 * 输出   ：none 
 * 功能   ：74hc595初始化
 **********************************************************/
void H595_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_HC595_CLK_PORT | RCC_HC595_DATA_PORT | RCC_HC595_RCLK_PORT | RCC_HC595_OE_PORT,ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = HC595_CLK_PIN ;        //74HC595串行输入时钟引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(HC595_CLK_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = HC595_DATA_PIN ;       // 74HC595串行数据输入引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(HC595_DATA_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = HC595_RCLK_PIN ;       // 74HC595输出存储器锁存时钟线
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(HC595_RCLK_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = HC595_OE_PIN ;         // 74HC595输出使能引脚
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(HC595_OE_PORT, &GPIO_InitStructure);
}

/**********************************************************
 * 函数名 : write_smg
 * 输入   ：temp 8位串行数据
 * 输出   ：none 
 * 功能   ：串行数据写入
 **********************************************************/
void H595_WriteData(u8 *value)
{
	u8 i,j;
	u8 temp[4]={0,};
	memcpy(temp,value,4);	
	HC595_OE_L();
	for(j=0;j<4;j++)
	{
		for(i=0;i<8;i++)
		{
			HC595_CLK_L();
			if((temp[j]&0x80) == 0x80)						
			{
				HC595_DAT_H(); //串行数据输出
			}
			else
			{
				HC595_DAT_L();
			} 
			HC595_CLK_H(); //移位输入时钟，上升沿输入
			temp[j]<<=1;
			delay_us(3);
			HC595_CLK_L();
		}
	}
}
/**********************************************************
 * 函数名 : H595_OutputEn
 * 输入   ：temp 8位串行数据
 * 输出   ：none 
 * 功能   ：串行数据写入
 **********************************************************/
void H595_OutputEn(void)
{
	HC595_RCLK_L(); //并行输出时钟
	delay_us(3);	
	HC595_RCLK_H();
	delay_us(3);	
//	HC595_RCLK_L();	
}
/*******************************************************************************************/
