//指令表
#define SYS_DIS 0x00                //关闭系统震荡
#define SYS_EN  0x01                //打开系统震荡
#define LED_OFF 0x02                //关闭LED显示
#define LED_ON  0x03                //打开LED显示
#define BLINK_OFF   0x08            //关闭闪烁
#define BLINK_ON    0x09            //打开闪烁
#define SLAVE_MODE  0x10            //从模式
#define RC_MASTER_MODE  0x18        //内部RC时钟
#define EXT_CLK_MASTER_MODE 0x1C    //外部时钟
#define COM_OPTION  0x24            //16COM，NMOS模式
//#define PWM_DUTY  0XA0            //PWM亮度控制
#define PWM_DUTY    0xAf            //PWM亮度控制

/****************************************
*函数名:HT1632C_Writer
*描    述:HT1632C发送数据函数，高位在前
*参    数:Data,cnt
*返    回:
****************************************/
void HT1632C_Writer(unsigned char Data,unsigned char cnt);     

/*******************************
*函数名:HT1632C_Writer_CMD
*描    述:发送命令函数
*参    数:cmd
*返    回:
********************************/
void HT1632C_Writer_CMD(unsigned char cmd);                   

/*******************************
*函数名:HT1632C_Writer_DATA
*描    述:指定地址写入数据
*参    数:Addr,Data
*返    回:
********************************/
void HT1632C_Writer_DATA(unsigned char Addr,unsigned char Data);

/************************************************************
*函数名:HT1632C_Writer_AllDATA
*描    述:指定地址写入连续数据，cnt为写入数据个数，按8位算
*参    数:Addr,*p,cnt
*返    回:
*************************************************************/
void HT1632C_Writer_AllDATA(unsigned char Addr,unsigned char *p,unsigned char cnt);

/*******************************
*函数名:HT1632C_clr
*描    述:清屏函数
*参    数:
*返    回:
********************************/
void HT1632C_clr(void);  

/*******************************
*函数名:HT1632C_Init
*描    述:HT1632C初始化函数
*参    数:
*返    回:
********************************/
void HT1632C_Init(void);     

/*******************************
*函数名:display_num
*描    述:数码管显示函数
*参    数:
*返    回:
********************************/
//显示数值，无小数点
void display_num(unsigned int dat);
