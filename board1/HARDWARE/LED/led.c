#include "led.h"


//��ʼ��PA0��PB0Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{
 
		GPIO_InitTypeDef  GPIO_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PB,PE�˿�ʱ��

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //LED0-->PB0 �˿�����
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
		GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA0


	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //PWRKEY-->PA0 �˿�����
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
		GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOA0

				
				
}
 

 