/**
  ******************************************************************************
  * @file    bsp_timer.c
  * @author  Jack Lu
  * @version V1.0
  * @date    2017-12-28
  * @brief   
  ******************************************************************************
	*/
#include "bsp_timer.h"

volatile int g_Tim6Array[(u16)Tim6_7] = {0,};//软件定时器计数存储器

///**********************************************************
// * 函数名 : Timerx_Init
// * 输入   ：arr：自动重装值�ppsc：时钟预分频数
// * 输出   ：none 
// * 功能   ：通用定时器初始化
// **********************************************************/
//void Timerx_Init(u16 arr,u16 psc)
//{
//  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);         //时钟使能
//					                       
//	TIM_TimeBaseStructure.TIM_Period = arr;                      //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
//	TIM_TimeBaseStructure.TIM_Prescaler = psc;                   //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
//	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                 //设置时钟分割:TDTS = Tck_tim 分频因子
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
//	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);              //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
// 
//	TIM_ITConfig(       //使能或者失能指定的TIM中断	 允许更新中断
//		TIM3,             //TIM3
//		TIM_IT_Update  |  //TIM 更新(溢出)中断                  
//		TIM_IT_Trigger,   //TIM 触发中断   （捕获中断等）					地址值	 ((uint16_t)0x0040)         
//		ENABLE            //使能
//		);
//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;            //TIM3中断
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;         //从优先级3级
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //IRQ通道被使能
//	NVIC_Init(&NVIC_InitStructure);                            //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

//	TIM_Cmd(TIM3, ENABLE);                                     //使能TIMx（定时器）外设 开启定时器						 
//}
////中断时间1ms//TIM3中断
//void TIM3_IRQHandler(void)   
//{
//	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
//	{
//		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );    //清除TIMx的中断待处理位:TIM 中断源 
//	}
//}
/**********************************************************
 * 函数名 : TIM5_PWM_Init
 * 输入   ：arr：自动重装值�ppsc：时钟预分频数
 * 输出   ：none 
 * 功能   ：通用定时器初始化
 **********************************************************/
void TIM5_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟使能
	
//	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3部分重映射  TIM3_CH2->PB5                                                                       	 //用于TIM3的CH2输出的PWM通过该LED显示
 
  //设置该引脚为复用输出功能,输出TIM5 CH1的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                    //TIM_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;              //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Period = arr;                      //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc;                    //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                 //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);              //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;            //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;                           //设置待装入捕获比较寄存器的脉冲值
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;    //输出极性:TIM输出比较极性高
	TIM_OC1Init(TIM5, &TIM_OCInitStructure);                     //根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);            //使能TIMx在CCR2上的预装载寄存器
	
	TIM_ARRPreloadConfig(TIM5, ENABLE); //使能TIMx在ARR上的预装载寄存器
	
	TIM_Cmd(TIM5, ENABLE);  //使能TIMx外设
}

/**********************************************************
 * 函数名 : TIM4_PWM_Init
 * 输入   ：arr：自动重装值�ppsc：时钟预分频数
 * 输出   ：none 
 * 功能   ：通用定时器初始化
 **********************************************************/
