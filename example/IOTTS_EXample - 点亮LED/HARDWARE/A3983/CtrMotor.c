#include "all_inc.h"

unsigned char RunSpeedChangeMark;
unsigned char UniformMark;
unsigned char SetMotorEnableReg;
unsigned int TMR1Temp;
unsigned char SetPwmErrMark;
unsigned char SetTimerPwmEn;
unsigned char MotorDIRReg;
unsigned char Microstep;
int RunSpeed;
int RunSpeed_last;
int RunSpeed_cur;
int RunSpeed_Add;
int RunSpeed_lastDir;

extern unsigned char PortCOutReg;
extern unsigned char PortAOutReg;
extern unsigned char Trigger;
extern unsigned char IOValue;

extern unsigned char DirChangeMark;

void MotorWork(void)
{	
	int RunSpeed_temp;	
	if(Trigger == 1)
	{
		Trigger = 0;
		SetSpeed();
	}

	SetMotorWork((unsigned int)RunSpeed,IOValue&b2,Microstep_16,IOValue);

}


void SetSpeed(void)
{	
	if(MotorDisEnable == IOValue)
	{
		RunSpeed = 0;
	}
	else
	{

		if(IOValue == 0)
		{
			RunSpeed = Speed1;
		}
		else if(IOValue == 1)
		{
			RunSpeed = Speed2;
		}
		else if(IOValue == 2)
		{
			RunSpeed = Speed3;
		}
		else if(IOValue == 3)
		{
			RunSpeed = Speed4;
		}	


		else if(IOValue == 4)
		{
			RunSpeed = Speed1;
		}
		else if(IOValue == 5)
		{
			RunSpeed = Speed2;
		}
		else if(IOValue == 6)
		{
			RunSpeed = Speed3;
		}	

	}
}

void SetMotorWork(unsigned int speed, unsigned char dir, unsigned char mic, unsigned char EN)
{

	SetPwmFre(speed);

	CtrMicrostep(mic);
	
	CtrMotorDir(dir);

	CtrMotorRun(EN);

}


void SetPwmFre(unsigned int Fre)
{
	if(Fre < 500)
	{
		SetPwmErrMark = 0;
		TMR1Temp = Fre*122 + 600;
		SetTimerPwmEn = 1;

		TMR1ON = 1;	
		TMR1IE = 1;

		CCP1CON = 0;
		T2CON = 0;		
	}
	else if(Fre < 16000)
	{
		SetPwmErrMark = 0;
		SetTimerPwmEn = 0;
		CCPR1L = 0X01;
		CCP1CON = 0X1C;
		PR2 = (125000/Fre)-1;
		T2CON = 0x06;

		TMR1ON = 0;	
		TMR1IE = 0;	
	}
	else
	{
		SetPwmErrMark = 1;
	}

}

void CtrMotorDir(unsigned char value)
{	
	if((!DirChangeMark)&&(MotorDIRReg != value))
	{
		MotorDIRReg = value;

		SetTimerPwmEn = 0;

		if(DIR_F == value)
		{
			IO_DIR_0;
		}
		else
		{
			IO_DIR_1;
		}

		SetTimerPwmEn = 1;
	}
}

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



void CtrMotorRun(unsigned char value)
{
	if(SetMotorEnableReg != value)
	{
		SetMotorEnableReg = value;
		if(MotorDisEnable != value)
		{
			IO_EN_0;
		}
		else
		{
			IO_EN_1;
		}
	}
}




