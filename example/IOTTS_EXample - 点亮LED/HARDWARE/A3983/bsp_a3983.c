/**
  ******************************************************************************
  * @file    bsp_a3983.c
  * @author  Jack Lu
  * @version V1.0
  * @date    2017-10-25
  * @brief   步进电机驱动
  ******************************************************************************
  */
#include "bsp_a3983.h"

static u8 SetMotorEnableReg;
u8 key_flag=0;

u8 MILK_Check_Flash=0;//检测完成标志

static volatile uint16_t Step1_counter=0;
static volatile uint16_t Step2_counter=0;

static  MOTOR_STATUS motormode=MOTORVTOZ;//电机上电进入Z轴校准模式
static  u8 Device_Start_First_Flag=0;//设备第一次启动标志

static MotorSelect motor_se=MOTOR_VERTICAL;//上电控制Z轴电机

/**********************************************************
 * 函数名 : A3983_Init
 * 输入   ：none 
 * 输出   ：none 
 * 功能   ：电机驱动控制脚初始化
 **********************************************************/
void A3983_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO,ENABLE);
  
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	/*
	初始设置电机转向为顺时针
	*/
  MOTOR_VERTICAL_DIR_0;
	MOTOR_ROTARY_DIR_0;
	/*
	开启电机使能控制 低电平有效
	*/
	MOTOR_ROTARY_EN_0;
	MOTOR_VERTICAL_EN_0;
	
	//定时器初始化
	TIM_DeInit(TIM4);
	TIM_TimeBaseStruct.TIM_Period=MOTOR_FRE-1;//初值
	TIM_TimeBaseStruct.TIM_Prescaler=MOTOR_SPEED-1;//预分频
	TIM_TimeBaseStruct.TIM_ClockDivision=0;
	TIM_TimeBaseStruct.TIM_CounterMode=TIM_CounterMode_Up;//向上计数
  TIM_TimeBaseStruct.TIM_RepetitionCounter = 0x0;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStruct);
	
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	TIM_ITConfig( TIM4, TIM_IT_Update | TIM_IT_Trigger, ENABLE);
	
	//pwm 初始化
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	
	TIM_OCInitStructure.TIM_Pulse = MOTOR_FRE; 
	TIM_OC3Init(TIM4,&TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);
  
	TIM_OCInitStructure.TIM_Pulse = MOTOR_FRE; 
	TIM_OC4Init(TIM4,&TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);
	
	TIM_Cmd(TIM4,ENABLE); 

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); 
}

/**********************************************************
 * 函数名 : TIM4_IRQHandler
 * 输入   ：none
 * 输出   ：none 
 * 功能   ：
 **********************************************************/
void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除TIMx的中断待处理位:TIM 中断源 
		if(Step1_counter>0)//计算Z轴电机旋转角度
		{
			Step1_counter--;
		}
		if(Step2_counter>0)//计算y轴电机旋转角度
		{
			Step2_counter--;
		}	
	}
}

/*---------------------------------------电机配置-----------------------------------------------*/

/**********************************************************
 * 函数名 : SetPwmFre
 * 输入   ：Fre ：设置电机PWM频率
 * 输出   ：none  
 * 功能   ：设置电机PWM频率
 **********************************************************/
void SetPwmFre(u16 Fre , MotorSelect MotorSelect_value)
{	
	if(MOTOR_VERTICAL==MotorSelect_value)TIM4->CCR3=Fre/2;
			else TIM4->CCR4=Fre/2;//open the rotary motor
}

/**********************************************************
 * 函数名 : SetMotorWork
 * 输入   ：speed ：电机转速 dir :电机转向 mic 电机步进值 EN：电机使能
 * 输出   ：none 
 * 功能   ：控制电机动作
 **********************************************************/
void SetMotorWork(u16 speed, MotorDir dir, u8 mic, MotorState EN, MotorSelect MotorSelect_value)
{
	SetPwmFre(speed,MotorSelect_value);

	//CtrMicrostep(mic);
	
	CtrMotorDir(dir,MotorSelect_value);

	CtrMotorRun(EN,MotorSelect_value);

}

