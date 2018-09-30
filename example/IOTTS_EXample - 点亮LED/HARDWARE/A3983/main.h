#ifndef MAIN_H
#define MAIN_H


#define Speed1 	600
#define Speed2 	800
#define Speed3 	1000
#define Speed4 	1200 






#define     Enable				 1
#define     Disenable			 0


#define b7			0x80		/*Î»ÆÁ±Î7 */
#define b6			0x40		/*Î»ÆÁ±Î6 */
#define b5			0x20		/*Î»ÆÁ±Î5 */
#define b4			0x10		/*Î»ÆÁ±Î4 */
#define b3			0x08		/*Î»ÆÁ±Î3 */
#define b2			0x04		/*Î»ÆÁ±Î2 */
#define b1			0x02		/*Î»ÆÁ±Î1 */
#define b0			0x01		/*Î»ÆÁ±Î0 */




#define		IO_MS1_0 	(PortCOutReg&=~b1,PORTC=PortCOutReg)
#define 	IO_MS1_1 	(PortCOutReg|=b1,PORTC=PortCOutReg)
#define 	IO_MS2_0 	(PortCOutReg&=~b0,PORTC=PortCOutReg)
#define 	IO_MS2_1 	(PortCOutReg|=b0,PORTC=PortCOutReg)

#define		IO_EN_0 	(PortCOutReg&=~b2,PORTC=PortCOutReg)
#define 	IO_EN_1 	(PortCOutReg|=b2,PORTC=PortCOutReg)
#define 	IO_PUL_0 	(PortCOutReg&=~b5,PORTC=PortCOutReg)
#define 	IO_PUL_1 	(PortCOutReg|=b5,PORTC=PortCOutReg)
#define 	IO_DIR_0 	(PortAOutReg&=~b2,PORTA=PortAOutReg)
#define 	IO_DIR_1 	(PortAOutReg|=b2,PORTA=PortAOutReg)

#define 	IO_ALM_0 	(PortAOutReg&=~b5,PORTA=PortAOutReg)
#define 	IO_ALM_1 	(PortAOutReg|=b5,PORTA=PortAOutReg)

#define     IO_M1_0     (PortAOutReg&=~b0,PORTA=PortAOutReg)
#define     IO_M1_1     (PortAOutReg|=b0,PORTA=PortAOutReg)

#define     IO_M2_0     (PortAOutReg&=~b1,PORTA=PortAOutReg)
#define     IO_M2_1     (PortAOutReg|=b1,PORTA=PortAOutReg)


extern void delay_cycles(unsigned int num);
extern void Delay_ms(unsigned int msec);
extern void Delay_s(unsigned char second);

void Eeprom_WriteByte(unsigned char addr, unsigned char data);
unsigned char Eeprom_ReadByte(unsigned char addr);
void Eeprom_WriteWord(unsigned char addr, unsigned int Wdata);
unsigned int Eeprom_ReadWord(unsigned char addr);
void Init_eeprom(void);
void SysAlarm(unsigned char alm);
void IO_InputScan(void);


#endif