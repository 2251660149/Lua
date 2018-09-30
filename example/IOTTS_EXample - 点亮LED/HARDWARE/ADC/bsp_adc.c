/**
  ******************************************************************************
  * @file    bsp_adc.c
  * @author  Jack Lu
  * @version V1.0
  * @date    2017-12-28
  * @brief   
  ******************************************************************************
	*/
	
#include "bsp_adc.h"
#include "bsp_usart1.h"
/********************************
 * 函数名：BSP_ADC_Init
 * 描述  ：AD初始化函数
 * 输入  ：无
 * 输出  ：无
 *********************************/
uint16_t ADC_ConvertedValue[400];

void BSP_ADC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	/* Enable DMA clock *//* Enable ADC1 and GPIOC clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
	
	/* Configure AD pin  as analog input */
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	/* DMA channel1 configuration */
	DMA_DeInit(DMA1_Channel1);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;	 			//ADC地址
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;	//内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 400;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;						//外设地址固定
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  										//内存地址增量
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//半字
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;															//循环传输
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	/* Enable DMA channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE);
	
	/* ADC1 configuration */	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;			//独立ADC模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 	 				//禁止扫描模式，扫描模式用于多通道采集
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;			//开启连续转换模式，即不停地进行ADC转换
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//不使用外部触发转换
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 	//采集数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 8;	 								//要转换的通道数目8
	ADC_Init(ADC1, &ADC_InitStructure);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);    //配置ADC时钟，为PCLK2的8分频，即9MHz
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_239Cycles5);     //配置ADC1的通道11为55.	5个采样周期，序列为1 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 2, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 3, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 4, ADC_SampleTime_239Cycles5);     //配置ADC1的通道11为55.	5个采样周期，序列为1 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 5, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 6, ADC_SampleTime_239Cycles5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 7, ADC_SampleTime_239Cycles5);     //配置ADC1的通道11为55.	5个采样周期，序列为1 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 8, ADC_SampleTime_239Cycles5);
	
	ADC_DMACmd(ADC1, ENABLE);     // Enable ADC1 DMA 
	ADC_Cmd(ADC1, ENABLE);        // Enable ADC1   
	ADC_ResetCalibration(ADC1);   //复位校准寄存器	
	while(ADC_GetResetCalibrationStatus(ADC1));     //等待校准寄存器复位完成 
	ADC_StartCalibration(ADC1);   // ADC校准	
	while(ADC_GetCalibrationStatus(ADC1));      // 等待校准完成	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);     // 由于没有采用外部触发，所以使用软件触发ADC转换 
}

/********************************
 * 函数名：BSP_GetAdcValue
 * 描述  ：获取（AD）
 * 输入  ：无
 * 输出  ：无
 *********************************/
