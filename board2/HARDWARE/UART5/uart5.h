#ifndef __UART5_H
#define __UART5_H	 
#include "sys.h"  
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

#define UART5_MAX_RECV_LEN		1024				//�����ջ����ֽ���
#define UART5_MAX_SEND_LEN		600				//����ͻ����ֽ���
#define UART5_RX_EN 			1					//0,������;1,����.

extern u8  UART5_RX_BUF[UART5_MAX_RECV_LEN]; 		//���ջ���,���UART5_MAX_RECV_LEN�ֽ�
extern u8  UART5_TX_BUF[UART5_MAX_SEND_LEN]; 		//���ͻ���,���UART5_MAX_SEND_LEN�ֽ�
extern u16 UART5_RX_STA;   						//��������״̬

void uart5_init(u32 bound);				//����4��ʼ�� 
void u4_printf(char* fmt, ...);
void uart5_send(uint8_t *data, uint8_t len);

#endif

