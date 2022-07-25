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
    delay_init();	    	             //��ʱ������ʼ��
    LED_Init();                      //����IN1\IN2
    GPIO_SetBits(GPIOB,GPIO_Pin_6);
    GPIO_SetBits(GPIOA,GPIO_Pin_8);
    uart_init(115200);	 	          //���ڳ�ʼ��Ϊ115200
   
    Ms53l0m_Init();                  //MS53L0Mģ���ʼ��         ����2
    Ms53l0m_Init_first();            //�ڶ���MS53L0Mģ���ʼ��   ����3
    Ms53l0m_Init_second();           //������MS53L0Mģ���ʼ��   ����4
    Ms53l0m_Init_third();            //���Ŀ�MS53L0Mģ���ʼ��   ����5
    IWDG_Init(7,625);    //8Sһ��
    //printf("\r\nModbusģʽ����\r\n\r\n");
    AT_Init();
    MQTT_Init();
    delay_ms(180);
   
    EXTI_Key_Config();               //�жϰ�����ʼ��
    TIM3_PWM_Init(999,72);            //��ʱ����ʼ��
    TIM5_PWM_Init(999,72);           //
    TIM_SetCompare1(TIM5,500);//A0
    IWDG_Feed();//ι��

    while(1)
    { 
      //Normal_DataGet();     /*NORMALģʽ��ȡ��������*/
        Modbus_DataGet();       /*MODBUSģʽ��ȡ��������*/
        IWDG_Feed();//ι�� 
        Modbus_DataGet_first(); /*MODBUSģʽ��ȡ��������*/
        IWDG_Feed();//ι��
        Modbus_DataGet_second();/*MODBUSģʽ��ȡ��������*/ 
        IWDG_Feed();//ι�� 
        Modbus_DataGet_third();/*MODBUSģʽ��ȡ��������*/ 
        IWDG_Feed();//ι��      
    }
}

