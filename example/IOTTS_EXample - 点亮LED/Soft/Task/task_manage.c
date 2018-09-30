/**********************************************************
                       康威电子
										 
功能：stm32f103rct6控制，75MHz时钟， AD9834正弦波点频输出，范围0-30M(可编辑0-100M)，
			方波：50k-5M，三角波：0-10M，扫频默认正弦波 。
			显示：12864cog
接口：控制接口请参照AD9834.h  按键接口请参照key.h
时间：2015/11/10
版本：1.0
作者：康威电子
其他：程序借鉴正点原子，添加自己的驱动，未经作者许可，不得用于其它任何用途

更多电子需求，请到淘宝店，康威电子竭诚为您服务 ^_^
https://shop110336474.taobao.com/?spm=a230r.7195193.1997079397.2.Ic3MRJ

**********************************************************/
#include "task_manage.h"
#include "delay.h"
#include "key.h"
#include "bsp_max262.h"
#include <stdio.h>
#include <ctype.h>
#include <cstring>

#define OUT_KEY  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)//读取按键0
#define FLASH_SAVE_ADDR  0x0801F000  				//设置FLASH 保存地址(必须为偶数)


u8 Task_Index = 0;//界面切换
u8 Param_Mode = 0;//参数类型切换
u8 fre_buf[StrMax]; //参数转换字符缓存
u8 display[StrMax]; //参数显示缓存
u8 P_Index = 0; //编辑位置
u8 Task_First=1;//第一次进入标记
u8 _return=0;
//扫频参数
u32 SweepMinFre = 1000;
u32 SweepMaxFre = 10000;
u32 fc = 1000;
u8  qn = 5;


u32 f0;

u8 Task_Delay(u32 delay_time, u8* delay_ID)
{
	static u8 Time_Get = 0;
	static u32 Time_Triger;
    if(Time_Get == 0)
    {
      Time_Triger = SysTimer + delay_time;
      Time_Get = 1;
    }
    if(SysTimer >= Time_Triger)
    { 
      Time_Get = 0;
			*delay_ID = 1;		//	后续代码已被执行一遍
			return 1;
    }
		return 0;
}

//u8 TaskOneToIdel(void)
//{
//	static u8 delay_ID0 = 0;
//	static u8 delay_ID1 = 0;
//	static u8 delay_ID2 = 0;
//	static u8 delay_ID3 = 0;
//	u8 delay_arrive0 = 0;
//	u8 delay_arrive1 = 0;
//	u8 delay_arrive2 = 0;
//	u8 delay_arrive3 = 0;
//	
//	delay_arrive0 = Task_Delay(100, &delay_ID0);
//	delay_arrive1 = Task_Delay(100, &delay_ID1);
//	delay_arrive2 = Task_Delay(100, &delay_ID2);
//	delay_arrive3 = Task_Delay(100, &delay_ID3);
//	if((delay_arrive0 == 0) && (delay_ID0 == 0))
//		return 0;
//	else if((delay_arrive0) && (delay_ID0))
//	{
//			//执行本延时后代码
//	}
//		
//	
//	
//	if(delay_ID0&&delay_ID1&&delay_ID2&&delay_ID3)
//	{
//		delay_ID0 = 0;
//		delay_ID1 = 0;
//		delay_ID2 = 0;
//		delay_ID3 = 0;
//	}
//}

u8 TaskCycleDelay(u32 delay_time, u8* Last_delay_ID, u8* Self_delay_ID)
{
	static u8 Time_Get = 0;
	static u32 Time_Triger;
	
	if(!(*Last_delay_ID))
		return 0;
	if(Time_Get == 0)
	{
		Time_Triger = SysTimer + delay_time;
		Time_Get = 1;
	}
	if(SysTimer >= Time_Triger)
	{ 
		Time_Get = 0;
		*Last_delay_ID = 0;
		*Self_delay_ID = 1;		//	后续代码已被执行一遍
		return 1;
	}
	return 0;
}