uint16_t BSP_GetAdcValue(void)
{ 
	u32 BattNum=0;
	uint32_t i,ADC_VAL=0;
	float ADC_ConvertedValueLocal; 
	
	for(i=0;i<400;i=i+8)BattNum+=ADC_ConvertedValue[i];
	ADC_VAL=BattNum/50;
  printf("\r\nADC0 = %d",ADC_VAL);
 	ADC_ConvertedValueLocal=(float)ADC_VAL/4096*3.3;
 	printf(",实际电压电压 = %f v\r\n",(ADC_ConvertedValueLocal*2-2.5)/0.24);
	
	BattNum=0;
	for(i=1;i<400;i=i+8)BattNum+=ADC_ConvertedValue[i];
	ADC_VAL=BattNum/50;
  printf("\r\nADC1 = %d",ADC_VAL);
 	ADC_ConvertedValueLocal=(float)ADC_VAL/4096*3.3;
 	printf(",实际电压电压 = %f v\r\n",ADC_ConvertedValueLocal);
	
	BattNum=0;
	for(i=2;i<400;i=i+8)BattNum+=ADC_ConvertedValue[i];
	ADC_VAL=BattNum/50;
  printf("\r\nADC2 = %d",ADC_VAL);
 	ADC_ConvertedValueLocal=(float)ADC_VAL/4096*3.3;
 	printf(",实际电压电压 = %f v\r\n",ADC_ConvertedValueLocal);
	
	BattNum=0;
	for(i=3;i<400;i=i+8)BattNum+=ADC_ConvertedValue[i];
	ADC_VAL=BattNum/50;
  printf("\r\nADC3 = %d",ADC_VAL);
 	ADC_ConvertedValueLocal=(float)ADC_VAL/4096*3.3;
 	printf(",实际电压电压 = %f v\r\n",ADC_ConvertedValueLocal);
	
	BattNum=0;
	for(i=4;i<400;i=i+8)BattNum+=ADC_ConvertedValue[i];
	ADC_VAL=BattNum/50;
  printf("\r\nADC4 = %d",ADC_VAL);
 	ADC_ConvertedValueLocal=(float)ADC_VAL/4096*3.3;
 	printf(",实际电压电压 = %f v\r\n",ADC_ConvertedValueLocal);
	
	BattNum=0;
	for(i=5;i<400;i=i+8)BattNum+=ADC_ConvertedValue[i];
	ADC_VAL=BattNum/50;
  printf("\r\nADC5 = %d",ADC_VAL);
 	ADC_ConvertedValueLocal=(float)ADC_VAL/4096*3.3;
 	printf(",实际电压电压 = %f v\r\n",ADC_ConvertedValueLocal);
	
	BattNum=0;
	for(i=6;i<400;i=i+8)BattNum+=ADC_ConvertedValue[i];
	ADC_VAL=BattNum/50;
  printf("\r\nADC6 = %d",ADC_VAL);
 	ADC_ConvertedValueLocal=(float)ADC_VAL/4096*3.3;
 	printf(",实际电压电压 = %f v\r\n",ADC_ConvertedValueLocal);
	
	BattNum=0;
	for(i=7;i<400;i=i+8)BattNum+=ADC_ConvertedValue[i];
	ADC_VAL=BattNum/50;
  printf("\r\nADC7 = %d",ADC_VAL);
 	ADC_ConvertedValueLocal=(float)ADC_VAL/4096*3.3;
 	printf(",实际电压电压 = %f v\r\n",ADC_ConvertedValueLocal);
	
	return ADC_VAL;
}

/**************************************************
 * 函数名：ADC_Manage
 * 描述  ：8路ADC处理模块
 * 输入  ：value ADC输入参数结构体变量，详见库文件
 * 输出  ：temp ADC输出参数结构体变量,详见库文件
 **************************************************/
SOFT_Adc_Module_Output ADC_Manage(ADC_Channel_Sel ADC_In_Sel,SOFT_Adc_Module_Input value)
{
	SOFT_Adc_Module_Output temp;
	u32 BattNum=0;
	uint32_t i,ADC_VAL=0;
	float ADC_ConvertedValueLocal; 
	
	if(value.ADC_ChannelInput_En==1)
	{
		BattNum=0;				
		for(i=ADC_In_Sel;i<400;i=i+8)BattNum+=ADC_ConvertedValue[i];
		ADC_VAL=BattNum/50;//ADC滤波
		ADC_ConvertedValueLocal=(float)ADC_VAL/4096*3.3;//计算实际电压值
	  temp.ADC_Channel_Data=ADC_VAL;//返回通道ADC值
		
		DEBUG("ADC = %d",ADC_VAL);
		DEBUG("实际电压电压 = %f v\r\n",(ADC_ConvertedValueLocal*2-2.5)/0.24);

		if(value.ADC_H_lim>0)//设置上限值是否为0
		{
			if(ADC_VAL>value.ADC_H_lim)temp.ADC_Out_Status=1;//大于设定高值输出 1
			else if(ADC_VAL<=value.ADC_L_lim)temp.ADC_Out_Status=0;//小于设定低值输出 0
			else temp.ADC_Out_Status=2;
		}
		else temp.ADC_Out_Status=-1;
	}
	else 
	{
		temp.ADC_Channel_Data=0;
		temp.ADC_Out_Status=-1;
	}
	temp.ADC_Channel_Module_Status=value.ADC_ChannelInput_En;
	return temp;
}
