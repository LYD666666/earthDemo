#ifndef __MOTOR1_H
#define __MOTOR1_H
 
 
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
 
#define High    1
#define Low     0

#define LED0 PBout(8)// PB6
#define LED1 PEout(5)// PE5	
 
#define IN1(a) if (a)  \
 GPIO_SetBits(GPIOC,GPIO_Pin_0);\
else  \
 GPIO_ResetBits(GPIOC,GPIO_Pin_0)
 
#define IN2(a) if (a)  \
 GPIO_SetBits(GPIOC,GPIO_Pin_1);\
else  \
 GPIO_ResetBits(GPIOC,GPIO_Pin_1)
 
#define IN3(a) if (a)  \
 GPIO_SetBits(GPIOB,GPIO_Pin_2);\
else  \
 GPIO_ResetBits(GPIOB,GPIO_Pin_2)
 
#define IN4(a) if (a)  \
 GPIO_SetBits(GPIOB,GPIO_Pin_6);\
else  \
 GPIO_ResetBits(GPIOB,GPIO_Pin_6)


void Motor_12_Config(void);
 
void Motor_1_STOP(void);
void Motor_1_PRun(void);
void Motor_1_NRun(void);

void Motor_2_STOP(void);
void Motor_2_PRun(void);
void Motor_2_NRun(void);
 
 
#endif

/*#define LFOOT1 PCout(0)    // 定义PB6
#define LFOOT2 PCout(1)    // 定义PB7
#define RFOOT1 PBout(2)    // 定义PB2
#define RFOOT2 PBout(5)    // 定义PB5


#include "sys.h"

void MOTOR_Init(void);
void GO(void);
void right_move(void);
void left_move(void);
void stop_move(void);

#endif*/
