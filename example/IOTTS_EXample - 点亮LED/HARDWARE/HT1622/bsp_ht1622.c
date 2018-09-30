/*****************************************************************
HMS17AN段码液晶屏测试程序 Drive HT1622 CPU MSP430G2454 @ 1MHz

液晶屏内容： 8888 8888 8888 88  88 第二排
              88   88   88         第一排

*****************************************************************/
#include "bsp_ht1622.h"
#include "delay.h"

unsigned char DIS_BUF[]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18};//显示缓冲区
//段码表 0 1 2 3 4 5 6 7 8 9 A b C d E F - _ 空
const unsigned char DIS[]={0xBF,0x06,0x7C,0x5E,0xC6,0xDA,0xFA,0x0E,0xFE,0xDE,0xEE,0xF2,0xB8,0x76,0xF8,0xE8,0x40,0x10,0x00};

void HT1622_Init(void)
{
	u8  j = 0;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE); // IO口时钟使能
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;  //初始化具体IO口
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //IO 口速率
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //IO 工作模式 推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_15;  //初始化具体IO口
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //IO 口速率
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //IO 工作模式 推挽输出
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	for(j=0; j<32; j++)
	{
			DIS_BUF[j] = 0;
	}
	Sendcmd(RC32K);  //RC32k
	Sendcmd(SYSEN);  //打开系统振荡器
	Sendcmd(LCDON);  //打开 LCD偏压发生器
}
void HT1622_R(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // IO口时钟使能
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_15;  //初始化具体IO口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   //IO 工作模式 上拉输入
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
void HT1622_W(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // IO口时钟使能
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_15;  //初始化具体IO口
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  //IO 口速率
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //IO 工作模式 推挽输出
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
/**********************************************************
 * 函数名 : LCD_Wr_Clk
 * 输入   ：none
 * 输出   ：none 
 * 功能   ：写时钟
 **********************************************************/
void LCD_Wr_Clk(void) 
{ 
	HT1622_WR_SCK_L;
	delay_us(4);
	HT1622_WR_SCK_H;
	delay_us(4);
} 

/**********************************************************
 * 函数名 : LCD_Rd_Clk
 * 输入   ：none
 * 输出   ：none 
 * 功能   ：读时钟
 **********************************************************/
void LCD_Rd_Clk(void) 
{ 
	HT1622_RD_SCK_L;
	delay_us(4);
	HT1622_RD_SCK_H;
	delay_us(4);
} 
/**********************************************************
 * 函数名 : SendBit_HL
 * 输入   ：dat 数据/命令 cnt 数据位
 * 输出   ：none 
 * 功能   ：dat 的高cnt 位写入HT1622，先发送高位
 **********************************************************/
void SendBit_HL(u8 dat,u8 cnt) //
{
    u8 i;
    for(i=0; i<cnt;i++)
    {
			if(dat&0x80)
			{
					HT1622_DAT_H;
			}
			else
			{
					HT1622_DAT_L;
			}
			
			LCD_Wr_Clk();
			dat <<= 1;//数据左移1位
    }
}
/**********************************************************
 * 函数名 : SendBit_LH
 * 输入   ：dat 数据/命令 cnt 数据位
 * 输出   ：none 
 * 功能   ：dat 的低cnt 位写入HT1622，先发送低位
 **********************************************************/
void  SendBit_LH(u8  dat,u8 cnt)
{
	u8 i;
	for(i=0; i<cnt;i++)
	{
			if(dat&0x01)
			{
					HT1622_DAT_H;
			}
			else
			{
					HT1622_DAT_L;
			}
			LCD_Wr_Clk();
			dat >>= 1;//数据右移1位
	}
}
/**********************************************************
 * 函数名 : ReceiveBit_LH
 * 输入   ：
 * 输出   ：none 
 * 功能   ：读取com口状态
 **********************************************************/
u8  ReceiveBit_LH(void)
{
	u8 i;
	u8 j=0x01;
	u8 dat=0x00;
	HT1622_R();//配置成输入模式读取com口状态
	
	for(i=0;i<4;i++)
	{   
		  LCD_Rd_Clk();
			if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)==1)
			{
					dat|=j;
			}
			else
			{
					dat&=~j;
			}
			j<<=1;
	}
	return dat;
}