void TIM4_PWM_Init(u16 arr,u16 psc)
{  
	TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
  /* TIM6 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  
  /* --------------------------------------------------------
  TIM3 input clock (TIM6CLK) is set to 2 * APB1 clock (PCLK1), 
  since APB1 prescaler is different from 1.   
    TIM6CLK = 2 * PCLK1  
    TIM6CLK = HCLK / 2 = SystemCoreClock /2 
          
  TIM6 Update event occurs each TIM6CLK/256 

  Note: 
   SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f4xx.c file.
   Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
   function to update SystemCoreClock variable value. Otherwise, any configuration
   based on this variable will be incorrect.    

  ----------------------------------------------------------- */
  /* Time base configuration */
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
  TIM_TimeBaseStructure.TIM_Period = arr;          
  TIM_TimeBaseStructure.TIM_Prescaler = psc;       
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;    
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

  TIM_ITConfig(       //使能或者失能指定的TIM中断	 允许更新中断
		TIM4,             //TIM6
		TIM_IT_Update  |  //TIM 更新(溢出)中断                  
		TIM_IT_Trigger,   //TIM 触发中断   （捕获中断等）					地址值	 ((uint16_t)0x0040)         
		ENABLE            //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;            //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;         //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure); 
  
  /* TIM4 enable counter */
  TIM_Cmd(TIM4, ENABLE);
}

//中断时间1ms//TIM6中断
static SOFT_Module_PwmOut_Input pwm_channel_s[SOFT_TIMERS_MAX];

void TIM4_IRQHandler(void)   
{
 static	u16 i = 0;
	if( SET == TIM_GetITStatus(TIM4,TIM_IT_Update) )
	{
		TIM_ClearITPendingBit( TIM4, TIM_IT_Update);
		for(i=0;i<8;i++)
		if(pwm_channel_s[i].PWM_OutputNumCounter)//判断输出pwm数量
		{
			if(pwm_channel_s[i].PWM_OutputCounter!=pwm_channel_s[i].PWM_OutputChannelt_Fre)
			{
				if(pwm_channel_s[i].PWM_OutputCounter<=pwm_channel_s[i].PWM_OutputChannel_Duty_Cycle)
				{
					switch(i)
					{
						case 0: GPIOE->BRR=GPIO_Pin_0; break;
						case 1: GPIOE->BRR=GPIO_Pin_1; break;
						case 2: GPIOD->BRR=GPIO_Pin_13; break;
						case 3: GPIOD->BRR=GPIO_Pin_14; break;
						case 4: GPIOE->BRR=GPIO_Pin_15; break;
						case 5: GPIOE->BRR=GPIO_Pin_11; break;
						case 6: GPIOE->BRR=GPIO_Pin_13; break;
						case 7: GPIOE->BRR=GPIO_Pin_14; break;
						default: break;
					}
				}
				else 
					switch(i)
					{
						case 0: GPIOE->BSRR=GPIO_Pin_0; break;
						case 1: GPIOE->BSRR=GPIO_Pin_1; break;
						case 2: GPIOD->BSRR=GPIO_Pin_13; break;
						case 3: GPIOD->BSRR=GPIO_Pin_14; break;
						case 4: GPIOE->BSRR=GPIO_Pin_15; break;
						case 5: GPIOE->BSRR=GPIO_Pin_11; break;
						case 6: GPIOE->BSRR=GPIO_Pin_13; break;
						case 7: GPIOE->BSRR=GPIO_Pin_14; break;
						default: break;
					}
			}
			else 
				{
					pwm_channel_s[i].PWM_OutputCounter=0;
					pwm_channel_s[i].PWM_OutputNumCounter--;//计数减一
				}
			pwm_channel_s[i].PWM_OutputCounter++;
		}	 
//		soft_timer_exec();
//		if(IS_TIMEOUT_1MS(0, 10000))//周期10*4(us)
//		{
//			GPIOE->ODR^=GPIO_Pin_0;
//		}
//		if(IS_TIMEOUT_1MS(1, 5000))//周期10*4(us)
//		{
//			GPIOE->ODR^=GPIO_Pin_1;
//		}
		
	}
	
}
void PWM_init(void)
{
	SOFT_Module_PwmOut_Input pwmout_value;
	u8 i;
	
	pwmout_value.PWM_OutputChannelt_Fre=0;
 	pwmout_value.PWM_OutputChannel_Duty_Cycle=0;
	pwmout_value.PWM_OutputChannel_En=1;
	pwmout_value.PWM_OutputNumCounter=0;
	for(i=0;i<8;i++)
	{
		PWM_OutManage(i,pwmout_value,1);
	}
	pwmout_value.PWM_OutputChannelt_Fre=10;
	pwmout_value.PWM_OutputChannel_Duty_Cycle=5;
	pwmout_value.PWM_OutputChannel_En=1;
	pwmout_value.PWM_OutputNumCounter=1;
	PWM_OutManage(0,pwmout_value,1);
}
/**********************************************************
 * 函数名 : TIM1_Cap_Init
 * 输入   ：arr：自动重装值�ppsc：时钟预分频数
 * 输出   ：none 
 * 功能   ：定时器1通道1输入捕获配置
 **********************************************************/
void TIMx_Cap_Init(TIM_TypeDef* TIMx,u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM1_ICInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1|RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3|RCC_APB2Periph_TIM8, ENABLE);	//使能TIM1时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE); //使能GPIOA时钟
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1|GPIO_Pin_8;     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;             //PA8 输入  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1|GPIO_Pin_8);						  //PA8 下拉
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6|GPIO_Pin_7;     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;             
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC,GPIO_Pin_6|GPIO_Pin_7);
	
	//初始化定时器1 TIM1	 
	TIM_TimeBaseStructure.TIM_Period = arr;                      //设定计数器自动重装值 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	                 //预分频器   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;      //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);              //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
  
	//初始化TIM1输入捕获参数
	TIM1_ICInitStructure.TIM_Channel = TIM_Channel_1;                //CC1S=01 	选择输入端 IC1映射到TI1上
	TIM1_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	   //上升沿捕获
	TIM1_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
	TIM1_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	         //配置输入分频,不分频 
	TIM1_ICInitStructure.TIM_ICFilter = 0x00;                        //IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIMx, &TIM1_ICInitStructure);
	
	//中断分组初始化
	if(TIMx==TIM1)
		NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;            //TIM1中断
	else 
		if(TIMx==TIM2)
			NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;            //TIM2中断
		else 
			if(TIMx==TIM3)
				NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;            //TIM3中断
			else 
				if(TIMx==TIM8)
					NVIC_InitStructure.NVIC_IRQChannel = TIM8_CC_IRQn;            //TIM8中断
				
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;     //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            //从优先级0级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);                               //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 
	
	TIM_ITConfig(TIMx,TIM_IT_Update | TIM_IT_CC1,ENABLE);//允许更新中断 ,允许CC1IE捕获中断	
	
  TIM_Cmd(TIMx,ENABLE ); 	//使能定时器1
}

