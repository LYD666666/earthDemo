/**
  ****************************************************************************************************
  * @file       ms53l0m.h
  * @author     ����ԭ���Ŷ�(ALIENTEK)
  * @version    V1.0
  * @date       2021-04-17
  * @brief      ms53l0mģ������
  * @license   	Copyright (c) 2021-2032, �������������ӿƼ����޹�˾
  ****************************************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:����ԭ�� STM32������
  * ������Ƶ:www.yuanzige.com
  * ������̳:www.openedv.com
  * ��˾��ַ:www.alientek.com
  * �����ַ:openedv.taobao.com
  *
  * �޸�˵��
  * V1.0 2021-04-17
  * ��һ�η���
  *
  ****************************************************************************************************
  */
  
#ifndef _MS53L0M_first_H
#define _MS53L0M_first_H

#include "sys.h"
#include "stdbool.h"

#define Uart_Init_first(bps) usart3_init(bps)       /*���ڳ�ʼ��API*/
#define Uart_Send_first(data,len)  usart3_send(data,len)  /*���ڷ���API*/

#define MFRAME_H       0X51 /*��������֡ͷ*/
#define SFRAME_H       0X55 /*�ӻ�Ӧ��֡ͷ*/
#define SENSOR_TYPE    0X0A /*MS53L0M������*/


#define RPACK_MAX_LEN  270  /*���հ���󳤶�*/
#define RPACK_MIN_LEN  8   /*���հ���С����*/

#define BUFF_MAX_LEN 300 /*���ڽ�����󳤶�*/


void Ms53l0m_Init_first(void);
uint8_t Ms53l0m_WData_first(uint16_t addr, uint8_t reg, uint8_t data);
uint8_t Ms53l0m_RData_first(uint16_t addr, uint8_t reg, uint8_t datalen, uint16_t *data);
void Modbus_DataGet_first(void);


#endif