/**********************************************************
 * 函数名 : Sendcmd
 * 输入   ：command 命令
 * 输出   ：none 
 * 功能   ：指令模式
 **********************************************************/
void  Sendcmd(u8  command)
{
    HT1622_CS_L;
    SendBit_HL(0x80,3);     //写入标志码"100"
    SendBit_HL(command,9);  //写入 9 位数据,其中前 8 位为 command 命令,最后 1 位任意
    HT1622_CS_H;
}
/**********************************************************
 * 函数名 : W1622_q
 * 输入   ：addr 地址 dat 数据
 * 输出   ：none 
 * 功能   ：指令模式
 **********************************************************/
void W1622_q(u8 addr,u8 dat)
{
    HT1622_CS_L;
    SendBit_HL(0xa0,3);    //写入标志码"101"
    SendBit_HL(addr<<2,6); //写入 6 位 addr
    SendBit_LH(dat,4);     //写入低4位数据
    HT1622_CS_H;
}

/**********************************************************
 * 函数名 : W1622_byte
 * 输入   ：SEG 地址 dat 数据
 * 输出   ：none 
 * 功能   ：写SEG脚上所有的位，SEG( 0~31 )
 **********************************************************/
void  W1622_byte(u8 SEG,u8 dat)
{
    u8 i,addr;
    addr = SEG * 2;
    for(i=0;i<2;i++)
    {
			W1622_q(addr,dat);
			addr += 1;
			dat >>= 4;
    }
}
/**********************************************************
 * 函数名 : W1622_q
 * 输入   ：addr 地址 dat 数据
 * 输出   ：none 
 * 功能   ：指令模式
 **********************************************************/
u8 W1622_Read_byte(u8 addr)
{
	 u8 i=0;
	 u8 dat=0;
	 for(i=0;i<2;i++)
		{
			HT1622_CS_L;
			SendBit_HL(0xc0,3);    //写入标志码"110"
			SendBit_HL(addr<<2,6); //写入 6 位 addr
			dat|=ReceiveBit_LH();  //读出低4位数据
			HT1622_CS_H;
			dat<<=4;
		}
	 return dat;
}

/**********************************************************
 * 函数名 : W1622_READ_MODIFY_WRITE 
 * 输入   ：SEG 位选端 dat 需要写入的数据
 * 输出   ：none 
 * 功能   ：读写同时进行
 **********************************************************/
void W1622_READ_MODIFY_WRITE(u8 SEG,u8 dat)
{
  u8 addr;
  addr = SEG * 2;
	W1622_q(addr,dat);
	addr += 1;
	dat >>= 4;//获取写入数据高4位
	dat|=0x01;
	dat&=ReceiveBit_LH();//读取COM8口状态
	HT1622_W();//设置数据端口为输出模式
	W1622_q(addr,dat);//写入数据
}

///**********************************************************
// * 函数名 : W1622_READ_MODIFY_WRITE 
// * 输入   ：SEG 位选端 dat 需要写入的数据
// * 输出   ：none 
// * 功能   ：读写同时进行,先读取com口高八位数据改变最高位数据，再次写入数据。用于修改COM8数据显示
// **********************************************************/
//void W1622_READ_MODIFY_WRITE_2(u8 SEG,u8 dat)
//{
//  u8 addr;
//	u8 i;
//	u8 dat_1=dat;
//  addr = SEG * 2;
//	HT1622_CS_L;//芯片使能
//	for(i=0;i<7;i++)//读取并改写COM8数据
//	{
//		SendBit_HL(0xa0,3);     //写入标志码"101"
//		SendBit_HL(addr<<2,6);  //写入 6 位 addr

