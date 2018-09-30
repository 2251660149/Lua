
#include "bsp_led.h"

/**********************************************************
 * ������ : LED_Init
 * ����   ��none
 * ���   ��none 
 * ����   ��LED�ӿڳ�ʼ��
 **********************************************************/
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);	//����GPIOC����ʱ��	
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_3 | GPIO_Pin_0| GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_SetBits(GPIOE ,GPIO_Pin_3);
	PWR_BackupAccessCmd(ENABLE);//�����޸�RTC�ͺ󱸼Ĵ���
  RCC_LSEConfig(RCC_LSE_OFF); //�ر��ⲿ����ʱ���ź�
	BKP_TamperPinCmd(DISABLE);  //�ر����ּ�⹦��
	BKP_ITConfig(DISABLE);      //������д����
}