u8  TIM1CH1_CAPTURE_STA=0;	  //输入捕获状态		    				
u16	TIM1CH1_CAPTURE_VAL_H;	  //输入捕获值
u16	TIM1CH1_CAPTURE_VAL_L;	  //输入捕获值
u8  TIM1_FLAG=0;

//定时器1中断服务程序	 
void TIM1_CC_IRQHandler(void)
{ 
//	static u32 temp=0;
 	if(TIM1_FLAG==0)//还未成功捕获	
	{	  
		if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET) 
		{	    
			if(((TIM1CH1_CAPTURE_STA&0x40)==0x40))//已经捕获到高电平了
			{
				if((TIM1CH1_CAPTURE_STA&0x3F)==0X3F)//高电平太长了
				{
					TIM1_FLAG=1;//标记成功捕获了一次
					TIM1CH1_CAPTURE_VAL_H=0xFFFF;
				}else TIM1CH1_CAPTURE_STA++;
			}else if((TIM1CH1_CAPTURE_STA&0x80)==0x80)
			{
				if((TIM1CH1_CAPTURE_STA&0x3F)==0X3F)//高电平太长了
				{
					TIM1_FLAG=1;//标记成功捕获了一次
					TIM1CH1_CAPTURE_VAL_L=0xFFFF;
				}else TIM1CH1_CAPTURE_STA++;
			}
		}
	if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)//捕获1发生捕获事件
		{	
			if((TIM1CH1_CAPTURE_STA&0x40)==0x40)		//捕获到下降沿 		
			{	  			
				TIM1CH1_CAPTURE_VAL_H=TIM_GetCapture1(TIM1);
			  TIM1CH1_CAPTURE_VAL_H+=(TIM1CH1_CAPTURE_STA&0x3F)*65536;//记录高电平计数值
				TIM1CH1_CAPTURE_STA=0x00;	
				TIM1CH1_CAPTURE_STA|=0x80;
				TIM_SetCounter(TIM1,0);
				TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
			}	else if((TIM1CH1_CAPTURE_STA&0x80)==0x80)//捕捉到上升沿
				{
					TIM1CH1_CAPTURE_VAL_L=TIM_GetCapture1(TIM1);
					TIM1CH1_CAPTURE_VAL_L+=(TIM1CH1_CAPTURE_STA&0x3F)*65536;//记录低电平计数值
					TIM1CH1_CAPTURE_STA=0;	
					TIM1_FLAG=1;
				}	else
					{
						TIM1CH1_CAPTURE_STA=0;			  //清空，开始捕捉下降沿
						TIM1CH1_CAPTURE_VAL_H=0;
						TIM1CH1_CAPTURE_VAL_L=0;
						TIM_SetCounter(TIM1,0);
						TIM1CH1_CAPTURE_STA|=0x40;		//标记捕获到了上升沿
						TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
					}		    
		}			     	    					   
 	}
  TIM_ClearITPendingBit(TIM1, TIM_IT_CC1|TIM_IT_Update); //清除中断标志位
}

