/**********************************************************
* @ File name -> main.c
* @ Version   -> V1.0.1
* @ Date      -> 02-06-2017
* @ Brief     -> 
* @ Revise    -> 
***********************************************************/
#include "stm32_config.h"
#include "stdio.h"
#include "bsp_led.h"
#include "time.h"
#include "usart.h"
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include "stdlib.h"
#include "bsp_74h595.h"
#include "pro_logic.h"
#include "bsp_74h165.h"
#include "bsp_timer.h"
#include "bsp_adc.h"

/* 测试的Lua代码字符串 */

const char lua_test[] = { 

    "print(\"Hello,I am Jack lu!\\n--this is newline printf\")\n"
    "function foo()\n"
    "  local i = 0\n"
    "  local sum = 1\n"
    "  while i <= 10 do\n"
    "    sum = sum * 2\n"
    "    i = i + 1\n"
    "  end\n"
    "return sum\n"
    "end\n"
    "print(\"sum =\", foo())\n"
    "print(\"and sum = 2^11 =\", 2 ^ 11)\n"
    "print(\"exp(200) =\", math.exp(200))\n"

};

const char lua_test1[] = { 
"print(\"sum =\", _VERSION)    "
"off = 1000  "
"on = 1000    "
"k1=0.0"
"k2=0.0"
"io_out(7,1)"
/**/
"while 1 do "
"print(\"AD值是\",ad_in(4,1))"
"print(\"IO输出值是\",io_out(22,1))\n"
	/*1轴归位*/
"while(io_in(3,0)==0.0)"
"	do"
"	pwm_out(7,10,5,30000,1)"
"	end"
"	pwm_out(7,10,5,0,1)"
	/*2轴归位*/
"while(io_in(3,0)==0.0)"
"	do"
"	pwm_out(7,10,5,30000,1)"
"	end"
"	pwm_out(7,10,5,0,1)"
	/*3轴归位*/
"while(io_in(3,0)==0.0)"
"	do"
"	pwm_out(7,10,5,30000,1)"
"	end"
"	pwm_out(7,10,5,0,1)"
	/*4轴归位*/
"while(io_in(3,0)==0.0)"
"	do"
"	pwm_out(7,10,5,30000,1)"
"	end"
"	pwm_out(7,10,5,0,1)"
	/*等待按键按下*/
//"while(io_in(3,0)==0.0)"
//"	do"
//"	pwm_out(7,10,5,30000,1)"
//"	end"
	/*检测是否插入竹签*/
"while(io_in(3,0)==0.0)"
"	do"
"	pwm_out(7,10,5,30000,1)"
"	end"
	/*1轴动作*/
"	pwm_out(7,10,5,30000,1)"
	/*2轴动作*/
"	pwm_out(7,10,5,30000,1)"
	/*3轴动作*/
"	pwm_out(7,10,5,30000,1)"
	/*打开出糖器电机*/
"print(\"IO输出值是\",io_out(22,1))\n"
	/*打开加热器*///检测PT100值,温度达到后加入糖
"print(\"IO输出值是\",io_out(22,1))\n"
	/*打开出糖电磁铁*///检测重量
"print(\"IO输出值是\",io_out(22,1))\n"

	/*4轴动作*/
"	pwm_out(7,10,5,30000,1)"

//"	while( (k1==0.0 and k2==0.0 ))"
//"	do"
//"	k1=pwm_out(7,10,5,30000,1)"
//"	k2=pwm_out(0,10,5,30000,1)"
//"	print(\"k1\",k1)"
//"	print(\"k2\",k2)"
//"	end"
"	led_off() "
"delay(off)    "
//"print(\"AD值是\",io_out(22,0))\n"
"led_on()        "
"delay(on)      "
"end"
};

time_t time(time_t * time)
{
    return 0;
}

void exit(int status)
{
  
}

int system(const char * string)
{
    return 0;
}

static int lua_led_on(lua_State *L)
{
         GPIO_ResetBits(GPIOE,GPIO_Pin_3); 
         return 1;
}
 
static int lua_led_off(lua_State *L)
{
         GPIO_SetBits(GPIOE,GPIO_Pin_3);
         return 1;
}
 
