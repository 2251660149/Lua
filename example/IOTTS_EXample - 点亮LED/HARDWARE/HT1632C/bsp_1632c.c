/**********************************************************
* @ File name -> bsp_1632c.c
* @ Version   -> V1.0.1
* @ Date      -> 08-06-2017
* @ Brief     -> LED显示驱动
* @ Revise    -> 
***********************************************************/
#include "bsp_1632c.h"

//数码管段选 dp-a方式:0-9,A,b,c,u
unsigned char tab[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x3e};
//数码管段选0-9，带小数点
unsigned char tab1[] = {0xbf, 0x86, 0xdb, 0xcf, 0xe6, 0xed, 0xfd, 0x17, 0xff, 0xef};
/****************************************
*函数名:HT1632C_Writer
*描    述:HT1632C发送数据函数，高位在前
*参    数:Data,cnt
*返    回:
****************************************/
void HT1632C_Writer(unsigned char Data,unsigned char cnt)     
{
    unsigned char i;
    for(i=0;i<cnt;i++)
    {
        GPIOEX_BitClr(HT_WR);
        if(Data&0x80)
            GPIOEX_BitSet(HT_DATA);
        else
            GPIOEX_BitClr(HT_DATA);
        Data<<=1;
        GPIOEX_BitSet(HT_WR);
    }
}
/*******************************
*函数名:HT1632C_Writer_CMD
*描    述:发送命令函数
*参    数:cmd
*返    回:
********************************/
void HT1632C_Writer_CMD(unsigned char cmd)                   
{
    GPIOEX_BitClr(HT_CS);                                             
    HT1632C_Writer(0x80,3);                                    //命令码100
    HT1632C_Writer(cmd,9);                                     //命令数据，第9位为0
    GPIOEX_BitSet(HT_CS);
}
/*******************************
*函数名:HT1632C_Writer_DATA
*描    述:指定地址写入数据
*参    数:Addr,Data
*返    回:
********************************/
void HT1632C_Writer_DATA(unsigned char Addr,unsigned char Data)
{
    GPIOEX_BitClr(HT_CS);
    HT1632C_Writer(0xa0,3);                                    //命令码101
    HT1632C_Writer(Addr<<1,7);                                  //写入7位地址
    HT1632C_Writer(Data,4);                                    //写入4位数据
    GPIOEX_BitSet(HT_CS);   
}
/************************************************************
*函数名:HT1632C_Writer_AllDATA
*描    述:指定地址写入连续数据，cnt为写入数据个数，按8位算
*参    数:Addr,*p,cnt
*返    回:
*************************************************************/
void HT1632C_Writer_AllDATA(unsigned char Addr,unsigned char *p,unsigned char cnt)
{
    unsigned char i;
    GPIOEX_BitClr(HT_CS);   
    HT1632C_Writer(0xa0,3);
    HT1632C_Writer(Addr<<1,7);
    for(i=0;i<cnt;i++)
    {
        HT1632C_Writer(*p,8);
        p++;
    }
    GPIOEX_BitSet(HT_CS);
}  
/*******************************
*函数名:HT1632C_clr
*描    述:清屏函数
*参    数:
*返    回:
********************************/
void HT1632C_clr(void)  
{
    unsigned char i;
    GPIOEX_BitClr(HT_CS);
    HT1632C_Writer(0xa0,3);
    HT1632C_Writer(0x00<<1,7);
    for(i=0;i<36;i++)
    {
        HT1632C_Writer(0,8);            
    }
    GPIOEX_BitSet(HT_CS);
} 
/*******************************
*函数名:HT1632C_Init
*描    述:HT1632C初始化函数
*参    数:
*返    回:
********************************/
void HT1632C_Init(void)     
{
    GPIOEX_BitSet(HT_CS);
    GPIOEX_BitSet(HT_WR);
    GPIOEX_BitSet(HT_DATA);
     
    HT1632C_Writer_CMD(SYS_DIS);
    HT1632C_Writer_CMD(COM_OPTION);
    HT1632C_Writer_CMD(RC_MASTER_MODE);
    HT1632C_Writer_CMD(SYS_EN);
    HT1632C_Writer_CMD(PWM_DUTY);
    HT1632C_Writer_CMD(BLINK_OFF);
    HT1632C_Writer_CMD(LED_ON);
}
/*******************************
*函数名:display_num
*描    述:数码管显示函数
*参    数:
*返    回:
********************************/
//显示数值，无小数点
void display_num(unsigned int dat)
{
    unsigned char i, j;
    unsigned char dt;//dt为要写入到ram的值
    dt = 0;
    for(i=0; i<8;i++)
    {
       dt =( tab[dat]&(1<<i))<<(7-i);//千
        if(i==7)
        {
            dt +=(tab[dat%1000/100 ]&(1<<i))>>(i-6);//百
            dt += (tab[dat%100/10 ]&(1<<i))>>(i-5);//十
            dt += (tab[dat%10]&(1<<i))>>(i-4);   //个
        }
        else if(i == 6)
        {
            dt +=(tab[dat%1000/100 ]&(1<<i))>>(i-6);
            dt += (tab[dat%100/10]&(1<<i))>>(i-5);
            dt += (tab[dat%10]&(1<<i))>>(i-4);   
        }
        else if(i == 5)
      {
          dt+=(tab[dat%1000/100 ]&(1<<i))<<(6-i);//百 
          dt += (tab[dat%100/10]&(1<<i))<<(5-i);
          dt += (tab[dat%10]&(1<<i))>>(i-4);  
      }
        else
       { 
          dt+=(tab[dat%1000/100 ]&(1<<i))<<(6-i);//百
          dt += (tab[dat%100/10]&(1<<i))<<(5-i);
          dt += (tab[dat%10]&(1<<i))<<(4-i);       
      }
        HT1632C_Writer_DATA((0x03+i*4),dt);
    }
}