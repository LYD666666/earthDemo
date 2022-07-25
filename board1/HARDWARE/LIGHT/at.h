#ifndef CMD_H_
#define CMD_H_
#include "stm32f10x.h"

#define UartFifo_MAXLEN      35

#define REV_OK		1	//接收完成标志
#define REV_WAIT	0	//接收未完成标志


typedef struct{
  uint8_t message[UartFifo_MAXLEN];
  uint16_t len;
  uint8_t over;
} UART_FIFO;

///////////////////////////////
extern UART_FIFO uartRxFifo;


void AT_Clear(void);
uint8_t AT_ReadCmd(char *cmd, char *res,int *data);//发送查询AT命令
uint8_t AT_WriteCmd(char *cmd, char *res);//发送设置AT命令

#endif
