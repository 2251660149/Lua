
#include "stm32f10x.h"

#define TRI_WAVE 	0  		//输出三角波
#define SIN_WAVE 	1		//输出正弦波
#define SQU_WAVE 	2		//输出方波

//***************************
//		Pin assign	   	
//		STM32			AD9833
//		GPIOB_Pin_15 		---> FSYNC
//		GPIOB_Pin_14 		---> SCK
//		GPIOB_Pin_13 		---> DAT
//		GPIOB_Pin_12		---> CS
//***************************	

	/*端口定义 */ 
	#define PORT_FSYNC	GPIOB
	#define PIN_FSYNC	GPIO_Pin_15

	#define PORT_SCK	GPIOB
	#define PIN_SCK		GPIO_Pin_14

	#define PORT_DAT	GPIOB
	#define PIN_DAT		GPIO_Pin_13

	#define PORT_CS		GPIOB
	#define PIN_CS		GPIO_Pin_12  //数字电位器片选

//****************************************************************

	#define FSYNC_0()		GPIO_ResetBits(PORT_FSYNC, PIN_FSYNC)
	#define FSYNC_1()		GPIO_SetBits(PORT_FSYNC, PIN_FSYNC)

	#define SCK_0()		GPIO_ResetBits(PORT_SCK, PIN_SCK)
	#define SCK_1()		GPIO_SetBits(PORT_SCK, PIN_SCK)

	#define DAT_0()		GPIO_ResetBits(PORT_DAT, PIN_DAT)
	#define DAT_1()		GPIO_SetBits(PORT_DAT, PIN_DAT)	

	#define CS_0()		GPIO_ResetBits(PORT_DAT, PIN_CS)	
	#define CS_1()		GPIO_SetBits(PORT_DAT, PIN_CS)	

void AD9833_WaveSeting(double frequence,unsigned int frequence_SFR,unsigned int WaveMode,unsigned int Phase );

void AD9833_Init_GPIO(void);
void AD9833_AmpSet(unsigned char amp);
void AD9833_Delay(void);
