#include "stm32f10x.h"
#include "key.h"
#include "sys.h" 
#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//按键驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////  
typedef struct _TIMER
{
	u16 DelayCnt;
	u16 sDelayCnt;
	u16 lDelayCnt;
} TIMER_TypeDef;


TIMER_TypeDef key0_Timer = {0,1,5};
TIMER_TypeDef key1_Timer    = {0,1,5};
TIMER_TypeDef key2_Timer  = {0,1,5};
TIMER_TypeDef wkup_Timer = {0,1,5};								    
//按键初始化函数
void KEY_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);//使能PORTA,PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;//KEY0-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE2,3,4

	//初始化 WK_UP-->GPIOA.0	  下拉输入
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0设置成输入，默认下拉	  
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.0

}
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY0按下
//2，KEY1按下
//3，KEY2按下 
//4，KEY3按下 WK_UP
//注意此函数有响应优先级,KEY0>KEY1>KEY2>KEY3!!
int KEY_Scan(u8 mode)
{	 
	int count=0;
	
	static u8 key_up=1;//按键按松开标志
	
	if(mode)key_up=1;  //支持连按		
   
	if(key_up&&(KEY0==0||KEY1==0||KEY2==0||WK_UP==1))
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY0==0)count= KEY0_PRES;
		else if(KEY1==0) 
		{
			if(KEY2==1)
		  count=KEY1_PRES;
			else
			count= PAUSE;
		}
		else if(KEY2==0) count=KEY2_PRES;
		else if(WK_UP==1)count= WKUP_PRES;
	}
	else if(KEY0==1&&KEY1==1&&KEY2==1&&WK_UP==0)key_up=1; 	   
  return count;	
 //	return 0;// 无按键按下
}
//	u8	Key_State=K_Idle;
//void KEY_Scan(void)
//{	 
//  // if(Key_State==K_Idle)
//	 //delay_ms(30);
//	{	   
//		//delay_ms(10);
//    if((KEY0 == 0)&&(KEY1 != 0)&&(KEY2 !=0)&&(WK_UP !=1))
//						{
//					    if(key0_Timer.DelayCnt <KEY_Over)  {key0_Timer.DelayCnt ++;}			
//							if(key0_Timer.DelayCnt >= key0_Timer.lDelayCnt)
//						{  
//							 Key_State= PAUSE;
//						}
//						}
//					 	else if((KEY0 != 0)&&(KEY1 == 0)&&(KEY2 !=0)&&(WK_UP !=1))
//						{
//							if(key1_Timer.DelayCnt <KEY_Over) {key1_Timer.DelayCnt ++;}								
//						if(key1_Timer.DelayCnt >= key1_Timer.lDelayCnt)
//							{
//								//	return KEY1_PRES;
//						  }
//						
//						}
//					 	else if((KEY0 != 0)&&(KEY1 != 0)&&(KEY2 ==0)&&(WK_UP !=1))
//						{
//							if(key2_Timer.DelayCnt <KEY_Over) {key2_Timer.DelayCnt ++;}								
//						if(key2_Timer.DelayCnt >= key2_Timer.lDelayCnt)
//							{
//								//	return KEY2_PRES;
//						  }
//						
//						}	
//					 	else if((KEY0 != 0)&&(KEY1 != 0)&&(KEY2 !=0)&&(WK_UP ==1))
//						{
//							if(wkup_Timer.DelayCnt <KEY_Over) {wkup_Timer.DelayCnt ++;}								
//						  if(wkup_Timer.DelayCnt >= wkup_Timer.lDelayCnt)
//							{
//						//			return WKUP_PRES;
//						  }
//						
//						}									
//						else 
//						{   
//						   if( (key0_Timer.DelayCnt >=key0_Timer.sDelayCnt) &&  (key0_Timer.DelayCnt < key0_Timer.lDelayCnt))							
//					   {	
//								 					Key_State= KEY0_PRES;	
//							}
//							if( (key1_Timer.DelayCnt >= key1_Timer.sDelayCnt) &&  (key1_Timer.DelayCnt < key1_Timer.lDelayCnt))								
//								{
//							Key_State= KEY1_PRES;
//							}	
//							if( (key2_Timer.DelayCnt >= key2_Timer.sDelayCnt)&&  (key2_Timer.DelayCnt < key2_Timer.lDelayCnt))	    
//							{		
//								Key_State= KEY2_PRES;
//							}	
//								if( (wkup_Timer.DelayCnt >= wkup_Timer.sDelayCnt)&&  (wkup_Timer.DelayCnt < wkup_Timer.lDelayCnt))	    
//							{		
//					       Key_State= WKUP_PRES;
//							}	
//					    key0_Timer.DelayCnt =0;  key1_Timer.DelayCnt =0;	key2_Timer.DelayCnt =0;  wkup_Timer.DelayCnt =0;
//	               // return Key_State;
//				     }	
//					 }
//}