/**********************************************************
 * 函数名 : CtrMotorDir
 * 输入   ：value : MOTOR_CLOCK_WISE 电机顺时针 MOTOR_COUNTER_CLOCK_WISE 电机逆时针
 * 输出   ：none 
 * 功能   ：电机正反转控制
 **********************************************************/
void CtrMotorDir(MotorDir value , MotorSelect MotorSelect_value)
{	
	if(MOTOR_COUNTER_CLOCKWISE == value)
	{
		if(MOTOR_VERTICAL==MotorSelect_value)MOTOR_VERTICAL_DIR_1;
			else MOTOR_ROTARY_DIR_1;
	}
	else
	{
		if(MOTOR_VERTICAL==MotorSelect_value)MOTOR_VERTICAL_DIR_0;
			else MOTOR_ROTARY_DIR_0;
	}
}
/*
void CtrMicrostep(unsigned char value)
{

	if(Microstep != value)
	{
		Microstep = value;
		if(value == Microstep_1)
		{
			IO_MS1_0;
			IO_MS2_0;
		}
		else if(value == Microstep_2)
		{
			IO_MS1_1;
			IO_MS2_0;
		}
		else if(value == Microstep_4)
		{
			IO_MS1_0;
			IO_MS2_1;
		}
		else if(value == Microstep_16)
		{
			IO_MS1_1;
			IO_MS2_1;
		}
	}
}
*/

/**********************************************************
 * 函数名 : CtrMotorRun
 * 输入   ：value : MOTOR_ON 电机启动 MOTOR_OFF 电机停止
 * 输出   ：none 
 * 功能   ：控制电机驱动是否工作
 **********************************************************/
void CtrMotorRun(MotorState value, MotorSelect MotorSelect_value)
{
	if(SetMotorEnableReg != value)
	{
		SetMotorEnableReg = value;
		if( value==MOTOR_ON )
		{
			if(MOTOR_VERTICAL==MotorSelect_value)MOTOR_VERTICAL_EN_0;//open the vertical motor
			else MOTOR_ROTARY_EN_0;//open the rotary motor
		}
		else
		{
			if(MOTOR_VERTICAL==MotorSelect_value)MOTOR_VERTICAL_EN_1;//close the vertical motor
			else MOTOR_ROTARY_EN_1;//close the rotary motor
		}
	}
}

/**********************************************************
 * 函数名 : CtrMotorBrake
 * 输入   ：value : MOTOR_ON 电机启动 MOTOR_OFF 电机停止
 * 输出   ：none 
 * 功能   ：电机刹车控制
 **********************************************************/
void CtrMotorBrake(MotorState value, MotorSelect MotorSelect_value)
{
	if(motor_se!=MotorSelect_value)motor_se=MotorSelect_value;
	switch(motor_se)
		{
		case MOTOR_VERTICAL : 
			if( value==MOTOR_OFF )TIM4->CCR3=MOTOR_FRE;
		  else TIM4->CCR3=MOTOR_FRE/2;
			break;
		case MOTOR_ROTARY	:
			if( value==MOTOR_OFF )TIM4->CCR4=MOTOR_FRE;
		  else TIM4->CCR4=MOTOR_FRE/2;
			break;
		default : break;
		}
}

/**********************************************************
 * 函数名 : CtrMotorStepControl
 * 输入   ：value : 电机旋转角度
 * 输出   ：none 
 * 功能   ：电机旋转角度设置
 **********************************************************/
void CtrMotorStepControl(u16 value, MotorSelect MotorSelect_value)
{
	if(motor_se!=MotorSelect_value)motor_se=MotorSelect_value;
	switch(motor_se)
		{
		case MOTOR_VERTICAL :
			Step1_counter=value;   //装入步进值
			TIM4->CCR3=MOTOR_FRE/2;//松开刹车
		  while(Step1_counter>0) //等待电机动作完成
				vTaskDelay(1);
			TIM4->CCR3=MOTOR_FRE;  //刹车
			break;
		case MOTOR_ROTARY	:
			Step2_counter=value;   //装入步进值 
			TIM4->CCR4=MOTOR_FRE/2;//松开刹车
		  while(Step2_counter>0) //等待电机动作完成
				vTaskDelay(1);
			TIM4->CCR4=MOTOR_FRE;  //刹车
			break;
		default : break;
		}
}