//把数据放到另一个缓存，显示，FloatNum小数点位数，CursorEn光标使能
void Copybuf2dis(u8 *source, u8 dis[StrMax], u8 dispoint, u8 FloatNum, u8 CursorEn)
{
	int i, len;
	
	len = strlen(source);
	i = len - FloatNum;//整数个数
	if(FloatNum>0)dis[i] = '.';
	for (i = 0; i < len; i++)
	{
		if(i < (len-FloatNum)) dis[i] = source[i];
		else 
		{ dis[i+1] = source[i]; }
	}
	
	if(CursorEn)
	{
		if(dispoint < (len-FloatNum)) dis[dispoint] += 128;
		else dis[dispoint+1] += 128;	
	}
}
//
void Set_PointFre(u32 Key_Value, u8* Task_ID)
{
	//按键判断
	switch(Key_Value)
	{
		case K_4_S: fre_buf[P_Index]++;break;
		case K_4_L: fre_buf[P_Index]++;break;
		case K_5_L: P_Index++;break;
		case K_5_S: P_Index++;break;
		case K_1_L: P_Index--;break;
		case K_1_S: P_Index--;break;
		case K_3_S: fre_buf[P_Index]--;break;
		case K_3_L: fre_buf[P_Index]--;break;
		case K_2_S: Param_Mode++;break;
	}
	if(fre_buf[P_Index] == '/') fre_buf[P_Index] = '9';//<'0'
	if(fre_buf[P_Index] == ':') fre_buf[P_Index] = '0';//>'9'
	//界面切换
	if(Key_Value == K_2_L) 
	{	
		Task_Index++;
		LCD_Clear();
	}
	if(Key_Value == K_2_L || Key_Value == K_2_S) Task_First=1;//更新第一次进入
	if(Task_Index >= Interface) Task_Index=0;
	switch(Task_Index)
	{
		case 0: 
			Task0_PointFre(Key_Value);
			break;
		case 1: 
			Task1_Square(Key_Value);
			break;
		case 2: 
			Task2_Triangular(Key_Value);
			break;
//		case 3: 
//			Task3_SweepFre(Key_Value);
//			break;
	}
	//其他处理
	LCD_Show_CEStr(0,0,"262");	
}
//任务

void Task0_PointFre(u32 Key_Value)//正弦波 (10M) 0-100 000 000
{
	u8 showstr[StrMax]={0};
	static u8 AB_Flag;
	
	if(Task_First)
	{
		Task_First = 0;//清除第一次进入标记
		Key_Value = K_2_S;
		Param_Mode %= 3;//计算参数模式，以便装入初始值
		if(Param_Mode == 0) sprintf(fre_buf, "%6d", fc);
		if(Param_Mode == 1) sprintf(fre_buf, "%2d", qn);

		LCD_Show_CEStr(64-8*3,0,"  高通");//模式名称
		_return=1;//更新显示标志 
	}
	if(Key_Value != K_NO)
	{
		if(Param_Mode == 0)//最小频率设置
		{
			P_Index %= 6;//参数位数
			fc = atol(fre_buf);//字符转换
			if(fc>140000) fc=140000;//数据限制
			if(fc<0) fc=0;
			sprintf(fre_buf, "%6d", fc);//数据重新写入
		}
		//显示
		sprintf(showstr, "%6d", fc);//重新申请缓存显示
		fre_buf_change(showstr);//fre_buf当中 ‘ '->'0'
		if(Param_Mode == 0) Copybuf2dis(showstr, display, P_Index, 0, 1);
		else Copybuf2dis(showstr, display, P_Index, 0, 0);
		display[6]=0;//只显示3位
		OLED_ShowString(56, 2, display);
		LCD_Show_CEStr(56+48,2,"Hz");
		LCD_Show_CEStr(0,2,"截止f:");
		
		if(Param_Mode == 1)//最大频率设置
		{
			P_Index %= 2;//参数位数
			qn = atol(fre_buf);//字符转换
			if(qn>90) qn=90;//数据限制
			if(qn<0) qn=0;
			sprintf(fre_buf, "%2d", qn);//数据重新写入
		}
		//显示
		sprintf(showstr, "%2d", qn);//重新申请缓存显示
		fre_buf_change(showstr);//fre_buf当中 ‘ '->'0'
		if(Param_Mode == 1) Copybuf2dis(showstr, display, P_Index, 0, 1);
		else Copybuf2dis(showstr, display, P_Index, 0, 0);
		display[2]=0;//只显示3位
		OLED_ShowString(56, 4, display);
		LCD_Show_CEStr(0,4,"Q值  :");
		
		if(Param_Mode == 2)//频率步进设置
		{
			P_Index %= 3;//参数位数
			AB_Flag = P_Index;
			if(AB_Flag==0) 
			{
				display[0]='A'+128;
				display[1]=' ';
			}
			if(AB_Flag==1) 
			{	
				display[0]='B'+128;
				display[1]=' ';
			}
			if(AB_Flag==2) 
			{
				display[0]='A'+128;
				display[1]='B'+128;
			}
			display[2]=0;
			OLED_ShowString(56,6,display);
		}
		else
		{
			if(AB_Flag==0) 
			{
				display[0]='A';
				display[1]=' ';
			}
			if(AB_Flag==1) 
			{	
				display[0]='B';
				display[1]=' ';
			}
			if(AB_Flag==2) 
			{
				display[0]='A';
				display[1]='B';
			}
			display[2]=0;
			OLED_ShowString(56,6,display);
		}
		//显示
		LCD_Show_CEStr(0,6,"通道 :");		
		
		if(AB_Flag==0) 
		{
			f0 = lhp_WorkFclk(fc, qn, highPass, modeThree, channelA);
			PWM_Set(f0);
		}
		else if(AB_Flag==1) 
		{
			f0 = lhp_WorkFclk(fc, qn, highPass, modeThree, channelB);
			PWM_Set(f0);
		}
		else
		{
			f0 = lhp_WorkFclk(fc, qn, highPass, modeThree, channelA);
			f0 = lhp_WorkFclk(fc, qn, highPass, modeThree, channelB);
			PWM_Set(f0);
		}
		
		sprintf(showstr,"%7d", f0);
		LCD_Show_CEStr(75,6,showstr);
		
		_return=1;
	}
}

