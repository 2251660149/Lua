#ifndef __BSP_TIMER_H
#define __BSP_TIMER_H

#include "stm32f10x.h"
#include "stdio.h"
#include "math.h"
//如果index对应的时间超时，清零并返回1，未超时返回0
#define IS_TIMEOUT_1MS(index, count)    ((g_Tim6Array[(u16)(index)] >= (count))? ((g_Tim6Array[(u16)(index)] = 0) == 0): 0) 
                                        
//定义软件定时器个数
#define SOFT_TIMERS_MAX 8  

/*PWM输入模块输入参数结构体*/
typedef struct
{
	uint8_t  PWM_InputChannel_En;      /*!<   输出通道使能 */ 
	
}SOFT_Module_PwmIn_Input;

/*
 * PWM输入模块输出参数结构体
 */
typedef struct
{
	uint8_t  SOFT_Module_PwmIn_Module_Status;     /*!<   软件模块工作状态 */
	
  float  PWM_InputChannel_Fre;               /*!<   频率 */ 	
  
	float  PWM_InputChannel_Duty_Cycle;        /*!<   PWM占空比 */
		
}SOFT_Module_PwmIn_Output;

/*  
 * PWM输出模块输入参数结构体
 */
typedef struct
{
  uint16_t  PWM_OutputChannelt_Fre;            /*!< 输出频率 */
  
	uint16_t  PWM_OutputCounter;
	
	uint16_t  PWM_OutputNumCounter;
	
	float  PWM_OutputChannel_Duty_Cycle;     /*!< PWM占空比 */ 	
	
	uint8_t  PWM_OutputChannel_En;             /*!< PWM输出模块使能 */ 	
    
}SOFT_Module_PwmOut_Input;

/*
 * PWM输出模块输出参数结构体
 */
typedef struct
{
	uint16_t  PWM_Count; /*!< 计数 */
	uint8_t  SOFT_Module_PwmOut_Module_Status; /*!< 软件模块工作状态 */
		
}SOFT_Module_PwmOut_Output;

/*
 * 计数器模块输入参数结构体
 */
typedef struct
{
	uint8_t  TIM_Channel_Sel;    /*!< 计数器选择 */
	
  uint8_t  TIM_Channel_En;     /*!< 计数器使能 */

  uint8_t  TIM_Channel_Trig;   /*!< 计数器触发 */

  uint16_t TIM_Counter;        /*!< 计数器计数值 */   

  uint16_t TIM_Cycle_Times;    /*!< 计数器重装载次数 0 不重载*/
	
}SOFT_Module_Tim_Input;

/*
 * 计数器模块输出参数结构体
 */
typedef struct
{
  uint16_t  TIM_Counter;       /*!< 计数器计数值显示输出 */     

  uint8_t TIM_Out_status;      /*!< 计数器计输出状态  */     

  uint8_t SOFT_Module_TIM_Module_Status;   /*!< 计数器模块工作状态  */    
	
}SOFT_Module_Tim_Output;

enum {
		Tim6_0,
    Tim6_1,
    Tim6_2,
    Tim6_3,
    Tim6_4,
    Tim6_5,
		Tim6_6,
    Tim6_7,
};

typedef struct  {
	  const char * const timer_name;
    uint32_t time_counter;
    uint32_t time_out;                      //阀值
    void (*time_out_callback)(uint8_t TimerID); //回调函数
    uint8_t TimerID;                     //定时器属性标志
}soft_pwm_t;

/*
 * 
 */
typedef struct soft_timer {
	  const char * const timer_name;
    uint32_t time_counter;
    uint32_t time_out;                      //阀值
    void (*time_out_callback)(uint8_t TimerID); //回调函数
    uint8_t TimerID;                     //定时器属性标志
}soft_timer_t;


typedef void (*time_out_callback)(uint8_t TimerID);
typedef void * TimerHandle_t;

void Timerx_Init(u16 arr,u16 psc); 
void TIM5_PWM_Init(u16 arr,u16 psc);
void TIM4_PWM_Init(u16 arr,u16 psc);
void TIMx_Cap_Init(TIM_TypeDef* TIMx,u16 arr,u16 psc);
void TIM6_Config(u16 arr,u16 psc);
TimerHandle_t soft_timer_registered(const char * const timer_name,
                                 const uint32_t period, 
																 const uint32_t AutoReload,   
                                 uint8_t TimerID ,
                                 time_out_callback callback);
void soft_timer_exec(void);
void callbackfun(uint8_t TimerID);
SOFT_Module_PwmIn_Output PWM_InManage(u8 PWM_InputChannel_Sel,SOFT_Module_PwmIn_Input value);																 
SOFT_Module_PwmOut_Output PWM_OutManage(uint8_t  PWM_OutputChannel_Sel,SOFT_Module_PwmOut_Input value,u8 ret_data);
void PWM_Out_Init(void);
void PWM_init(void);																 	    				
																	 
extern volatile int g_Tim6Array[(u16)Tim6_7];
#endif
