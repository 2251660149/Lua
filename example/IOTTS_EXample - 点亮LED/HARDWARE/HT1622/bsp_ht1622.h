#ifndef _HT1622_H_ 
#define _HT1622_H_  

#include "stm32f10x.h"

/*****************************************************************
管脚分配 CS P1.0 WR P1.1 DAT P1.2
*****************************************************************/
#define HT1622_PORT_RD_SCK  GPIOB
#define HT1622_PORT_WR_SCK  GPIOB
#define HT1622_PORT_DAT     GPIOB
#define HT1622_PORT_CS      GPIOB
#define HT1622_PORT_LED     GPIOC

#define HT1622_RD_CLK       GPIO_Pin_13
#define HT1622_WR_CLK       GPIO_Pin_14
#define HT1622_DAT          GPIO_Pin_15
#define HT1622_CS           GPIO_Pin_12 
#define HT1622_LED          GPIO_Pin_15 

#define HT1622_RD_SCK_L		GPIO_ResetBits(HT1622_PORT_RD_SCK, HT1622_RD_CLK)
#define HT1622_RD_SCK_H		GPIO_SetBits(HT1622_PORT_RD_SCK, HT1622_RD_CLK)

#define HT1622_WR_SCK_L		GPIO_ResetBits(HT1622_PORT_WR_SCK, HT1622_WR_CLK)
#define HT1622_WR_SCK_H		GPIO_SetBits(HT1622_PORT_WR_SCK, HT1622_WR_CLK)

#define HT1622_DAT_L		  GPIO_ResetBits(HT1622_PORT_DAT, HT1622_DAT)
#define HT1622_DAT_H		  GPIO_SetBits(HT1622_PORT_DAT, HT1622_DAT)	

#define HT1622_CS_L		    GPIO_ResetBits(HT1622_PORT_CS, HT1622_CS)	
#define HT1622_CS_H		    GPIO_SetBits(HT1622_PORT_CS, HT1622_CS)


/*****************************************************************
HT1622 指令宏定义
*****************************************************************/
#define SYSDIS   0X00 // 关振系统荡器和LCD偏压发生器
#define SYSEN    0X01 // 打开系统振荡器
#define LCDOFF   0X02 // 关LCD偏压
#define LCDON    0X03 // 打开LCD偏压
#define TIMEDIS  0x04 // 禁止时基输出
#define WDTDIS   0x05 // 禁止WDT超时标志输出
#define TIMEREN  0x08 // 启用时基输出
#define WDTEN    0x0D // 启用WDT超时标志输出
#define TONEOFF  0x0F // 关闭声音输出
#define CLRTIMER 0x05 // 清除时基发生器的内容
#define CLRWDT   0x0F // 清除WDT的内容
#define RC32K    0x18 // 系统时钟源，片上RC振荡器
#define EXT32K   0x1C // 系统时钟源，外部时钟源
#define TONE4K   0x40 // 音的输出频率：4kHz的
#define TONE2K   0x60 // 音的输出频率：2kHz的
#define IRQDIS   0x80 // 禁止IRQ输出
#define IRQEN    0x88 // 启用IRQ输出
#define F1       0xA0 // 时基时钟输出：1Hz WDT的超时标志后：4s
#define F2       0xA1 // 时基时钟输出：2Hz WDT的超时标志后：2s
#define F4       0xA2 // 时基时钟输出：4Hz WDT的超时标志后：1s
#define F8       0xA3 // 时基时钟输出：8Hz WDT的超时标志后：1/2s
#define F16      0xA4 // 时基时钟输出：16Hz WDT的超时标志后：1/4s
#define F32      0xA5 // 时基时钟输出：32Hz WDT的超时标志后：1/8s
#define F64      0xA6 // 时基时钟输出：64Hz WDT的超时标志后：1/16s
#define F128     0xA7 // 时基时钟输出：128Hz WDT的超时标志后：1/32s
#define TEST     0xE0 // 测试模式下，用户不使用。
#define NORMAL   0xE3 // 普通模式

void  Sendcmd(u8  command);
u8 W1622_Read_byte(u8 addr);
void W1622_READ_MODIFY_WRITE(u8 SEG,u8 dat);
void W1622_READ_MODIFY_WRITE_2(u8 SEG,u8 dat);
void W1622_READ_MODIFY_WRITE_3(u8 dat);

#endif