void Task1_Square(u32 Key_Value)//方波 300k
{
	u8 showstr[StrMax]={0};
	static u8 AB_Flag;
	
	if(Task_First)
	{
		Task_First = 0;//清除第一次进入标记
		Key_Value = K_2_S;
		Param_Mode %= 3;//计算参数模式，以便装入初始值
		if(Param_Mode == 0) sprintf(fre_buf, "%6d", fc);
		if(Param_Mode == 1) sprintf(fre_buf, "%2d", qn);

		LCD_Show_CEStr(64-8*3,0,"  低通");//模式名称
		_return=1;//更新显示标志 
	}
	if(Key_Value != K_NO)
	{
		if(Param_Mode == 0)//最小频率设置
		{
			P_Index %= 6;//参数位数
			fc = atol(fre_buf);//字符转换
			if(fc>140000) fc=140000;//数据限制
			if(fc<0) fc=0;
			sprintf(fre_buf, "%6d", fc);//数据重新写入
		}
		//显示
		sprintf(showstr, "%6d", fc);//重新申请缓存显示
		fre_buf_change(showstr);//fre_buf当中 ‘ '->'0'
		if(Param_Mode == 0) Copybuf2dis(showstr, display, P_Index, 0, 1);
		else Copybuf2dis(showstr, display, P_Index, 0, 0);
		display[6]=0;//只显示3位
		OLED_ShowString(56, 2, display);
		LCD_Show_CEStr(56+48,2,"Hz");
		LCD_Show_CEStr(0,2,"截止f:");
		
		if(Param_Mode == 1)//最大频率设置
		{
			P_Index %= 2;//参数位数
			qn = atol(fre_buf);//字符转换
			if(qn>90) qn=90;//数据限制
			if(qn<0) qn=0;
			sprintf(fre_buf, "%2d", qn);//数据重新写入
		}
		//显示
		sprintf(showstr, "%2d", qn);//重新申请缓存显示
		fre_buf_change(showstr);//fre_buf当中 ‘ '->'0'
		if(Param_Mode == 1) Copybuf2dis(showstr, display, P_Index, 0, 1);
		else Copybuf2dis(showstr, display, P_Index, 0, 0);
		display[2]=0;//只显示3位
		OLED_ShowString(56, 4, display);
		LCD_Show_CEStr(0,4,"Q值  :");
		
		if(Param_Mode == 2)//频率步进设置
		{
			P_Index %= 3;//参数位数
			AB_Flag = P_Index;
			if(AB_Flag==0) 
			{
				display[0]='A'+128;
				display[1]=' ';
			}
			if(AB_Flag==1) 
			{	
				display[0]='B'+128;
				display[1]=' ';
			}
			if(AB_Flag==2) 
			{
				display[0]='A'+128;
				display[1]='B'+128;
			}
			display[2]=0;
			OLED_ShowString(56,6,display);
		}
		else
		{
			if(AB_Flag==0) 
			{
				display[0]='A';
				display[1]=' ';
			}
			if(AB_Flag==1) 
			{	
				display[0]='B';
				display[1]=' ';
			}
			if(AB_Flag==2) 
			{
				display[0]='A';
				display[1]='B';
			}
			display[2]=0;
			OLED_ShowString(56,6,display);
		}
		//显示
		LCD_Show_CEStr(0,6,"通道 :");		
		
		if(AB_Flag==0) 
		{
			f0 = lhp_WorkFclk(fc, qn, lowPass, modeTwo, channelA);
			PWM_Set(f0);
		}
		else if(AB_Flag==1) 
		{
			f0 = lhp_WorkFclk(fc, qn, lowPass, modeTwo, channelB);
			PWM_Set(f0);
		}
		else
		{
			f0 = lhp_WorkFclk(fc, qn, lowPass, modeTwo, channelA); 
			f0 = lhp_WorkFclk(fc, qn, lowPass, modeTwo, channelB);
			PWM_Set(f0);
		}
		
		sprintf(showstr,"%7d", f0);
		LCD_Show_CEStr(75,6,showstr);
		
		_return=1;
	}
}
void Task2_Triangular(u32 Key_Value)//三角波 1M
{	
	static u8 AB_Flag;
	u8 showstr[StrMax]={0};
	
	if(Task_First)
	{
		Task_First = 0;//清除第一次进入标记
		Key_Value = K_2_S;
		Param_Mode %= 3;//计算参数模式，以便装入初始值
		if(Param_Mode == 0) sprintf(fre_buf, "%6d", SweepMinFre);
		if(Param_Mode == 1) sprintf(fre_buf, "%6d", SweepMaxFre);

		LCD_Show_CEStr(64-8*3,0,"  带通");//模式名称
		_return=1;//更新显示标志 
	}
	if(Key_Value != K_NO)
	{
		if(Param_Mode == 0)//最小频率设置
		{
			P_Index %= 6;//参数位数
			SweepMinFre = atol(fre_buf);//字符转换
			if(SweepMinFre>140000) SweepMinFre=140000;//数据限制
			if(SweepMinFre<0) SweepMinFre=0;
			sprintf(fre_buf, "%6d", SweepMinFre);//数据重新写入
		}
		//显示
		sprintf(showstr, "%6d", SweepMinFre);//重新申请缓存显示
		fre_buf_change(showstr);//fre_buf当中 ‘ '->'0'
		if(Param_Mode == 0) Copybuf2dis(showstr, display, P_Index, 0, 1);
		else Copybuf2dis(showstr, display, P_Index, 0, 0);
		OLED_ShowString(56, 2, display);
		LCD_Show_CEStr(56+48,2,"Hz");
		LCD_Show_CEStr(0,2,"开始f:");
		
		if(Param_Mode == 1)//最大频率设置
		{
			P_Index %= 6;//参数位数
			SweepMaxFre = atol(fre_buf);//字符转换
			if(SweepMaxFre>140000) SweepMaxFre=140000;//数据限制
			if(SweepMaxFre<0) SweepMaxFre=0;
			sprintf(fre_buf, "%6d", SweepMaxFre);//数据重新写入
		}
		//显示
		sprintf(showstr, "%6d", SweepMaxFre);//重新申请缓存显示
		fre_buf_change(showstr);//fre_buf当中 ‘ '->'0'
		if(Param_Mode == 1) Copybuf2dis(showstr, display, P_Index, 0, 1);
		else Copybuf2dis(showstr, display, P_Index, 0, 0);
		OLED_ShowString(56, 4, display);
		LCD_Show_CEStr(56+48, 4,"Hz");
		LCD_Show_CEStr(0,4,"截止f:");
		
				if(Param_Mode == 2)//频率步进设置
		{
			P_Index %= 3;//参数位数
			AB_Flag = P_Index;
			if(AB_Flag==0) 
			{
				display[0]='A'+128;
				display[1]=' ';
			}
			if(AB_Flag==1) 
			{	
				display[0]='B'+128;
				display[1]=' ';
			}
			if(AB_Flag==2) 
			{
				display[0]='A'+128;
				display[1]='B'+128;
			}
			display[2]=0;
			OLED_ShowString(56,6,display);
		}
		else
		{
			if(AB_Flag==0) 
			{
				display[0]='A';
				display[1]=' ';
			}
			if(AB_Flag==1) 
			{	
				display[0]='B';
				display[1]=' ';
			}
			if(AB_Flag==2) 
			{
				display[0]='A';
				display[1]='B';
			}
			display[2]=0;
			OLED_ShowString(56,6,display);
		}
		//显示
		LCD_Show_CEStr(0,6,"通道 :");		
		
		if(AB_Flag==0) 
		{
			f0 = bp_WorkFclk(SweepMinFre, SweepMaxFre, modeOne, channelA);
			PWM_Set(f0);
		}
		else if(AB_Flag==1) 
		{
			f0 = bp_WorkFclk(SweepMinFre, SweepMaxFre, modeOne, channelB);
			PWM_Set(f0);
		}
		else
		{
			f0 = bp_WorkFclk(SweepMinFre, SweepMaxFre, modeOne, channelA);
			f0 = bp_WorkFclk(SweepMinFre, SweepMaxFre, modeOne, channelB);
			PWM_Set(f0);
		} 
		
		sprintf(showstr,"%7d", f0);
		LCD_Show_CEStr(75,6,showstr);
		
		_return=1;
	}
}

void Task3_SweepFre(u32 Key_Value)//扫频
{}

	
 
//
void fre_buf_change(u8 *strbuf)
{
	int i;
	for (i = 0 ; i < strlen(strbuf); i++)
	 if(strbuf[i]==0x20) strbuf[i] = '0';
	for (i = 0 ; i < strlen(fre_buf); i++)
	 if(fre_buf[i]==0x20) fre_buf[i] = '0';
}
/*
每一项显示数据的位数不同的话，
1，P_Index %= 3;
2，sprintf(fre_buf, "%3d", SweepTime);
3，display[3]=0;//只显示3位


*/