/*-----------------------------------------------------------------------------------------------*/

/**********************************************************
 * 函数名 : MOTORVTOZ_manage
 * 输入   ：none
 * 输出   ：none 
 * 功能   ：电机纵轴归零操作函数，开机校准一次，每次测量完成都需校准
 **********************************************************/
uint8_t MOTORVTOZ_manage(void)
{
	if(Motor_Location.PS_Vertical_State!=MOTOR_Sensor_ON)//开机传感器无遮挡
	{
		CtrMotorDir(MOTOR_COUNTER_CLOCKWISE , MOTOR_VERTICAL);//设置电机逆时针旋转
		CtrMotorBrake(MOTOR_ON, MOTOR_VERTICAL);//关闭电机刹车
	}
	else if(Motor_Location.PS_Vertical_State==MOTOR_Sensor_ON)//开机限位传感器有遮挡
	{
		CtrMotorBrake(MOTOR_OFF, MOTOR_VERTICAL);//电机刹车
		return 0;
	}
	return 1;
}

/**********************************************************
 * 函数名 : MOTORRTOZ_manage
 * 输入   ：none
 * 输出   ：none 
 * 功能   ：电机转盘归零操作函数
 **********************************************************/
uint8_t MOTORRTOZ_manage(void)
{
	if((Motor_Location.PS_Rotary_State!=MOTOR_Sensor_ON)&&(Motor_Location.PS_Vertical_State==MOTOR_Sensor_ON))//开机传感器无遮挡
	{
		CtrMotorDir(MOTOR_CLOCKWISE , MOTOR_ROTARY);//设置电机顺时针旋转
		CtrMotorBrake(MOTOR_ON, MOTOR_ROTARY);//关闭电机刹车
	}
	else if(Motor_Location.PS_Vertical_State!=MOTOR_Sensor_ON)//纵轴无遮挡
	{
	  motormode=MOTORVTOZ;   // 纵轴未归位返回纵轴归位步骤
	}
	else if(Motor_Location.PS_Rotary_State==MOTOR_Sensor_ON)//开机有遮挡
	{
		CtrMotorBrake(MOTOR_OFF, MOTOR_ROTARY);
		return 0;// 转盘已归位，进入下一步
	}
	return 1;
}

/**********************************************************
 * 函数名 : MOTORDOWN_manage
 * 输入   ：none
 * 输出   ：none 
 * 功能   ：电机纵轴下降操作函数
 **********************************************************/
uint8_t MOTORDOWN_manage(void)
{
	if(Motor_Location.PS_Vertical_State==MOTOR_Sensor_ON)//如果吸管在纵轴最顶端时允许电机下降操作
	{
		CtrMotorDir(MOTOR_CLOCKWISE , MOTOR_VERTICAL);//设置电机逆时针旋转
		CtrMotorStepControl(MOTOR_VERTICAL_DIS, MOTOR_VERTICAL);//z轴下降准备测试
	}
	return 0;	
}

/**********************************************************
 * 函数名 : MOTORWAIT_manage
 * 输入   ：none
 * 输出   ：none 
 * 功能   ：等待检测完成
 **********************************************************/

uint8_t MOTORWAIT_manage(void)
{
	u16 i=0;
//	u8 i=0;
//  if((usart_re_flag==4)||(lcd_dis.menue_flag==59))//检测完成标志
//	{
//		lcd_dis.menue_flag=100;
//		usart_re_flag=0;
//		MOTORVTOZ_manage();
////		for(i=0;i<TUBE_SUM;i++)
////		{
////			
////			vTaskDelay(5);
////		}
//		key_value=MILK_SET;
//	}
//	else if(key_flag==0)//防止多次触发
//	{
//		key_flag=1;
//		KEY_Manage(KEY_CONFIRM);
//	}
	while(Check_ONE_Finash_Flag==0)
	{
		vTaskDelay(10);
		if(i>9000)break;
		i++;
//		if(i%100==0)
//		DEBUG_Msg("i=%d",i/100);
	}
	vTaskDelay(5000);
	Check_ONE_Finash_Flag=0;
//vTaskDelay(50000);
//	MILK_Check_Flash=0xF1;
	return 0;
}