u8  TIM2CH1_CAPTURE_STA=0;	//输入捕获状态		    				
u16	TIM2CH1_CAPTURE_VAL_H;	  //输入捕获值
u16	TIM2CH1_CAPTURE_VAL_L;	  //输入捕获值 
u8  TIM2_FLAG;
//定时器1中断服务程序	 
void TIM2_IRQHandler(void)
{ 
//	static u32 temp=0;
 	if(TIM2_FLAG==0)//还未成功捕获	
	{	  
		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) 
		{	    
			if(((TIM2CH1_CAPTURE_STA&0x40)==0x40))//已经捕获到高电平了
			{
				if((TIM2CH1_CAPTURE_STA&0x3F)==0X3F)//高电平太长了
				{
					TIM2_FLAG=1;//标记成功捕获了一次
					TIM2CH1_CAPTURE_VAL_H=0xFFFF;
				}else TIM2CH1_CAPTURE_STA++;
			}else if((TIM2CH1_CAPTURE_STA&0x80)==0x80)
			{
				if((TIM2CH1_CAPTURE_STA&0x3F)==0X3F)//高电平太长了
				{
					TIM2_FLAG=1;//标记成功捕获了一次
					TIM2CH1_CAPTURE_VAL_L=0xFFFF;
				}else TIM1CH1_CAPTURE_STA++;
			}
		}
	if (TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)//捕获1发生捕获事件
		{	
			if((TIM2CH1_CAPTURE_STA&0x40)==0x40)		//捕获到下降沿 		
			{	  			
				TIM2CH1_CAPTURE_VAL_H=TIM_GetCapture1(TIM2);
			  TIM2CH1_CAPTURE_VAL_H+=(TIM2CH1_CAPTURE_STA&0x3F)*65536;//记录高电平计数值
				TIM2CH1_CAPTURE_STA=0x00;	
				TIM2CH1_CAPTURE_STA|=0x80;
				TIM_SetCounter(TIM1,0);
				TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
			}	else if((TIM2CH1_CAPTURE_STA&0x80)==0x80)//捕捉到上升沿
				{
					TIM2CH1_CAPTURE_VAL_L=TIM_GetCapture1(TIM1);
					TIM2CH1_CAPTURE_VAL_L+=(TIM2CH1_CAPTURE_STA&0x3F)*65536;//记录低电平计数值
					TIM2CH1_CAPTURE_STA=0;	
					TIM2_FLAG=1;
				}	else
					{
						TIM2CH1_CAPTURE_STA=0;			  //清空，开始捕捉下降沿
						TIM2CH1_CAPTURE_VAL_H=0;
						TIM2CH1_CAPTURE_VAL_L=0;
						TIM_SetCounter(TIM2,0);
						TIM2CH1_CAPTURE_STA|=0x40;		//标记捕获到了上升沿
						TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
					}		    
		}			     	    					   
 	}
  TIM_ClearITPendingBit(TIM2, TIM_IT_CC1|TIM_IT_Update); //清除中断标志位
}

u8  TIM3CH1_CAPTURE_STA=0;	//输入捕获状态		    				
u16	TIM3CH1_CAPTURE_VAL_H;	  //输入捕获值
u16	TIM3CH1_CAPTURE_VAL_L;	  //输入捕获值
u8  TIM3_FLAG;

//定时器1中断服务程序	 
void TIM3_IRQHandler(void)
{ 
//	static u32 temp=0;
 	if(TIM3_FLAG==0)//还未成功捕获	
	{	  
		if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) 
		{	    
			if(((TIM3CH1_CAPTURE_STA&0x40)==0x40))//已经捕获到高电平了
			{
				if((TIM3CH1_CAPTURE_STA&0x3F)==0X3F)//高电平太长了
				{
					TIM3_FLAG=1;//标记成功捕获了一次
					TIM3CH1_CAPTURE_VAL_H=0xFFFF;
				}else TIM3CH1_CAPTURE_STA++;
			}else if((TIM3CH1_CAPTURE_STA&0x80)==0x80)
			{
				if((TIM3CH1_CAPTURE_STA&0x3F)==0X3F)//高电平太长了
				{
					TIM3_FLAG=1;//标记成功捕获了一次
					TIM3CH1_CAPTURE_VAL_L=0xFFFF;
				}else TIM3CH1_CAPTURE_STA++;
			}
		}
	if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET)//捕获1发生捕获事件
		{	
			if((TIM3CH1_CAPTURE_STA&0x40)==0x40)		//捕获到下降沿 		
			{	  			
				TIM3CH1_CAPTURE_VAL_H=TIM_GetCapture1(TIM3);
			  TIM3CH1_CAPTURE_VAL_H+=(TIM3CH1_CAPTURE_STA&0x3F)*65536;//记录高电平计数值
				TIM3CH1_CAPTURE_STA=0x00;	
				TIM3CH1_CAPTURE_STA|=0x80;
				TIM_SetCounter(TIM3,0);
				TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
			}	else if((TIM3CH1_CAPTURE_STA&0x80)==0x80)//捕捉到上升沿
				{
					TIM3CH1_CAPTURE_VAL_L=TIM_GetCapture1(TIM3);
					TIM3CH1_CAPTURE_VAL_L+=(TIM3CH1_CAPTURE_STA&0x3F)*65536;//记录低电平计数值
					TIM3CH1_CAPTURE_STA=0;	
					TIM1_FLAG=1;
				}	else
					{
						TIM3CH1_CAPTURE_STA=0;			  //清空，开始捕捉下降沿
						TIM3CH1_CAPTURE_VAL_H=0;
						TIM3CH1_CAPTURE_VAL_L=0;
						TIM_SetCounter(TIM3,0);
						TIM3CH1_CAPTURE_STA|=0x40;		//标记捕获到了上升沿
						TIM_OC1PolarityConfig(TIM3,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
					}		    
		}
	}
  TIM_ClearITPendingBit(TIM3, TIM_IT_CC1|TIM_IT_Update); //清除中断标志位
}

