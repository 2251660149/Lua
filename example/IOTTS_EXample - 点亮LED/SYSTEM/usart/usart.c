/**********************************************************
* @ File name -> usart.c
* @ Version   -> V1.0
* @ Date      -> 12-26-2013
* @ Brief     -> 系统串口设置相关的函数
**********************************************************/

#include "usart.h"

/**********************************************************
   加入以下代码，支持printf函数，而不需要选择use MicroLIB
**********************************************************/

//重定义fputc函数 
int fputc(int ch, FILE *f)
{
	while((USART1->SR & 0x40) == 0)
		;	//循环发送，直到发送完毕   
    USART1->DR = (uint8_t)ch;	//发送数据      
	return ch;
}
/// 重定向c库函数scanf到USART1
int fgetc(FILE *f)
{
		/* 等待串口1输入数据 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}
/**********************   end   **************************/


/**********************************************************
* 函数功能 ---> 初始化IO 串口1
* 入口参数 ---> bound：波特率	
* 返回数值 ---> none
* 功能说明 ---> none
**********************************************************/
void USARTx_Init(uint32_t bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	USART1->CR1 |= 1 << 8;	//PE中断使能
	USART1->CR1 |= 1 << 5;	//接收缓冲区非空中断使能
	
	MY_NVIC_Init(3, 3, USART1_IRQn, NVIC_PriorityGroup_2);	//中断分组2，最低优先级
	
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
		
	USART_DeInit(USART1);  //复位串口1
	
	//USART1_TX   PA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA9
   
	//USART1_RX	  PA.10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA10
  
	//USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;	//设置波特率，一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;	//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	
	USART_Init(USART1, &USART_InitStructure); //初始化串口
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
	USART_Cmd(USART1, ENABLE);                    //使能串口 
}


/**********************************************************
* 函数功能 ---> 串口1接收中断服务程序
* 入口参数 ---> bound：波特率	
* 返回数值 ---> none
* 功能说明 ---> none
**********************************************************/
void USART1_IRQHandler(void)
{
	uint8_t res;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收到数据
	{
		res = USART_ReceiveData(USART1);	//读取接收到的数据
		USART1->DR = (uint8_t)res; 		 
	}	//end 接收到数据
}


