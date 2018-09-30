#ifndef _MCP41010_H_ 
#define _MCP41010_H_  
#include "stm32f10x.h"
//#include "sys.h"

/*数字电位器选通引脚*/
#define MCP41010_PORT_CS1  GPIOA
#define MCP41010_PORT_CS2  GPIOC
#define MCP41010_PORT_CS3  GPIOC 
#define MCP41010_PORT_CS4  GPIOC
#define MCP41010_PORT_CS5  GPIOC

#define MCP41010_CS1      GPIO_Pin_0
#define MCP41010_CS2      GPIO_Pin_1
#define MCP41010_CS3      GPIO_Pin_2
#define MCP41010_CS4      GPIO_Pin_3
#define MCP41010_CS5      GPIO_Pin_0

/*数字电位器时钟引脚*/
#define MCP41010_PORT_SCK  GPIOA
#define MCP41010_CLK       GPIO_Pin_5 

/*数字电位器数据引脚*/
#define MCP41010_PORT_SI   GPIOA
#define MCP41010_SI        GPIO_Pin_7 


#define SCK_L()		GPIO_ResetBits(MCP41010_PORT_SCK, MCP41010_CLK)
#define SCK_H()		GPIO_SetBits(MCP41010_PORT_SCK, MCP41010_CLK)

#define DAT_L()		GPIO_ResetBits(MCP41010_PORT_SI, MCP41010_SI)
#define DAT_H()		GPIO_SetBits(MCP41010_PORT_SI, MCP41010_SI)	

#define CS1_L()		GPIO_ResetBits(MCP41010_PORT_CS1, MCP41010_CS1)	
#define CS1_H()		GPIO_SetBits(MCP41010_PORT_CS1, MCP41010_CS1)

#define CS2_L()		GPIO_ResetBits(MCP41010_PORT_CS2, MCP41010_CS2)	
#define CS2_H()		GPIO_SetBits(MCP41010_PORT_CS2, MCP41010_CS2)

#define CS3_L()		GPIO_ResetBits(MCP41010_PORT_CS3, MCP41010_CS3)	
#define CS3_H()		GPIO_SetBits(MCP41010_PORT_CS3, MCP41010_CS3)

#define CS4_L()		GPIO_ResetBits(MCP41010_PORT_CS4, MCP41010_CS4)	
#define CS4_H()		GPIO_SetBits(MCP41010_PORT_CS4, MCP41010_CS4)

#define CS5_L()		GPIO_ResetBits(MCP41010_PORT_CS5, MCP41010_CS5)	
#define CS5_H()		GPIO_SetBits(MCP41010_PORT_CS5, MCP41010_CS5)

 extern enum {modeCS1=0,modeCS2,modeCS3,modeCS4,modeCS5} csMode;
 
 void MCP41010_Init(void);
 void MCP41010_AmpSet(uint8_t amp,uint8_t csx);
#endif