static int lua_delay(lua_State *L)
{
         int num;
         num= lua_tonumber(L, -1);
         delay_ms(num);
         return 1;
}
static int lua_ad(lua_State *L)
{
	 ADC_Channel_Sel ADC_In_Sel;
	 SOFT_Adc_Module_Input value;
	 SOFT_Adc_Module_Output out_value; 
	 ADC_In_Sel= lua_tonumber(L, 1);
	 value.ADC_ChannelInput_En=lua_tonumber(L, 2);
	 out_value=ADC_Manage(ADC_In_Sel,value);
	 lua_pushnumber(L,out_value.ADC_Channel_Data);
   return 1;
}
static int lua_io_out(lua_State *L)
{
	 int IO_Channel_Sel;
	 SOFT_O_Module_Input value;
	 IO_Channel_Sel= lua_tonumber(L, 1);
      value.IO_Channel_En=1;
	 value.IO_Channel_status=lua_tonumber(L, 2);
	 IO_Output_Manage(IO_Channel_Sel, value);
	 lua_pushnumber(L,value.IO_Channel_status);
	 return 1;
}
static int lua_io_in(lua_State *L)
{
	int IO_Channel_Sel;
	SOFT_I_Module_Input value;
	SOFT_IO_Module_Output out_value;
	IO_Channel_Sel= lua_tonumber(L, 1);
	value.IO_Channel_En=1;
	out_value.IO_Channel_status=lua_tonumber(L, 2);
	out_value=IO_Input_Manage(IO_Channel_Sel,value);
	lua_pushnumber(L,out_value.IO_Channel_status);
	return 1;
}
static int lua_pwm_out(lua_State *L)
{
	SOFT_Module_PwmOut_Input pwmout_value;
	SOFT_Module_PwmOut_Output value;
	static u8 i=1;
	u8 data[10];
	u8 IO_Channel_Sel;
	IO_Channel_Sel=lua_tonumber(L, 1);
	pwmout_value.PWM_OutputChannelt_Fre=lua_tonumber(L, 2);
 	pwmout_value.PWM_OutputChannel_Duty_Cycle= lua_tonumber(L, 3);
	pwmout_value.PWM_OutputNumCounter=lua_tonumber(L, 4);
	pwmout_value.PWM_OutputChannel_En=lua_tonumber(L, 5);
	value=PWM_OutManage(IO_Channel_Sel,pwmout_value,1);
	lua_pushnumber(L,value.PWM_Count);
	return 1;
}

const char* LED;
static const struct luaL_Reg mylib[] =
{
	{"led_on",lua_led_on},
	{"led_off",lua_led_off},
	{"delay",lua_delay},
	{"ad_in",lua_ad},
	{"io_out",lua_io_out},
	{"io_in",lua_io_in},
	{"pwm_out",lua_pwm_out},
  {NULL,NULL}
};
 
int luaopen_mylib(lua_State *L)
{ 
#if 1
     luaL_setfuncs(L,mylib, 0);
	 lua_setglobal(L,"LED");
#else
     luaL_newlib(L,mylib);
#endif
     return 1;
}
 
static int do_file_script(void)
{
    lua_State *L;
 
    L = luaL_newstate(); /* 建立Lua运行环境 */
    luaL_openlibs(L);
    luaopen_base(L);
    luaL_dostring(L, lua_test1); /* 运行Lua脚本 */
    lua_close(L);
    return 0;
}
static int do_file_script_1(void)
{
	lua_State *L;
	int ret = 0;
	L =luaL_newstate();/* 建立Lua运行环境 */
	if(L == NULL)
		abort();
	luaopen_base(L);
	luaopen_mylib(L);//注册C库

	ret= luaL_dostring(L, lua_test1);/* 运行Lua脚本 */
	if (ret != 0)
		abort();

	lua_close(L);

	abort();
}
 /**********************************************************
 * 函数名 : main
 * 输入   ：none
 * 输出   ：none 
 * 功能   ：
 **********************************************************/
int main(void)
{
	
	delay_init(72);
	LED_Init();	        //LED接口初始化
	USARTx_Init(115200);
	H595_Init();
	H165_Init();
	BSP_ADC_Init();
//  do_file_script();
  TIM5_PWM_Init(899,72); 		//不分频 PWM频率=72000/(899+1)=80Khz
	TIM4_PWM_Init(719,1);    //10分频 PWM频率=72000/10/(719+1)=10Khz /100us
	TIM6_Config(71,10);       //10分频 PWM频率=72000/10/(71+1)=100Khz /10us
  PWM_Out_Init();
	
	TIMx_Cap_Init(TIM1,0XFFFF,72-1);	//以1Mhz的频率计数 
	TIMx_Cap_Init(TIM2,0XFFFF,72-1);	//以1Mhz的频率计数 
	TIMx_Cap_Init(TIM3,0XFFFF,72-1);	//以1Mhz的频率计数 
	TIMx_Cap_Init(TIM8,0XFFFF,72-1);	//以1Mhz的频率计数 
	PWM_Out_Init();
	PWM_init();
	
	do_file_script_1();
	while(1)
	{
//		GPIO_SetBits(GPIOE,GPIO_Pin_5);
//		delay_ms(200);
//    GPIO_ResetBits(GPIOE,GPIO_Pin_5);
//		delay_ms(200);
	}
}

