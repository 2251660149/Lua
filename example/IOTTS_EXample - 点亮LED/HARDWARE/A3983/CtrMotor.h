#ifndef CTRMOTOR_H
#define CTRMOTOR_H


#define RunSpeed_Min 100


#define		MotorDisEnable       0b00000111

 
#define	    DIR_Z  			 	1  
#define	    DIR_F	 		 	0  


#define		Microstep_1			 1
#define		Microstep_2			 2
#define		Microstep_4			 4
#define		Microstep_16		 16

#define	    MaxFrequency1		 10000
#define	    MaxFrequency2		 10000
#define	    MaxFrequency4		 10000
#define	    MaxFrequency16		 10000

void SetMotorWork(unsigned int speed, unsigned char dir, unsigned char mic, unsigned char EN);
void SetPwmFre(unsigned int Fre);
void CtrMotorDir(unsigned char value);
void CtrMotorRun(unsigned char value);
void SetSpeed(void);
void CtrMicrostep(unsigned char mic);

void MotorWork(void);
void SpeedRamInit(int SpeedValue);
#endif