//		dat_1&=0x01;//取要写入数据最低位
//		HT1622_R();//数据口配置成输入模式
//		dat_1|=(ReceiveBit_LH()&0x07);        //读取COM8口状态
//		HT1622_W();//数据口配置成输出模式
//		SendBit_LH(dat,4);      //写入低4位数据
//		addr+=2;
//		dat_1=dat;
//		dat_1>>=i;
//	}
//	HT1622_CS_H;
//}
/**********************************************************
 * 函数名 : W1622_READ_MODIFY_WRITE 
 * 输入   ：SEG 位选端 dat 需要写入的数据
 * 输出   ：none 
 * 功能   ：读写同时进行,先读取com口高八位数据改变最高位数据，再次写入数据。用于修改COM8数据显示
 **********************************************************/
void W1622_READ_MODIFY_WRITE_2(u8 SEG,u8 dat)
{
  u8 addr;
	u8 dat_1=dat;
//  addr = SEG * 2;
	HT1622_CS_L;//芯片使能
	SendBit_HL(0xa0,3);     //写入标志码"101"
	SendBit_HL(9<<2,6);  //写入 6 位 addr
	dat_1&=0x01;//取要写入数据最低位
	HT1622_R();//数据口配置成输入模式
	dat_1|=(ReceiveBit_LH()&0x07);        //读取COM8口状态
	HT1622_W();//数据口配置成输出模式
	SendBit_LH(dat,4);      //写入低4位数据
	addr+=2;
	dat_1=dat;
	dat_1>>=1;
	
	SendBit_HL(0xa0,3);     //写入标志码"101"
	SendBit_HL(11<<2,6);  //写入 6 位 addr
	dat_1&=0x01;//取要写入数据最低位
	HT1622_R();//数据口配置成输入模式
	dat_1|=(ReceiveBit_LH()&0x07);        //读取COM8口状态
	HT1622_W();//数据口配置成输出模式
	SendBit_LH(dat,4);      //写入低4位数据
	addr+=2;
	dat_1=dat;
	dat_1>>=2;
	
	SendBit_HL(0xa0,3);     //写入标志码"101"
	SendBit_HL(7<<2,6);  //写入 6 位 addr
	dat_1&=0x01;//取要写入数据最低位
	HT1622_R();//数据口配置成输入模式
	dat_1|=(ReceiveBit_LH()&0x07);        //读取COM8口状态
	HT1622_W();//数据口配置成输出模式
	SendBit_LH(dat,4);      //写入低4位数据
	addr+=2;
	dat_1=dat;
	
	dat_1>>=3;
	SendBit_HL(0xa0,3);     //写入标志码"101"
	SendBit_HL(5<<2,6);  //写入 6 位 addr
	dat_1&=0x01;//取要写入数据最低位
	HT1622_R();//数据口配置成输入模式
	dat_1|=(ReceiveBit_LH()&0x07);        //读取COM8口状态
	HT1622_W();//数据口配置成输出模式
	SendBit_LH(dat,4);      //写入低4位数据
	addr+=2;
	dat_1=dat;
	dat_1>>=4;
	
	SendBit_HL(0xa0,3);     //写入标志码"101"
	SendBit_HL(13<<2,6);  //写入 6 位 addr
	dat_1&=0x01;//取要写入数据最低位
	HT1622_R();//数据口配置成输入模式
	dat_1|=(ReceiveBit_LH()&0x07);        //读取COM8口状态
	HT1622_W();//数据口配置成输出模式
	SendBit_LH(dat,4);      //写入低4位数据
	addr+=2;
	dat_1=dat;
	dat_1>>=5;
	
	SendBit_HL(0xa0,3);     //写入标志码"101"
	SendBit_HL(3<<2,6);  //写入 6 位 addr
	dat_1&=0x01;//取要写入数据最低位
	HT1622_R();//数据口配置成输入模式
	dat_1|=(ReceiveBit_LH()&0x07);        //读取COM8口状态
	HT1622_W();//数据口配置成输出模式
	SendBit_LH(dat,4);      //写入低4位数据
	addr+=2;
	dat_1=dat;
	dat_1>>=6;
	
	SendBit_HL(0xa0,3);     //写入标志码"101"
	SendBit_HL(1<<2,6);    //写入 6 位 addr
	dat_1&=0x01;//取要写入数据最低位
	HT1622_R();//数据口配置成输入模式
	dat_1|=(ReceiveBit_LH()&0x07);        //读取COM8口状态
	HT1622_W();//数据口配置成输出模式
	SendBit_LH(dat,4);      //写入低4位数据
	
	HT1622_CS_H;
}
/**********************************************************
 * 函数名 : W1622_READ_MODIFY_WRITE_3 
 * 输入   ：dat 需要写入的数据
 * 输出   ：none 
 * 功能   ：读写同时进行,先读取com口高八位数据改变最高位数据，再次写入数据。用于修改COM8数据显示
 **********************************************************/
