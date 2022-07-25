#ifndef CMD_H_
#define CMD_H_
#include "stm32f10x.h"

#define UartFifo_MAXLEN      35

#define REV_OK		1	//������ɱ�־
#define REV_WAIT	0	//����δ��ɱ�־


typedef struct{
  uint8_t message[UartFifo_MAXLEN];
  uint16_t len;
  uint8_t over;
} UART_FIFO;

///////////////////////////////
extern UART_FIFO uartRxFifo;


void AT_Clear(void);
uint8_t AT_ReadCmd(char *cmd, char *res,int *data);//���Ͳ�ѯAT����
uint8_t AT_WriteCmd(char *cmd, char *res);//��������AT����

#endif
