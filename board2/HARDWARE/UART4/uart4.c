#include "delay.h"
#include "uart4.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"
#include "ms53l0m_first.h"
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
__align(8) u8 UART4_TX_BUF[UART4_MAX_SEND_LEN]; //���ͻ���,���UART4_MAX_SEND_LEN�ֽ�
u8 UART4_RX_BUF[UART4_MAX_RECV_LEN]; 				//���ջ���,���UART4_MAX_RECV_LEN���ֽ�.

extern _Tof_Data Tof_Data3;

/*UART2�жϷ�����*/
void UART4_IRQHandler(void)
{
    u8 res;

    if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET) //���յ�����
    {
        res = USART_ReceiveData(UART4);

        if (!Tof_Data3.rx_ok) /*û�������*/
        {
            if(Tof_Data3.rx_len < (BUFF_MAX_LEN - 1))
            {
                 Tof_Data3.rx_buff[Tof_Data3.rx_len] = res;/*��������*/
                 Tof_Data3.rx_len++; /*���ճ����ۼ�*/
            }else 
            {
                 Tof_Data3.rx_len = 0; /*���ճ�����Χ*/
                 Tof_Data3.rx_ok = 1;
            }
        }
    }
    
    if (USART_GetITStatus(UART4, USART_IT_IDLE) != RESET) //�����ж�
    {
         res = USART_ReceiveData(UART4); /*������ж�*/
        
         if ((Tof_Data3.rx_len != 0) && !Tof_Data3.rx_ok)
         {
            Tof_Data3.rx_ok = 1;/*��־������*/
         }
    }
    
}

extern USART_InitTypeDef USART_InitStructure;

//��ʼ��IO ����2
//pclk1:PCLK1ʱ��Ƶ��(Mhz)
//bound:������
void uart4_init(u32 bound)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);// GPIOAʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

    USART_DeInit(UART4);                                //��λ����3
    //UART4_TX   PB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;            //PC.10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	     //�����������
    GPIO_Init(GPIOC, &GPIO_InitStructure);               //��ʼ��PC.10

    //UART4_RX	  PB.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;            //PC.11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;        //��������
    GPIO_Init(GPIOC, &GPIO_InitStructure);               //��ʼ��PC.11

    USART_InitStructure.USART_BaudRate = bound;                                    //һ������Ϊ9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;                            //����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	               //�շ�ģʽ

    USART_Init(UART4, &USART_InitStructure); //��ʼ������3
    
    USART_Cmd(UART4, ENABLE);                                         //ʹ�ܴ���

    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//���������ж�
    USART_ITConfig(UART4, USART_IT_IDLE, ENABLE);//���������ж�

    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ; //��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

    Tof_Data3.rx_ok = 0;				  //����
}
//����2,printf ����
//ȷ��һ�η������ݲ�����UART4_MAX_SEND_LEN�ֽ�
void u4_printf(char *fmt, ...)
{
    u16 i, j;
    va_list ap;
    va_start(ap, fmt);
    vsprintf((char *)UART4_TX_BUF, fmt, ap);
    va_end(ap);

    for (j = 0; j < i; j++) //ѭ����������
    {
        while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET); //ѭ������,ֱ���������

        USART_SendData(UART4, UART4_TX_BUF[j]);
    }
}

//����2���ݷ���
//*date:���ݻ����ַ
//len:���ݳ���
void uart4_send(uint8_t *data, uint8_t len)
{

    uint8_t i = 0;

    for (i = 0; i < len; i++) //ѭ����������
    {
        while ((UART4->SR & 0X40) == 0); //ѭ������,ֱ���������

        UART4->DR = *(data + i);
    }

}


