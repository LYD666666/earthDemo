#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "stm32f10x_iwdg.h"
#include "ms53l0m.h"
#include "ms53l0m_first.h"
#include "ms53l0m_second.h"
#include "ms53l0m_third.h"
#include "bc26.h"
#include "wdg.h"
#include "switch.h"
#include "pwm.h"

int main(void)
{
    NVIC_Configuration(); 
    delay_init();	    	             //延时函数初始化
    LED_Init();                      //代表IN1\IN2
    GPIO_SetBits(GPIOB,GPIO_Pin_6);
    GPIO_SetBits(GPIOA,GPIO_Pin_8);
    uart_init(115200);	 	          //串口初始化为115200
   
    Ms53l0m_Init();                  //MS53L0M模块初始化         串口2
    Ms53l0m_Init_first();            //第二块MS53L0M模块初始化   串口3
    Ms53l0m_Init_second();           //第三块MS53L0M模块初始化   串口4
    Ms53l0m_Init_third();            //第四块MS53L0M模块初始化   串口5
    IWDG_Init(7,625);    //8S一次
    //printf("\r\nModbus模式测试\r\n\r\n");
    AT_Init();
    MQTT_Init();
    delay_ms(180);
   
    EXTI_Key_Config();               //中断按键初始化
    TIM3_PWM_Init(999,72);            //定时器初始化
    TIM5_PWM_Init(999,72);           //
    TIM_SetCompare1(TIM5,500);//A0
    IWDG_Feed();//喂狗

    while(1)
    { 
      //Normal_DataGet();     /*NORMAL模式获取距离数据*/
        Modbus_DataGet();       /*MODBUS模式获取距离数据*/
        IWDG_Feed();//喂狗 
        Modbus_DataGet_first(); /*MODBUS模式获取距离数据*/
        IWDG_Feed();//喂狗
        Modbus_DataGet_second();/*MODBUS模式获取距离数据*/ 
        IWDG_Feed();//喂狗 
        Modbus_DataGet_third();/*MODBUS模式获取距离数据*/ 
        IWDG_Feed();//喂狗      
    }
}