void W1622_READ_MODIFY_WRITE_3(u8 dat)
{
  u8 addr;
	u8 dat_1=dat;
//  addr = SEG * 2;
	HT1622_CS_L;//芯片使能
	
	SendBit_HL(0xa0,3);     //写入标志码"101"
	SendBit_HL(19<<2,6);  //写入 6 位 addr
	dat_1&=0x01;//取要写入数据最低位
	HT1622_R();//数据口配置成输入模式
	dat_1|=(ReceiveBit_LH()&0x07);        //读取COM8口状态
	HT1622_W();//数据口配置成输出模式
	SendBit_LH(dat,4);      //写入低4位数据
	addr+=2;
	dat_1=dat;
	dat_1>>=1;
	SendBit_HL(0xa0,3);     //写入标志码"101"
	SendBit_HL(21<<2,6);  //写入 6 位 addr
	dat_1&=0x01;//取要写入数据最低位
	HT1622_R();//数据口配置成输入模式
	dat_1|=(ReceiveBit_LH()&0x07);        //读取COM8口状态
	HT1622_W();//数据口配置成输出模式
	SendBit_LH(dat,4);      //写入低4位数据
	addr+=2;
	dat_1=dat;
	dat_1>>=2;
	SendBit_HL(0xa0,3);     //写入标志码"101"
	SendBit_HL(17<<2,6);  //写入 6 位 addr
	dat_1&=0x01;//取要写入数据最低位
	HT1622_R();//数据口配置成输入模式
	dat_1|=(ReceiveBit_LH()&0x07);        //读取COM8口状态
	HT1622_W();//数据口配置成输出模式
	SendBit_LH(dat,4);      //写入低4位数据
	addr+=2;
	dat_1=dat;
	dat_1>>=3;
	SendBit_HL(0xa0,3);     //写入标志码"101"
	SendBit_HL(23<<2,6);  //写入 6 位 addr
	dat_1&=0x01;//取要写入数据最低位
	HT1622_R();//数据口配置成输入模式
	dat_1|=(ReceiveBit_LH()&0x07);        //读取COM8口状态
	HT1622_W();//数据口配置成输出模式
	SendBit_LH(dat,4);      //写入低4位数据
	addr+=2;
	dat_1=dat;
	dat_1>>=4;
	SendBit_HL(0xa0,3);     //写入标志码"101"
	SendBit_HL(25<<2,6);  //写入 6 位 addr
	dat_1&=0x01;//取要写入数据最低位
	HT1622_R();//数据口配置成输入模式
	dat_1|=(ReceiveBit_LH()&0x07);        //读取COM8口状态
	HT1622_W();//数据口配置成输出模式
	SendBit_LH(dat,4);      //写入低4位数据
	addr+=2;
	dat_1=dat;
	dat_1>>=5;
	SendBit_HL(0xa0,3);     //写入标志码"101"
	SendBit_HL(15<<2,6);  //写入 6 位 addr
	dat_1&=0x01;//取要写入数据最低位
	HT1622_R();//数据口配置成输入模式
	dat_1|=(ReceiveBit_LH()&0x07);        //读取COM8口状态
	HT1622_W();//数据口配置成输出模式
	SendBit_LH(dat,4);      //写入低4位数据
	addr+=2;
	dat_1=dat;
	dat_1>>=6;
	
	SendBit_HL(0xa0,3);     //写入标志码"101"
	SendBit_HL(39<<2,6);    //写入 6 位 addr

	dat_1&=0x01;//取要写入数据最低位
	HT1622_R();//数据口配置成输入模式
	dat_1|=(ReceiveBit_LH()&0x07);        //读取COM8口状态
	HT1622_W();//数据口配置成输出模式
	SendBit_LH(dat,4);      //写入低4位数据
	HT1622_CS_H;
}
/**********************************************************
 * 函数名 : W1622_READ_MODIFY_WRITE 
 * 输入   ：SEG 位选端 dat 需要写入的数据
 * 输出   ：none 
 * 功能   ：读写同时进行
 **********************************************************/
