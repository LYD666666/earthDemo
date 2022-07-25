#include "delay.h"
#include "usart2.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"
#include "ms53l0m.h"

//////////////////////////////////////////////////////////////////////////////////
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32������
//����2��������
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2014/3/29
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////

//���ڷ��ͻ�����
__align(8) u8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; //���ͻ���,���USART2_MAX_SEND_LEN�ֽ�
u8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; 				//���ջ���,���USART2_MAX_RECV_LEN���ֽ�.


/*UART2�жϷ�����*/
void USART2_IRQHandler(void)
{
    u8 res;

    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //���յ�����
    {
        res = USART_ReceiveData(USART2);

        if (!Tof_Data.rx_ok) /*û�������*/
        {
            if(Tof_Data.rx_len < (BUFF_MAX_LEN - 1))
            {
                 Tof_Data.rx_buff[Tof_Data.rx_len] = res;/*��������*/
                 Tof_Data.rx_len++; /*���ճ����ۼ�*/
            }else 
            {
                 Tof_Data.rx_len = 0; /*���ճ�����Χ*/
                 Tof_Data.rx_ok = 1;
            }
        }
    }
    
    if (USART_GetITStatus(USART2, USART_IT_IDLE) != RESET) //�����ж�
    {
         res = USART_ReceiveData(USART2); /*������ж�*/
        
         if ((Tof_Data.rx_len != 0) && !Tof_Data.rx_ok)
         {
            Tof_Data.rx_ok = 1;/*��־������*/
         }
    }
    
}

USART_InitTypeDef USART_InitStructure;

//��ʼ��IO ����2
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������
void usart2_init(u32 bound)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);// GPIOAʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    USART_DeInit(USART2);                                //��λ����1
    //USART2_TX   PA.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;            //PA.2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	     //�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);               //��ʼ��PA2

    //USART2_RX	  PA.3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;            //PA.3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;        //��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);               //��ʼ��PA3

    USART_InitStructure.USART_BaudRate = bound;                                    //һ������Ϊ9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;                            //����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	               //�շ�ģʽ

    USART_Init(USART2, &USART_InitStructure); //��ʼ������2
    
    USART_Cmd(USART2, ENABLE);                                         //ʹ�ܴ���

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//���������ж�
    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//���������ж�

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2 ; //��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

    Tof_Data.rx_ok = 0;				  //����
}
//����2,printf ����
//ȷ��һ�η������ݲ�����USART2_MAX_SEND_LEN�ֽ�
void u2_printf(char *fmt, ...)
{
    u16 i, j;
    va_list ap;
    va_start(ap, fmt);
    vsprintf((char *)USART2_TX_BUF, fmt, ap);
    va_end(ap);

    for (j = 0; j < i; j++) //ѭ����������
    {
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET); //ѭ������,ֱ���������

        USART_SendData(USART2, USART2_TX_BUF[j]);
    }
}

//����2���ݷ���
//*date:���ݻ����ַ
//len:���ݳ���
void usart2_send(uint8_t *data, uint8_t len)
{

    uint8_t i = 0;

    for (i = 0; i < len; i++) //ѭ����������
    {
        while ((USART2->SR & 0X40) == 0); //ѭ������,ֱ���������

        USART2->DR = *(data + i);
    }

}