/**********************************************************
 * 函数名 : MOTORSTOP_manage
 * 输入   ：none
 * 输出   ：none 
 * 功能   ：电机停转操作函数 检测完成时使用
 **********************************************************/
uint8_t MOTORSTOP_manage(void)
{
	
	return 0;
}

/**********************************************************
 * 函数名 : MOTORMANAGE_CLOCKWISE_manage
 * 输入   ：none
 * 输出   ：none 
 * 功能   ：电机处理程序
 **********************************************************/
uint8_t MOTORMANAGE_CLOCKWISE_manage(void)
{
	CtrMotorDir(MOTOR_CLOCKWISE , MOTOR_ROTARY);//设置电机顺时针旋转
	CtrMotorStepControl(MOTOR_ROTARY_ANGLE_ONETIME, MOTOR_ROTARY);//旋转一个试管位置
	return 0;
}

/**********************************************************
 * 函数名 : MOTORMANAGE_COUNTER_CLOCKWISE_manage
 * 输入   ：none
 * 输出   ：none 
 * 功能   ：电机处理程序
 **********************************************************/
uint8_t MOTORMANAGE_COUNTER_CLOCKWISE_manage(void)
{
	CtrMotorDir(MOTOR_COUNTER_CLOCKWISE , MOTOR_ROTARY);//设置电机顺时针旋转
	CtrMotorStepControl(MOTOR_ROTARY_ANGLE_ONETIME, MOTOR_ROTARY);//旋转一个试管位置
	return 0;
}

/**********************************************************
 * 函数名 : MotorExec
 * 输入   ：none
 * 输出   ：none 
 * 功能   ：电机操作函数 
 **********************************************************/

//void MotorExec(void)
//{
//	u8 i=0;
//	static u8 counter=0;
//	TUBE_STATUS Flash_Data_read;
//	/*
//	测试数据
//	*/
////	Tube_Status_Flash.Tube_sum_one=3;//装入8个试管
//	/******************/
//	switch (motormode)
//   {
//    case MOTORVTOZ:     //纵轴归位
//		 if((MOTORVTOZ_manage()==0)&&(Device_Start_First_Flag==0))//判断是否是首次启动
//		 {
//			 Device_Start_First_Flag=1;
//			 motormode=MOTORRTOZ;
//		 }
//		 else if(MOTORVTOZ_manage()==0)
//		 {
//			motormode=MOTORRETREAT;
//			/*
//			判断是否所有试管检测完成
//			*/
//			if(Tube_Status_Flash.Tube_num>=Tube_Status_Flash.Tube_sum_one)
//			{
//				Tube_Status_Flash.Tube_num=0;
//				/*
//				蜂鸣器提示
//				*/
//				MILK_Check_Flash=0xFE;
//				DEBUG_Msg("检测完成");
//				ScreenCommand(0,0,0);
//				screenControl=WAIT;
//				Tube_Status_Flash.Tube_ins_sts=0;//切换设备状态
//				for(i=0;i<24;i++)
//				Tube_Status_Flash.Tube_sts[i]=0;
//				motormode=MOTORRTOZ;
//			}

