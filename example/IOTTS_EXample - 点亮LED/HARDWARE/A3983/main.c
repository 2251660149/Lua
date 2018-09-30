#include "all_inc.h"

#define uchar unsigned char
#define uint unsigned int

unsigned char PortCOutReg;
unsigned char PortAOutReg;
unsigned char PortCInReg;
unsigned char PortAInReg;
unsigned char Flag10ms;

unsigned char IOValue;
unsigned char Trigger;
unsigned char DirChangeMark;
unsigned char DirChangeMark_ZXG;

unsigned int Tmr0cnt;
unsigned int Tmr0cnt1;
extern unsigned char MotorDIRReg;
extern unsigned char SetTimerPwmEn;
extern unsigned char SetMotorEnableReg;
extern unsigned int TMR1Temp;
extern int RunSpeed;
extern int RunSpeed_cur;
extern int RunSpeed_last;
extern int RunSpeed_Add;
extern unsigned char RunSpeedChangeMark;
extern unsigned char UniformMark;


void TMR0_Init(void)
{
	TMR0 = 100;
	OPTION_REG = 0x06;
}


void TMR1Init(void)
{
    T1CON = 0x02;        
 	TMR1 = 15486;
}

void ADC_Init(void)
{
	ADCON1 = 0x03;
	FVRCON = 0xc2; 
	ADCON0 = 0x11;
}



void OSC_Init(void)
{
	OSCCON = 0x77;
	WDTCON |= 0x01;
}


void IO_Init(void)
{
	PORTA = 0;
	TRISA = 0b00011000;
	ANSELA = 0;

	PORTC = 0;
	TRISC = 0b00011000;
	ANSELC = 0;

	IO_EN_1;
}

void PWM_Init(void)
{
	TRISC5 = 0; 
	CCPR1L = 0X01;
	CCP1CON = 0x1C;
	PR2 = 0xFF;
	T2CON = 0x02;
}


void InterruptInit(void)
{
	PEIE = 1;
	TMR0IE = 1;
	TMR1IE = 1;
	SBOREN = 1;
}




void Delay(unsigned int cnt)
{
	while(cnt--);
}

void RamInit(void)
{	
	SysAlarm(Disenable);
	SetTimerPwmEn = 0;
	UniformMark = 0;
	PortAInReg = PORTA;
	PortCInReg = PORTC;
	RunSpeed_last = 0;
	RunSpeed_cur = 0;
	RunSpeed_Add = 0;	
	Trigger = 1;
}	


void main(void)
{	
	unsigned char temp;
	OSC_Init();
	IO_Init();
	PWM_Init();
	TMR0_Init();
	TMR1Init();
	InterruptInit();
	RamInit();

	GIE = 1;


	while(1)
	{
		CLRWDT();
		if(Flag10ms)
		{
			Flag10ms = 0;
			IO_InputScan(); 					
		}

		if((Trigger == 1)||((!UniformMark)&&(RunSpeedChangeMark)))
		{
			MotorWork();	
		}

	}
}



void interrupt service()
{
    if(TMR0IF==1)     
    {
	    TMR0IF=0;
		TMR0 = 100;
		Flag10ms = 1; 
		RunSpeedChangeMark = 1;
		if(++Tmr0cnt >= 100)
		{
			Tmr0cnt = 0;			
		}

		if(++Tmr0cnt1 >= 80)
		{
			Tmr0cnt1 = 0;
		}
	}		

    if(TMR1IF==1) 
    {
	    TMR1IF=0;        
        TMR1 = TMR1Temp; 		

		if(SetTimerPwmEn)
		{
			IO_PUL_1;
			delay_cycles(10);
			IO_PUL_0;
		}
	}
}

void IO_InputScan(void)
{
	unsigned char dirtmp,entmp,pultmp,IOValuetmp;
	unsigned char static cnt;
	PortAInReg = PORTA;
	PortCInReg = PORTC;
	
	entmp = (PortCInReg&b4)>>4;
	dirtmp = (PortCInReg&b3)>>3;
	pultmp = (PortAInReg&b4)>>4;

	IOValuetmp = (dirtmp<<2)+(entmp<<1)+pultmp;

	if(IOValue != IOValuetmp)
	{
		if(++cnt > 3)
		{
			cnt = 0;

			DirChangeMark = 0;


			IOValue = IOValuetmp;			
			Trigger = 1;
		}
	}
	else
	{
		cnt = 0;
	}
}

void SysAlarm(unsigned char alm)
{
	if(alm == Enable)
	{
		PortAOutReg &= ~b5;
		PORTA = PortAOutReg;
	}
	else
	{
		PortAOutReg |= b5;
		PORTA = PortAOutReg;
	}
}


void delay_cycles(unsigned int num)
{
	while(num)
	{
		num --;
	}
}


void Delay_ms(unsigned int msec)
{
    while(msec--)
    {
        delay_cycles(215);
    }
}

void Delay_s(unsigned char second)
{
    while(second--)
    {
        Delay_ms(1000);
    }
}
