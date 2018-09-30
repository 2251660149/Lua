/**
  ******************************************************************************
  * @file    bsp_74h595.c
  * @author  Jack Lu
  * @version V1.0
  * @date    2017-12-28
  * @brief   
  ******************************************************************************
	*/

#include "pro_logic.h"
SOFT_O_Module_Input iomanage;
/**********************************************************
 * 函数名 : OR_LogicModule_Manage
 * 输入   ：input1 信号1输入  input2 信号2输入
 * 输出   ：运算结果 
 * 功能   ：逻辑或运算
 **********************************************************/
uint8_t OR_LogicModule_Manage(uint8_t input1,uint8_t input2)
{	
	return input1||input2;
}

/**********************************************************
 * 函数名 : AND_LogicModule_Manage
 * 输入   ：input1 信号1输入  input2 信号2输入
 * 输出   ：运算结果 
 * 功能   ：逻辑与运算
 **********************************************************/
uint8_t AND_LogicModule_Manage(uint8_t input1,uint8_t input2)
{	
	return input1&&input2;
}

/**********************************************************
 * 函数名 : NOT_LogicModule_Manage
 * 输入   ：input1 信号1输入
 * 输出   ：运算结果 
 * 功能   ：逻辑非运算
 **********************************************************/
uint8_t NOT_LogicModule_Manage(uint8_t input1)
{	
	return !input1;
}

/**********************************************************
 * 函数名 : IO_Output_Manage
 * 输入   ：none
 * 输出   ：none 
 * 功能   ：开关量输出模块
 **********************************************************/

SOFT_IO_Module_Output IO_Output_Manage(uint8_t IO_Channel_Sel,SOFT_O_Module_Input value)
{
	static u8 hc595_test[4]={0,0,0,0};
	u8 io_temp=0x01;
	SOFT_IO_Module_Output temp;
		
	if(value.IO_Channel_En==1)
	{
		io_temp<<=(IO_Channel_Sel-IO_Channel_Sel/8*8);//选择对应通道
		if(value.IO_Channel_status==1)hc595_test[IO_Channel_Sel/8]|=io_temp;
		else hc595_test[IO_Channel_Sel/8]&=~io_temp;
		H595_WriteData(hc595_test);//
		H595_OutputEn();
	}
	temp.SOFT_IO_Module_status=value.IO_Channel_En;
	temp.IO_Channel_status=0;
	return temp;
}

/**********************************************************
 * 函数名 : IO_Manage
 * 输入   ：none
 * 输出   ：none 
 * 功能   ：开关量输出模块
 **********************************************************/
SOFT_IO_Module_Output IO_Input_Manage(uint8_t IO_Channel_Sel,SOFT_I_Module_Input value)
{
	static u8 hc165_data[6];
	u8 io_temp=0x01;
	
	SOFT_IO_Module_Output temp;
	
	H165_ReadData(6,hc165_data);
	if(value.IO_Channel_En==1)
	{
		io_temp<<=(IO_Channel_Sel-IO_Channel_Sel/8*8);
		io_temp=~io_temp;	
		if(((hc165_data[IO_Channel_Sel/8])|io_temp)==io_temp)//判断输入是否为高
		{
			temp.IO_Channel_status=1;
		}
		else 
		{
			temp.IO_Channel_status=0;
		}
	}
	else temp.IO_Channel_status=0;//关闭软件模块输出0
	temp.SOFT_IO_Module_status=value.IO_Channel_En;
	
	return temp;
}