//		 }
//        break; 
//    case MOTORRTOZ:     //转盘归位
//				if(MOTORRTOZ_manage()==0)
//					motormode=MOTORMANAGE;
//        break;
//    case MOTORMANAGE:   //处理试管个数及旋转角度
//				if(screenControl==FINISH_CHECK_CONFIRM)MILK_Check_Flash=0x01;
//				if((screenControl==INS_TUBE_COUNTER_CLOCKWISE)||(screenControl==INS_TUBE_CLOCKWISE)||(screenControl==TAKE_DOWN_TUBE))//收到安装试管命令及取下试管操作
//				{
//					if(screenControl==INS_TUBE_COUNTER_CLOCKWISE)
//					{
//						if(Tube_Status_Flash.Tube_sum_one<24)
//							{
//								/*当前正在安装试管*/
//								Tube_Status_Flash.Tube_ins_sts=0;
//								/*试管总数加1 统计试管总数*/
//								Tube_Status_Flash.Tube_sum_one+=3;
//								/*将试管数保存到Flash中*/
//								for(i=0;i<3;i++)
//								/*安装试管时旋转一个试管位置*/
//								MOTORMANAGE_COUNTER_CLOCKWISE_manage();
//							}
//					}
//					else 
//						if(screenControl==INS_TUBE_CLOCKWISE)
//						{
//							if(Tube_Status_Flash.Tube_sum_one!=0)
//							{
//								/*当前正在安装试管*/
//								Tube_Status_Flash.Tube_ins_sts=0;
//								/*试管总数加1 统计试管总数*/
//								Tube_Status_Flash.Tube_sum_one-=3;
//								
//								for(i=0;i<3;i++)
//								/*安装试管时旋转一个试管位置*/
//								MOTORMANAGE_CLOCKWISE_manage();
//							}
//						}
//					 else 
//						 if(screenControl==TAKE_DOWN_TUBE)
//							{
//								for(i=0;i<3;i++)
//								/*安装试管时旋转一个试管位置*/
//								MOTORMANAGE_CLOCKWISE_manage();
//							}
//					/*
//					添加屏幕显示命令
//					*/
//							
//					/***************/
////					/*将试管数保存到Flash中*/
////					SPI_Flash_Save();		
//					screenControl=WAIT;
//					/*将试管数保存到Flash中*/
//					SPI_Flash_Save();		
//					DEBUG_Msg("试管总数:%d", Tube_Status_Flash.Tube_sum_one);		
//				}
//				else if((screenControl==MODE_ONE_START_CHECK)||(screenControl==MODE_TWO_START_CHECK)||(screenControl==MODE_THREE_START_CHECK))//收到开始检测命令
//				{
////					screenControl=WAIT;
//					if(Tube_Status_Flash.Tube_ins_sts==0)//安装完成进入检测前准备
//					{
//						/*当前正在检测*/
//						Tube_Status_Flash.Tube_ins_sts=1;//切换设备状态
//						/*
//						转盘归零
//						*/
//						while(MOTORRTOZ_manage()!=0)vTaskDelay(10);
//						/*
//						旋转到检测传感器位置
//						*/
//						for(i=0;i<9;i++)//旋转到检测传感器位置
//						{
//							MOTORMANAGE_COUNTER_CLOCKWISE_manage();
//						}
//					}	
//					if(Tube_Status_Flash.Tube_num<Tube_Status_Flash.Tube_sum_one)//判断当前试管数量是否超过总数
//						{
//							if(Motor_Location.PS_X_State==MOTOR_Sensor_ON)//检测有无试管传感器判断当前状态
//								{
//									Tube_Status_Flash.Tube_num++;
//									/*添加试管处理程序,判断当前是否有试管及检测试管数量统计*/
//									Tube_Status_Flash.Tube_sts[Tube_Status_Flash.Tube_num]|=0x10;//检测到试管
//									GPIOD->BSRR=GPIO_Pin_5;
//									/*正常检测模式下旋转一个试管位置*/
//									MOTORMANAGE_COUNTER_CLOCKWISE_manage();
//									vTaskDelay(200);	
//									for(i=0;i<2;i++)
//									{
//									 /*按键触发扫描枪*/
//										GPIOD->BSRR=GPIO_Pin_5;
//										vTaskDelay(1000);
//										GPIOD->BRR=GPIO_Pin_5;
//										/*--------------------*/							
//										vTaskDelay(2000);//延时
//										
//										/*检测扫码是否成功*/
//										if((Tube_Status_Flash.Tube_sts[Tube_Status_Flash.Tube_num]&0x0F)==0x0F)
//										{
//											/*正常检测模式下旋转一个试管位置*/
//											for(i=0;i<2;i++)
//											{
//												MOTORMANAGE_COUNTER_CLOCKWISE_manage();/*!<此时试管位于吸管正下方*/
//												vTaskDelay(500);
//											}
//											/*进入下一个模式*/
//											motormode=MOTORDOWN;	
//											break;
//										} 
//									 }
//								/*将试管数保存到Flash中*/
//								SPI_Flash_Save();
//								if(screenControl==MODE_ONE_START_CHECK)
//								ScreenCommand(1,1,0);	
//								else if(screenControl==MODE_TWO_START_CHECK)
//								ScreenCommand(1,1,1);	
//								else if(screenControl==MODE_THREE_START_CHECK)
//								ScreenCommand(1,1,2);						
//									DEBUG_Msg("正在检测试管:%d,试管总数:%d", Tube_Status_Flash.Tube_num,Tube_Status_Flash.Tube_sum_one);
//								}
//							else 
//								{
//									/*
//									 添加有无试管统计
//									*/
//									/*----------------------------------------------------------------------------------*/
//									
//			//					  Tube_Status_Flash.Tube_sts[Tube_Status_Flash.Tube_num]&=0xEF;//没有检测到试管
//									if(counter<23)
//									/*没有检测到试管继续旋转*/
//									{
//										MOTORMANAGE_COUNTER_CLOCKWISE_manage();
//										counter++;
//										vTaskDelay(500);
//										
//									}
//									else 
//										{
//											counter=0;
//											Tube_Status_Flash.Tube_num=0;
//											/*
//											蜂鸣器提示
//											*/
//											MILK_Check_Flash=0xFE;
//											DEBUG_Msg("检测完成");
//											ScreenCommand(0,0,0);
//											screenControl=WAIT;
//											Tube_Status_Flash.Tube_ins_sts=0;//切换设备状态
//											for(i=0;i<24;i++)
//											Tube_Status_Flash.Tube_sts[i]=0;
//											motormode=MOTORRTOZ;
//										}
//								}
//						}
//						else  motormode=MOTORVTOZ;
//				}
//				/*
//				点击取消后响应
//				*/
//				else if((screenControl==MODE_ONE_CANCEL)||(screenControl==MODE_TWO_CANCEL)||(screenControl==MODE_THREE_CANCEL))
//							{
//								Tube_Status_Flash.Tube_sum_one=3;
//								motormode=MOTORRTOZ;
//								/*将试管数保存到Flash中*/
//								SPI_Flash_Save();
//							}
//				data_re_sc.SAMPLING_LOOP=Tube_Status_Flash.Tube_sum_one; 
//				ScreenCommand(2,1,0);//采样模式中显示检测试管总数 			
//        break;
//		case MOTORDOWN:            //纵轴放下吸管
//				MOTORDOWN_manage();
//		    motormode = MOTORWAIT; //进入乳液检测模式
//        break;
//		case MOTORWAIT:         //等待检测完成
////			  /*
////				将正在测试的试管的状态存入Flash中
////				*/
////				SPI_Flash_Save();
//		    MOTOR_ROTARY_EN_1;
//				MOTOR_VERTICAL_EN_1;
//		    KEY_Manage(KEY_CONFIRM);//按下开始按键
//			  MOTORWAIT_manage(); //等待检测完成
//		    motormode=MOTORVTOZ;//升起吸管
//				MOTOR_ROTARY_EN_0;
//				MOTOR_VERTICAL_EN_0;
//        break;
//    case MOTORSTOP:         //电机关闭
//				MOTORSTOP_manage();
//        break; 
//   	
//		case MOTORRETREAT:         //电机回退
//			for(i=0;i<2;i++)//回退两个试管位置
//			{
//				MOTORMANAGE_CLOCKWISE_manage();
//				vTaskDelay(500);
//			}
//			motormode=MOTORMANAGE;
//			break; 	
//    default:
//        motormode = MOTORVTOZ;//纵轴归位
//        break;
//    }
//}