u8  TIM8CH1_CAPTURE_STA=0;	  //输入捕获状态		    				
u16	TIM8CH1_CAPTURE_VAL_H;	  //输入捕获值
u16	TIM8CH1_CAPTURE_VAL_L;	  //输入捕获值 
u8  TIM8_FLAG;

//定时器1中断服务程序	 
void TIM8_CC_IRQHandler(void)
{ 
 	if(TIM8_FLAG==0)//还未成功捕获	
	{	  
		if (TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET) 
		{	    
			if(((TIM8CH1_CAPTURE_STA&0x40)==0x40))//已经捕获到高电平了
			{
				if((TIM8CH1_CAPTURE_STA&0x3F)==0X3F)//高电平太长了
				{
					TIM8_FLAG=1;//标记成功捕获了一次
					TIM8CH1_CAPTURE_VAL_H=0xFFFF;
				}else TIM8CH1_CAPTURE_STA++;
			}else if((TIM8CH1_CAPTURE_STA&0x80)==0x80)
			{
				if((TIM8CH1_CAPTURE_STA&0x3F)==0X3F)//高电平太长了
				{
					TIM8_FLAG=1;//标记成功捕获了一次
					TIM8CH1_CAPTURE_VAL_L=0xFFFF;
				}else TIM8CH1_CAPTURE_STA++;
			}
		}
	if (TIM_GetITStatus(TIM8, TIM_IT_CC1) != RESET)//捕获1发生捕获事件
		{	
			if((TIM8CH1_CAPTURE_STA&0x40)==0x40)		//捕获到下降沿 		
			{	  			
				TIM8CH1_CAPTURE_VAL_H=TIM_GetCapture1(TIM8);
			  TIM8CH1_CAPTURE_VAL_H+=(TIM8CH1_CAPTURE_STA&0x3F)*65536;//记录高电平计数值
				TIM8CH1_CAPTURE_STA=0x00;	
				TIM8CH1_CAPTURE_STA|=0x80;
				TIM_SetCounter(TIM8,0);
				TIM_OC1PolarityConfig(TIM8,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
			}	else if((TIM8CH1_CAPTURE_STA&0x80)==0x80)//捕捉到上升沿
				{
					TIM8CH1_CAPTURE_VAL_L=TIM_GetCapture1(TIM8);
					TIM8CH1_CAPTURE_VAL_L+=(TIM8CH1_CAPTURE_STA&0x3F)*65536;//记录低电平计数值
					TIM8CH1_CAPTURE_STA=0;	
					TIM8_FLAG=1;
				}	else
					{
						TIM8CH1_CAPTURE_STA=0;			  //清空，开始捕捉下降沿
						TIM8CH1_CAPTURE_VAL_H=0;
						TIM8CH1_CAPTURE_VAL_L=0;
						TIM_SetCounter(TIM8,0);
						TIM8CH1_CAPTURE_STA|=0x40;		//标记捕获到了上升沿
						TIM_OC1PolarityConfig(TIM8,TIM_ICPolarity_Falling);		//CC1P=1 设置为下降沿捕获
					}		    
		}			     	    					   
 	}
  TIM_ClearITPendingBit(TIM8, TIM_IT_CC1|TIM_IT_Update); //清除中断标志位
}

SOFT_Module_PwmIn_Output PWM_InManage(u8 PWM_InputChannel_Sel,SOFT_Module_PwmIn_Input value)
{
	SOFT_Module_PwmIn_Output out_temp;
	if(value.PWM_InputChannel_En==1)
	{
		switch(PWM_InputChannel_Sel)
		{
			case 0:
				if(TIM1_FLAG)//成功捕获到了一次上升沿
				{
					out_temp.PWM_InputChannel_Duty_Cycle=\
					(float)TIM1CH1_CAPTURE_VAL_H/(TIM1CH1_CAPTURE_VAL_H+TIM1CH1_CAPTURE_VAL_L)*100;
					out_temp.PWM_InputChannel_Fre=1/(float)(TIM1CH1_CAPTURE_VAL_H+TIM1CH1_CAPTURE_VAL_L)*pow(10,6);
//					printf("HIGH:%d us\r\n",TIM1CH1_CAPTURE_VAL_H);//打印总的高点平时间
//					printf("LOW:%d us\r\n",TIM1CH1_CAPTURE_VAL_L);//打印总的高点平时间
					printf("T :%d\n",TIM1CH1_CAPTURE_VAL_H+TIM1CH1_CAPTURE_VAL_L);
					printf("PWM :%0.2f%%\r\n",(float)TIM1CH1_CAPTURE_VAL_H/(TIM1CH1_CAPTURE_VAL_H+TIM1CH1_CAPTURE_VAL_L)*100);
					TIM1_FLAG=0;//开启下一次捕获
				}
				break;
			case 1:
				if(TIM2_FLAG)//成功捕获到了一次上升沿
				{
					out_temp.PWM_InputChannel_Duty_Cycle=\
					(float)TIM2CH1_CAPTURE_VAL_H/(TIM2CH1_CAPTURE_VAL_H+TIM2CH1_CAPTURE_VAL_L)*100;
					out_temp.PWM_InputChannel_Fre=1/(float)(TIM1CH1_CAPTURE_VAL_H+TIM1CH1_CAPTURE_VAL_L)*pow(10,6);
//					printf("HIGH:%d us\r\n",TIM1CH1_CAPTURE_VAL_H);//打印总的高点平时间
//					printf("LOW:%d us\r\n",TIM1CH1_CAPTURE_VAL_L);//打印总的高点平时间
					printf("T :%d\n",TIM2CH1_CAPTURE_VAL_H+TIM2CH1_CAPTURE_VAL_L);
//					printf("PWM :%0.2f%%\r\n",(float)TIM1CH1_CAPTURE_VAL_H/(TIM1CH1_CAPTURE_VAL_H+TIM1CH1_CAPTURE_VAL_L)*100);
					TIM2_FLAG=0;//开启下一次捕获
				}
				break;
			case 2:
				if(TIM3_FLAG)//成功捕获到了一次上升沿
				{
					out_temp.PWM_InputChannel_Duty_Cycle=\
					(float)TIM3CH1_CAPTURE_VAL_H/(TIM3CH1_CAPTURE_VAL_H+TIM3CH1_CAPTURE_VAL_L)*100;
					out_temp.PWM_InputChannel_Fre=1/(float)(TIM3CH1_CAPTURE_VAL_H+TIM3CH1_CAPTURE_VAL_L)*pow(10,6);
//					printf("HIGH:%d us\r\n",TIM1CH1_CAPTURE_VAL_H);//打印总的高点平时间
//					printf("LOW:%d us\r\n",TIM1CH1_CAPTURE_VAL_L);//打印总的高点平时间
					printf("T :%d\n",TIM3CH1_CAPTURE_VAL_H+TIM3CH1_CAPTURE_VAL_L);
//					printf("PWM :%0.2f%%\r\n",(float)TIM1CH1_CAPTURE_VAL_H/(TIM1CH1_CAPTURE_VAL_H+TIM1CH1_CAPTURE_VAL_L)*100);
					TIM3_FLAG=0;
				}
				break;
			case 3:
				if(TIM8_FLAG)//成功捕获到了一次上升沿
				{
					out_temp.PWM_InputChannel_Duty_Cycle=\
					(float)TIM8CH1_CAPTURE_VAL_H/(TIM8CH1_CAPTURE_VAL_H+TIM8CH1_CAPTURE_VAL_L)*100;
					out_temp.PWM_InputChannel_Fre=1/(float)(TIM8CH1_CAPTURE_VAL_H+TIM8CH1_CAPTURE_VAL_L)*pow(10,6);
//					printf("HIGH:%d us\r\n",TIM1CH1_CAPTURE_VAL_H);//打印总的高点平时间
//					printf("LOW:%d us\r\n",TIM1CH1_CAPTURE_VAL_L);//打印总的高点平时间
					printf("T :%d\n",TIM8CH1_CAPTURE_VAL_H+TIM8CH1_CAPTURE_VAL_L);
//					printf("PWM :%0.2f%%\r\n",(float)TIM1CH1_CAPTURE_VAL_H/(TIM1CH1_CAPTURE_VAL_H+TIM1CH1_CAPTURE_VAL_L)*100);
					TIM8_FLAG=0;
				}
				break;
			default:break;
		}
	}
	out_temp.SOFT_Module_PwmIn_Module_Status=value.PWM_InputChannel_En;
	return out_temp;
}
/***********************************************************             
  * @brief  TIM6 Configuration
  * @note   TIM6 configuration is based on APB1 frequency
  * @note   TIM6 Update event occurs each TIM6CLK/256   
  * @param  None
  * @retval None
  **********************************************************/
void TIM6_Config(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
	
	NVIC_InitTypeDef NVIC_InitStructure;
	
  /* TIM6 Periph clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
  
  /* --------------------------------------------------------
  TIM3 input clock (TIM6CLK) is set to 2 * APB1 clock (PCLK1), 
  since APB1 prescaler is different from 1.   
    TIM6CLK = 2 * PCLK1  
    TIM6CLK = HCLK / 2 = SystemCoreClock /2 
          
  TIM6 Update event occurs each TIM6CLK/256 

  Note: 
   SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f4xx.c file.
   Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
   function to update SystemCoreClock variable value. Otherwise, any configuration
   based on this variable will be incorrect.    

  ----------------------------------------------------------- */
  /* Time base configuration */
  TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
  TIM_TimeBaseStructure.TIM_Period = arr;          
  TIM_TimeBaseStructure.TIM_Prescaler = psc;       
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;    
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

  TIM_ITConfig(       //使能或者失能指定的TIM中断	 允许更新中断
		TIM6,             //TIM6
		TIM_IT_Update  |  //TIM 更新(溢出)中断                  
		TIM_IT_Trigger,   //TIM 触发中断   （捕获中断等）					地址值	 ((uint16_t)0x0040)         
		ENABLE            //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;            //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure); 
  
  /* TIM6 enable counter */
  TIM_Cmd(TIM6, ENABLE);
}

/**********************************************************
 * 函数名 : BspTim6Close
 * 输入   ：none
 * 输出   ：none 
 * 功能   ：关闭定时器
 **********************************************************/
void BspTim6Close(void)
{
    TIM_ITConfig(TIM6, TIM_IT_Update, DISABLE);
    TIM_Cmd(TIM6, DISABLE);
}

//中断时间1ms//TIM6中断
void TIM6_IRQHandler(void)   
{
	u16 i = 0;
  
	if( SET == TIM_GetITStatus(TIM6,TIM_IT_Update) )
	{
		TIM_ClearITPendingBit( TIM6, TIM_IT_Update);
		
		for (i = 0; i < (u16)Tim6_7; i++)
		{
			g_Tim6Array[i]++;
		}	
	}
}
//SOFT_Module_Tim_Output TIM_Manage
/************************************软件定时器******************************/
/**********************************************************
 * 函数名 : callbackfun
 * 输入   ：通道
 * 输出   ：none 
 * 功能   ：PWM输出回调函数
 **********************************************************/
void callbackfun(uint8_t TimerID)
{
	switch(TimerID)
	{
		case 0: 
			GPIOE->ODR^=GPIO_Pin_0;
			break;
		case 1:
			GPIOE->ODR^=GPIO_Pin_1;
			break;
		case 2:
			GPIOD->ODR^=GPIO_Pin_13;
			break;
		case 3:
			GPIOD->ODR^=GPIO_Pin_14;
			break;
		case 4:
			GPIOD->ODR^=GPIO_Pin_15;
			break;
		case 5:
			GPIOE->ODR^=GPIO_Pin_11;
			break;
		case 6:
			GPIOE->ODR^=GPIO_Pin_13;
			break;
		case 7:
			GPIOE->ODR^=GPIO_Pin_14;
			break;
		default:
			break;
	}
}



/* 
* !brief timer_list, you can change the timers num by SOFT_TIMERS_MAX
  */
  static soft_timer_t timer_list[SOFT_TIMERS_MAX];
/*
* !brief module init, you can run this function in first main
  */
  void soft_timer_init(void)
  {
    uint8_t i;
    for (i = 0; i < SOFT_TIMERS_MAX; i++) {
        timer_list[i].TimerID = 0;
        timer_list[i].time_out = 0;
        timer_list[i].time_out_callback = NULL;
    }
  }

/*
* !brief use this function to create a soft_timer
* !param first time the timer count machine's period
* !param every loop cycles
* !param callback run with the time up
* !param callback 's param
* !param 0:periods 1:one shot
* !retval timer handle
  */
  TimerHandle_t soft_timer_registered(const char * const timer_name,
                                 const uint32_t period, 
								                 const uint32_t AutoReload,   
                                 uint8_t TimerID ,
                                 time_out_callback callback)
  {  
		if (callback == NULL) return NULL;
		if (timer_list[TimerID].time_out_callback != NULL) ;
		timer_list[TimerID].time_out = period;
		timer_list[TimerID].time_out_callback = callback;
		timer_list[TimerID].TimerID = TimerID;
	  
    return (TimerHandle_t)&timer_list[TimerID];
  }
/*设置定时器重装载值*/
  TimerHandle_t set_timer(const char * const timer_name,
								 const uint32_t num, 
                                 const uint32_t period, 
								                 const uint32_t AutoReload,   
                                 uint8_t TimerID ,
                                 time_out_callback callback)
  {  
		if (callback == NULL) return NULL;
		if (timer_list[TimerID].time_out_callback != NULL) ;
		timer_list[TimerID].time_counter=num;
		timer_list[TimerID].time_out = period;
		timer_list[TimerID].time_out_callback = callback;
		timer_list[TimerID].TimerID = TimerID;
	  
    return (TimerHandle_t)&timer_list[TimerID];
  }
/*
* !brief you can use this function to delete a soft_timer
* !param timer handle
* !retval 0 is successful, 1 is fail
  */
  int soft_timer_delete(uint32_t handle)
  {
    soft_timer_t *p = (soft_timer_t *)handle;

    if (p == NULL) return 1;
    p->time_out_callback = NULL;
    
    return 0;
  }

/*
* !brief you should run this function in main loop
  */
  void soft_timer_exec(void)
  {
		u8 i;
		for (i = 0; i < 2; i++)
		{
			if(IS_TIMEOUT_1MS(i, timer_list[i].time_out))
			{
				timer_list[i].time_out_callback(timer_list[i].TimerID);
			}
		}
  }
/**********************************************************
 * 函数名 : PWM_Out_Init
 * 输入   ：none
 * 输出   ：none 
 * 功能   ：PWM输出引脚初始化
 **********************************************************/	
void PWM_Out_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE); //使能GPIOA时钟
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_14;     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;             
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOE,GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_14);	

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;             
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOD,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);		
}
/**********************************************************
 * 函数名 : PWM_OutMange
 * 输入   ：none
 * 输出   ：none 
 * 功能   ：PWM输出控制模块
 **********************************************************/
