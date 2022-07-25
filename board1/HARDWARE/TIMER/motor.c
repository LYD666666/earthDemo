#include "motor.h"  //����ledͷ�ļ�
#include "stm32f10x.h"  //����STM32�ٷ���
#include "stm32f10x_rcc.h"  //����STM32��RCCʱ�ӿ�
#include "delay.h"       //��ʱ��
 
void Motor_12_Config(void)  //�����ʼ������
{
GPIO_InitTypeDef  GPIO_InitStructure;                        //����GPIO�ṹ��
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	      //ʹ��PB�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                    //LFOOT1-->PB.5�˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;             //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;            //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);                       //�����趨������ʼ��GPIOB.5
 GPIO_SetBits(GPIOB,GPIO_Pin_6);                              //PCB.5 �����
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;                    //LFOOT1-->PB.2�˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;             //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;            //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);                       //�����趨������ʼ��GPIOB.2
 GPIO_SetBits(GPIOB,GPIO_Pin_2);                              //PCB.2 �����
 
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	      //ʹ��PB�˿�ʱ��
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                    //LFOOT1-->PB.5�˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;             //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;            //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);                       //�����趨������ʼ��GPIOB.5
 GPIO_SetBits(GPIOC,GPIO_Pin_0);                              //PCB.5 �����
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;                    //LFOOT1-->PB.5�˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;             //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;            //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);                       //�����趨������ʼ��GPIOB.5
 GPIO_SetBits(GPIOC,GPIO_Pin_1);            
	
	
}
 
 
  void Motor_1_STOP(void)
{
 IN1(High);
 IN2(High);
}
 
void Motor_1_PRun(void)
{
 IN1(Low);
 IN2(High);
}
 
void Motor_1_NRun(void)
{
 IN1(High);
 IN2(Low);
}



void Motor_2_STOP(void)
{
 IN3(High);
 IN4(High);
}
 
void Motor_2_PRun(void)
{
 IN3(Low);
 IN4(High);
}
 
void Motor_2_NRun(void)
{
 IN3(High);
 IN4(Low);
}