void W1622_Write_Seg(u8 SEG,u8 dat)
{
  u8 addr;
	u8 data=0x0f;
	u8 dat_1=dat;
  addr = SEG * 2;
	SendBit_HL(0xa0,3);    //写入标志码"101"
  SendBit_HL((addr+1)<<2,6); //写入 6 位 addr
	dat_1&=0x01;//获取写入数据低位
	data&=ReceiveBit_LH();//读取COM口高4位状态
	HT1622_W();       //设置数据端口为输出模式
	W1622_q(addr,dat);//写入数据
}
/**********************************************************
 * 函数名 : W1622_success
 * 输入   ：
 * 输出   ：none 
 * 功能   ：连续写单元 SEG(0~31)//单个段式数码管连续刷新
 **********************************************************/
void W1622_success(u8 SEG,u8 cnt,u8 *p)
{
    u8 i,addr;
	
    addr = SEG * 2;
    HT1622_CS_L;
    SendBit_HL(0xa0,3);    //写入标志码"101"
    SendBit_HL(addr<<2,6); //写入 6 位 addr
    for(i =0; i<cnt;i++)
    {
			SendBit_LH(*p,4);
			SendBit_LH(*p>>4,4);
			p++;
    }
    HT1622_CS_H;
}

void disp_renew(void)   //显示更新,所有8x32段全部刷新
{
    W1622_success(0,32,(u8*)DIS_BUF);
}
//com(0-7)  SEG(0-31)
void  dispON(u8 com,u8 SEG)
{
    W1622_byte(SEG,DIS[SEG]);     //去掉注释后，写完就马上显示
}

void  cleandisp(void)
{
    u8  i;
    HT1622_CS_L;           //清屏幕
    SendBit_HL(0xa0,3);    //写入标志码"101"
    SendBit_HL(0,6);       //写入 6 位 addr
    for(i =0; i <32; i++)  //连续写入数据
    {
        SendBit_LH(0,4);
        SendBit_LH(0,4);
    }
    HT1622_CS_H;
    for(i=0; i<32; i++)
    {
        DIS_BUF[i] = 0x00;    //清空显存
    }
}

void  disp_All(void)
{
    u8  i;
    HT1622_CS_L;           //全显示屏幕
    SendBit_HL(0xa0,3);    //写入标志码"101"
    SendBit_HL(0,6);       //写入 6 位 addr
    for(i =0; i <32; i ++) //连续写入数据
    {
        SendBit_LH(0xff,4);
        SendBit_LH(0xff,4);
    }
    HT1622_CS_H;
    for(i=0; i<32; i++)
    {
        DIS_BUF[i] = 0xff;    //填充显存
    }
}


