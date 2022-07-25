#include "delay.h"
#include "uart4.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"
#include "ms53l0m_first.h"
#include "ms53l0m.h"

//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口2驱动代码
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/3/29
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////

//串口发送缓存区
__align(8) u8 UART4_TX_BUF[UART4_MAX_SEND_LEN]; //发送缓冲,最大UART4_MAX_SEND_LEN字节
u8 UART4_RX_BUF[UART4_MAX_RECV_LEN]; 				//接收缓冲,最大UART4_MAX_RECV_LEN个字节.

extern _Tof_Data Tof_Data3;

/*UART2中断服务函数*/
void UART4_IRQHandler(void)
{
    u8 res;

    if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET) //接收到数据
    {
        res = USART_ReceiveData(UART4);

        if (!Tof_Data3.rx_ok) /*没接收完成*/
        {
            if(Tof_Data3.rx_len < (BUFF_MAX_LEN - 1))
            {
                 Tof_Data3.rx_buff[Tof_Data3.rx_len] = res;/*保存数据*/
                 Tof_Data3.rx_len++; /*接收长度累加*/
            }else 
            {
                 Tof_Data3.rx_len = 0; /*接收超过范围*/
                 Tof_Data3.rx_ok = 1;
            }
        }
    }
    
    if (USART_GetITStatus(UART4, USART_IT_IDLE) != RESET) //空闲中断
    {
         res = USART_ReceiveData(UART4); /*清空闲中断*/
        
         if ((Tof_Data3.rx_len != 0) && !Tof_Data3.rx_ok)
         {
            Tof_Data3.rx_ok = 1;/*标志接收完*/
         }
    }
    
}

extern USART_InitTypeDef USART_InitStructure;

//初始化IO 串口2
//pclk1:PCLK1时钟频率(Mhz)
//bound:波特率
void uart4_init(u32 bound)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);// GPIOA时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);

    USART_DeInit(UART4);                                //复位串口3
    //UART4_TX   PB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;            //PC.10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	     //复用推挽输出
    GPIO_Init(GPIOC, &GPIO_InitStructure);               //初始化PC.10

    //UART4_RX	  PB.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;            //PC.11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;        //上拉输入
    GPIO_Init(GPIOC, &GPIO_InitStructure);               //初始化PC.11

    USART_InitStructure.USART_BaudRate = bound;                                    //一般设置为9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                            //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	               //收发模式

    USART_Init(UART4, &USART_InitStructure); //初始化串口3
    
    USART_Cmd(UART4, ENABLE);                                         //使能串口

    USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启接收中断
    USART_ITConfig(UART4, USART_IT_IDLE, ENABLE);//开启空闲中断

    NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ; //抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

    Tof_Data3.rx_ok = 0;				  //清零
}
//串口2,printf 函数
//确保一次发送数据不超过UART4_MAX_SEND_LEN字节
void u4_printf(char *fmt, ...)
{
    u16 i, j;
    va_list ap;
    va_start(ap, fmt);
    vsprintf((char *)UART4_TX_BUF, fmt, ap);
    va_end(ap);

    for (j = 0; j < i; j++) //循环发送数据
    {
        while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET); //循环发送,直到发送完毕

        USART_SendData(UART4, UART4_TX_BUF[j]);
    }
}

//串口2数据发送
//*date:数据缓存地址
//len:数据长度
void uart4_send(uint8_t *data, uint8_t len)
{

    uint8_t i = 0;

    for (i = 0; i < len; i++) //循环发送数据
    {
        while ((UART4->SR & 0X40) == 0); //循环发送,直到发送完毕

        UART4->DR = *(data + i);
    }

}


