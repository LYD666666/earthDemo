/**
  ****************************************************************************************************
  * @file       ms53l0m.h
  * @author     正点原子团队(ALIENTEK)
  * @version    V1.0
  * @date       2021-04-17
  * @brief      ms53l0m模块驱动
  * @license   	Copyright (c) 2021-2032, 广州市星翼电子科技有限公司
  ****************************************************************************************************
  * @attention
  *
  * 实验平台:正点原子 STM32开发板
  * 在线视频:www.yuanzige.com
  * 技术论坛:www.openedv.com
  * 公司网址:www.alientek.com
  * 购买地址:openedv.taobao.com
  *
  * 修改说明
  * V1.0 2021-04-17
  * 第一次发布
  *
  ****************************************************************************************************
  */
  
#ifndef _MS53L0M_third_H
#define _MS53L0M_third_H

#include "sys.h"
#include "stdbool.h"

#define Uart_Init_third(bps)       uart5_init(bps)       /*串口初始化API*/
#define Uart_Send_third(data,len)  uart5_send(data,len)  /*串口发送API*/

#define MFRAME_H       0X51 /*主机请求帧头*/
#define SFRAME_H       0X55 /*从机应答帧头*/
#define SENSOR_TYPE    0X0A /*MS53L0M传感器*/


#define RPACK_MAX_LEN  270  /*接收包最大长度*/
#define RPACK_MIN_LEN  8   /*接收包最小长度*/

#define BUFF_MAX_LEN 300 /*串口接收最大长度*/


void Ms53l0m_Init_third(void);
uint8_t Ms53l0m_WData_third(uint16_t addr, uint8_t reg, uint8_t data);
uint8_t Ms53l0m_RData_third(uint16_t addr, uint8_t reg, uint8_t datalen, uint16_t *data);
void Modbus_DataGet_third(void);


#endif