SOFT_Module_PwmOut_Output PWM_OutManage(uint8_t  PWM_OutputChannel_Sel,SOFT_Module_PwmOut_Input value,u8 ret_data)
{
	SOFT_Module_PwmOut_Output temp;
	if(ret_data)//只查询是否执行完成
	{
		if(value.PWM_OutputNumCounter!=pwm_channel_s[PWM_OutputChannel_Sel].PWM_OutputNumCounter)
		{
			pwm_channel_s[PWM_OutputChannel_Sel].PWM_OutputCounter=0;
			pwm_channel_s[PWM_OutputChannel_Sel].PWM_OutputChannelt_Fre=value.PWM_OutputChannelt_Fre;
			pwm_channel_s[PWM_OutputChannel_Sel].PWM_OutputChannel_Duty_Cycle=value.PWM_OutputChannel_Duty_Cycle;
			pwm_channel_s[PWM_OutputChannel_Sel].PWM_OutputNumCounter=value.PWM_OutputNumCounter;
		}
	}
	temp.PWM_Count=pwm_channel_s[PWM_OutputChannel_Sel].PWM_OutputNumCounter;
	if(pwm_channel_s[PWM_OutputChannel_Sel].PWM_OutputNumCounter==0)//PWM输出脉冲计数到达时关闭软件模块
	temp.SOFT_Module_PwmOut_Module_Status=0;
	return temp;
}
