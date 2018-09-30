/**********************************************************									 
功能按键程序
时间：2017/06/08
版本：1.0
作者：jack lu
**********************************************************/
#include "bap_key.h"
#include "delay.h"

u8 key_data[4][4]={{1,2,3,4},{5,6,7,8},{9,10,11,12},{13,14,15,16}};//键值
__IO u8 Keyboard_Val =0;

/**********************************************************
 * 函数名 : key_init
 * 输入   ：none
 * 输出   ：none 
 * 功能   ：按键初始化
 **********************************************************/
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE); // IO口时钟使能
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_10 | GPIO_Pin_11;  //初始化具体IO口
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //IO 口速率
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //IO 工作模式 推挽输出 扫描按键
	GPIO_Init(GPIOB, &GPIO_InitStructure);             //初始化GPIOB
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;          //初始化具体IO口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;      //IO 工作模式 下拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);             //初始化GPIOA
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;          //初始化具体IO口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;      //IO 工作模式 下拉输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);             //初始化GPIOB
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;          //初始化具体IO口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;      //IO 工作模式 下拉输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);             //初始化GPIOC
}
/**********************************************************
 * 函数名 : KEY_Interrupt
 * 输入   ：none
 * 输出   ：none 
 * 功能   ：LED接口初始化
 **********************************************************/
void KEY_Interrupt(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟(使用中断和引脚映射时要使能复用功能时钟)

	KEY_Init();//初始化按键IO

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0);
	EXTI_InitStructure.EXTI_Line= EXTI_Line0;	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource5);
	EXTI_InitStructure.EXTI_Line= EXTI_Line5;	
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource4);
	EXTI_InitStructure.EXTI_Line= EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	
	
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource4);
	EXTI_InitStructure.EXTI_Line= EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;//             		
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); 

	NVIC_InitStructure.NVIC_IRQChannel =EXTI9_5_IRQn;//             		
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); 	
	
	NVIC_InitStructure.NVIC_IRQChannel =  EXTI0_IRQn;//               		
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
	NVIC_Init(&NVIC_InitStructure);
	KEY_Control(0);
}
/**********************************************************
 * 函数名 : KEY_Control
 * 输入   ：扫描模式
 * 输出   ：none 
 * 功能   ：扫描按键控制程序
 **********************************************************/
void KEY_Control(u8 i)
{
	switch(i)
	{
		case 0:
		GPIO_SetBits(GPIOB,GPIO_Pin_11);
		GPIO_SetBits(GPIOB,GPIO_Pin_10);
		GPIO_SetBits(GPIOB,GPIO_Pin_2);
		GPIO_SetBits(GPIOB,GPIO_Pin_1);
		break;
		case 1:
		GPIO_SetBits(GPIOB,GPIO_Pin_11);
		GPIO_ResetBits(GPIOB,GPIO_Pin_10);
		GPIO_ResetBits(GPIOB,GPIO_Pin_1);
		GPIO_ResetBits(GPIOB,GPIO_Pin_2);
		break;
	  case 2:
		GPIO_SetBits(GPIOB,GPIO_Pin_10);
		GPIO_ResetBits(GPIOB,GPIO_Pin_11);
		GPIO_ResetBits(GPIOB,GPIO_Pin_1);
		GPIO_ResetBits(GPIOB,GPIO_Pin_2);
		break;
		case 3:
		GPIO_SetBits(GPIOB,GPIO_Pin_2);
		GPIO_ResetBits(GPIOB,GPIO_Pin_11);
		GPIO_ResetBits(GPIOB,GPIO_Pin_1);
		GPIO_ResetBits(GPIOB,GPIO_Pin_10);
    break;	
		case 4:
		GPIO_SetBits(GPIOB,GPIO_Pin_1);
		GPIO_ResetBits(GPIOB,GPIO_Pin_11);
		GPIO_ResetBits(GPIOB,GPIO_Pin_10);
		GPIO_ResetBits(GPIOB,GPIO_Pin_2);
    break;
		default:break;		
	}
}
//外部中断0服务程序 
void EXTI0_IRQHandler(void)
{
	u8 i=0;
	delay_ms(10);//消抖
	printf("进入中断0");
	for(i=1;i<4;i++)//启动按键扫描
	{
		KEY_Control(i);
		
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==1)//第一列扫描
		{
			Keyboard_Val=key_data[i-1][0];//返回对应键值
		}
	}
	KEY_Control(0);
	EXTI_ClearITPendingBit(EXTI_Line0); //清除LINE0上的中断标志位  
}
 
//外部中断2服务程序
void EXTI9_5_IRQHandler(void)
{
	u8 i=0;
	delay_ms(10);//消抖
	printf("进入中断5");
	for(i=1;i<4;i++)
	{
		KEY_Control(i);
		
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)==1)//第二列扫描
		{
			Keyboard_Val=key_data[i-1][1];
		}
	}
	KEY_Control(0);
	EXTI_ClearITPendingBit(EXTI_Line5);  //清除LINE5上的中断标志位  
}

void EXTI4_IRQHandler(void)
{
	u8 i=0;
	delay_ms(10);//消抖
	printf("进入中断4");
	for(i=1;i<4;i++)
	{
		KEY_Control(i);
		
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)==1)//第三列扫描
		{
			Keyboard_Val=key_data[i-1][2];
		}
		else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==1)//第四列扫描
		{
			Keyboard_Val=key_data[i-1][3];
		}
	}	
	KEY_Control(0);//等待按键按下
	EXTI_ClearITPendingBit(EXTI_Line4);  //清除LINE4上的中断标志位  
}

/**********************************************************
 * 函数名 : KEY_Read
 * 输入   ：none
 * 输出   ：none 
 * 功能   ：按键处理程序
 **********************************************************/
uint8_t KEY_Read(void)
{
	u8 i=0;
	static u8 key_stay_flag=0;
	u8 Keyboard_Val =0;
	
	for(i=1;i<4;i++)
	{
		KEY_Control(i);
		
		if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==1&&(key_stay_flag==0))
		{
			key_stay_flag=1;
			Keyboard_Val=key_data[i][0];
		}
		else if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)==1&&(key_stay_flag==0))
		{
			key_stay_flag=1;
			Keyboard_Val=key_data[i][1];
		}
		else if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)==1&&(key_stay_flag==0))
		{
			key_stay_flag=1;
			Keyboard_Val=key_data[i][2];
		}
		else if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)==1&&(key_stay_flag==0))
		{
			key_stay_flag=1;
			Keyboard_Val=key_data[i][3];
		}
		key_stay_flag=0;
  }
	return Keyboard_Val;
}


