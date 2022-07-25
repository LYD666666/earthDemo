#include "light.h"
#include "delay.h"
#include "usart.h"
#include "sys.h"
#include "math.h"			
#include "stdio.h"
#include "stm32f10x_flash.h"
#include "stdlib.h"
#include "string.h"
#include "wdg.h"
#include "timer.h"
#include "stm32f10x_tim.h"
#include "bc26.h"	 
#include "pwm.h"
#include "motor.h"
#include "at.h"

 extern  uint16_t num1;
 extern	uint16_t num2;
 extern  uint16_t num3;

extern char  RxBuffer[100],RxCounter;
uint32_t m_Lux = 0;
int data;                
volatile float data_eerup=0;

int main(void)
{	
	float  vfLux;
   Motor_12_Config();  //���IN1234��ʼ��
	delay_init();  //��ʱ������ʼ��	
   NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�  
	IIC_EN=1;
	IIC_init();
	uart_init(9600);
   uart2_init(115200);//�Խ�BC26���ڳ�ʼ��
   uart3_at_init(9600); //��ɫ������
   TIM3_PWM_Init(899,71);	//����Ƶ��PWMƵ��=72000000/900/72=1Khz
   TIM5_PWM_Init(899,71);   //����Ƶ��PWMƵ��=72000000/900=80Khz 
   IWDG_Init(7,625);    //8Sһ��
   
//    AT_Init();
//    MQTT_Init();
   IWDG_Feed();//ι��
   
	
	while(1)
	{	
    if(IIC_getLux(&m_Lux)==0)
		{ 
			vfLux=(m_Lux/1000.0);
			printf("��ǰ����ֵΪ%f\r\n",vfLux);
      //aliyunMQTT_PUBdata(vfLux);
		}
      if(vfLux > 1000)
      {
            Motor_1_NRun();	//c0 c1	
            Motor_2_PRun();	//B2 B6		 		
            TIM_SetCompare1(TIM5,500);//A0       �ұ�  
            TIM_SetCompare2(TIM3,500);//B5  
      }
      else {
            Motor_1_PRun();	//c0 c1	
            Motor_2_NRun();	//B2 B6  		
            TIM_SetCompare1(TIM5,500);//A0         
            TIM_SetCompare2(TIM3,500);//B5    
      }
   
   if(AT_ReadCmd("AT+COLOR\r\n","OK",&data)==0)//Ҳ�ɷ�������ָ��
	{
		data_eerup=data;//��ȡ�ɹ�
	}
	else
	{ 
		Uart1_SendStr("0\r\n");/*��ȡʧ��*/
	}
   
      if(num1<5&&num2<5)
      {
         TIM_SetCompare1(TIM5,1000);//A0         
         TIM_SetCompare2(TIM3,1000);//B5  
      }
      else{
         TIM_SetCompare1(TIM5,500);//A0         
         TIM_SetCompare2(TIM3,500);//B5  
      }
   
     delay_ms(1000);
   
      
    IWDG_Feed();//ι��
      
      
	}	
	
}
