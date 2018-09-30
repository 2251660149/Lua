#ifndef __PRO_LOGIC_H
#define __PRO_LOGIC_H

#include "stm32f10x.h"
#include "stdio.h"
#include "sys.h"

#include "bsp_74h595.h"
#include "bsp_74h165.h"


/*
 *开关量输出模块输入参数结构体
 */
typedef struct
{
	uint16_t  com_addr[100];       /*!<   命令首地址*/
	
  uint16_t  com_count_num;           /*!<   命令条数 */
	
}LOGIC_PLC;
/*
 *开关量输出模块输入参数结构体
 */
typedef struct
{
	uint16_t  com_addr[100];       /*!<   命令首地址*/
	
  uint16_t  com_count_num;           /*!<   命令条数 */
	
}COMMAND_PLC;

/*
 *开关量输出模块输入参数结构体
 */
typedef struct
{
	uint8_t  IO_Channel_status;       /*!<   模块输出状态输入 1 输出高 0 输出低*/
	
  uint8_t  IO_Channel_En;           /*!<   模块工作使能 1 使能 0 关闭 */
	
}SOFT_O_Module_Input;

/*
 *开关量输入模块输入参数结构体
 */
typedef struct
{
  uint8_t  IO_Channel_En;           /*!<   模块工作使能 1 使能 0 关闭 */
	
}SOFT_I_Module_Input;

/*
 *开关量模块输出参数结构体
 */
typedef struct
{
	uint8_t  IO_Channel_status;       /*!<   模块输出状态 1 高电平 0 低电平*/
	
  uint8_t  SOFT_IO_Module_status;   /*!<   模块工作状态 1 工作正常 0 模块关闭*/          
     
}SOFT_IO_Module_Output;

typedef struct
{
  uint8_t GPIO_Pin_in;             

  uint8_t GPIO_Pin_out;            

  uint8_t GPIO_Pin_adc;      
	
	uint8_t GPIO_Pin_dac;
	
}LOGIC_Parameter;




SOFT_IO_Module_Output IO_Output_Manage(uint8_t  IO_Channel_Sel,SOFT_O_Module_Input value);
SOFT_IO_Module_Output IO_Input_Manage(uint8_t IO_Channel_Sel,SOFT_I_Module_Input value);

uint8_t OR_LogicModule_Manage(uint8_t input1,uint8_t input2);
uint8_t AND_LogicModule_Manage(uint8_t input1,uint8_t input2);
uint8_t NOT_LogicModule_Manage(uint8_t input1);

extern SOFT_O_Module_Input iomanage;

#endif




