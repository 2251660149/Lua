
/**********************************************************									 
功能：数字电位器控制程序
时间：2017/06/02
版本：1.0
作者：jack lu
**********************************************************/

#include "bsp_mcp41010.h" 
#include "delay.h"

/***************************************************************
 *函数名 ：MCP41010_Init
 *功能   ：MCP41010 IO口初始化
 *输入   ：无
 *输出   ：无
****************************************************************/
void MCP41010_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;            //定义IO口初始化数据的结构体
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); // IO口时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // IO口时钟使能
	
	GPIO_InitStructure.GPIO_Pin = MCP41010_CS2 | MCP41010_CS3 | MCP41010_CS4 | MCP41010_CS5;  //初始化具体IO口
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //IO 口速率
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //IO 工作模式 推挽输出
	GPIO_Init(GPIOC, &GPIO_InitStructure);             //初始化GPIOC
	
	GPIO_InitStructure.GPIO_Pin = MCP41010_CS1;        //初始化具体IO口
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //IO 口速率
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //IO 工作模式 推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);             //初始化GPIOA
	
	GPIO_InitStructure.GPIO_Pin = MCP41010_CLK;        //初始化MCP41010时钟引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //IO 口速率
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //IO 工作模式 推挽输出
	GPIO_Init(MCP41010_PORT_SCK, &GPIO_InitStructure); //初始化GPIOC
	
	GPIO_InitStructure.GPIO_Pin = MCP41010_SI;         //初始化MCP41010数据引脚
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //IO 口速率
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //IO 工作模式 推挽输出
	GPIO_Init(MCP41010_PORT_SI, &GPIO_InitStructure);  //初始化GPIOC

}

/*
*********************************************************************************************************
*	函 数 名: MCP41010_AmpSet
*	功能    : 改变输出信号幅度值
*	输入    : 1.amp ：幅度值  0- 255
*	输出    : 无
*********************************************************************************************************
*/ 

void MCP41010_AmpSet(uint8_t amp,uint8_t csx)
{
	uint8_t i;
	uint16_t temp;
	static uint8_t write_flag=0;
	
	if(write_flag==0)write_flag=1;//防止频繁调节导致误操作
	SCK_H();
	delay_us(1);
  switch(csx)
	{
		case modeCS1: {CS1_L();CS2_H();CS3_H();CS4_H();CS5_H();} break;
		case modeCS2: {CS1_H();CS2_L();CS3_H();CS4_H();CS5_H();} break;
		case modeCS3: {CS1_H();CS2_H();CS3_L();CS4_H();CS5_H();} break;
		case modeCS4: {CS1_H();CS2_H();CS3_H();CS4_L();CS5_H();} break;
		case modeCS5: {CS1_H();CS2_H();CS3_H();CS4_H();CS5_L();} break;
	  default:  break;
	}		
	temp =0x1100|amp;//0001 0001 0000 0000 对电位器0进行写数据操作
	for(i=0;i<16;i++)
	{
	   if(temp&0x8000)
	   	DAT_H();
	   else
			DAT_L();
		 temp<<=1;
		 delay_us(200);
		 SCK_L();
		 delay_us(200);
	   SCK_H();
	}
	delay_us(1);
	SCK_L();
	switch(csx)
	{
		case modeCS1: CS1_H(); break;
		case modeCS2: CS2_H(); break;
		case modeCS3: CS3_H(); break;
		case modeCS4: CS4_H(); break;
		case modeCS5: CS5_H(); break;
	  default:  break;
	}
	write_flag=0;
}

