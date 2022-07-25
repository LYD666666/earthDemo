#include "motor.h"  //导入led头文件
#include "stm32f10x.h"  //导入STM32官方库
#include "stm32f10x_rcc.h"  //导入STM32的RCC时钟库
#include "delay.h"       //延时库
 
void Motor_12_Config(void)  //定义初始化函数
{
GPIO_InitTypeDef  GPIO_InitStructure;                        //定义GPIO结构体
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	      //使能PB端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;                    //LFOOT1-->PB.5端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;             //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;            //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);                       //根据设定参数初始化GPIOB.5
 GPIO_SetBits(GPIOB,GPIO_Pin_6);                              //PCB.5 输出高
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;                    //LFOOT1-->PB.2端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;             //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;            //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);                       //根据设定参数初始化GPIOB.2
 GPIO_SetBits(GPIOB,GPIO_Pin_2);                              //PCB.2 输出高
 
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	      //使能PB端口时钟
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                    //LFOOT1-->PB.5端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;             //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;            //IO口速度为50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);                       //根据设定参数初始化GPIOB.5
 GPIO_SetBits(GPIOC,GPIO_Pin_0);                              //PCB.5 输出高
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;                    //LFOOT1-->PB.5端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;             //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;            //IO口速度为50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);                       //根据设定参数初始化GPIOB.5
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


