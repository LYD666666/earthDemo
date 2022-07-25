#include "at.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "usart.h"
#include "delay.h"
#include "sys.h"

   uint16_t num1;
	uint16_t num2;
   uint16_t num3;


UART_FIFO uartRxFifo =
{
  0,
  0,
  0
};


////m^n����
//u32 mypow(u8 m, u8 n)
//{
//  u32 result = 1;
//  while(n--)result *= m;
//  return result;
//}

//==========================================================
//	�������ƣ�	AT_Clear
//
//	�������ܣ�	��ջ���
//
//	��ڲ�����	��
//
//	���ز�����	��
//
//	˵����		
//==========================================================
void AT_Clear(void)
{
	memset(uartRxFifo.message, 0, uartRxFifo.len);
	uartRxFifo.len = 0;
  uartRxFifo.over = 0;

}
//==========================================================
//	�������ƣ�AT_ReadCmd
//
//	�������ܣ�	���Ͳ�ѯAT����
//
//	��ڲ�����	cmd������
//				res����Ҫ���ķ���ָ��
//        data�����ص�����
//
//	���ز�����	0-�ɹ�	1-ʧ��  2ʧ��:���ݳ���
//
//	˵����		
//==========================================================
uint8_t AT_ReadCmd(char *cmd, char *res,int *data)
{
   uint8_t r[20];
   uint8_t g[20];
   uint8_t b[20];
   
	uint16_t timeOut = 300;
	char* start=NULL;
	uint8_t num[20];
   char match_str[10] = ':';
   char *loc;
   char *loc1;
   char *loc2;
   
   char* infor;
   char numR[20] = 'R';
   char numG[20] = 'G';
   char numB[20] = 'B';
	UartSendStr_at((char *)cmd);
   
	while(timeOut--)
	{
      
		if(uartRxFifo.over == REV_OK)							//����յ�����
		{
			if(strstr((const char *)uartRxFifo.message, res) != NULL)		//����������ؼ���
			{
				
				//start=strchr((const char *)uartRxFifo.message, '=');		//��������
            start=strchr((const char *)uartRxFifo.message, '=');	
				if(start!=NULL)
				{
               //����
           		//strncpy((char*)test, start+1,3);
					strncpy((char*)num, start+2,14);
					*data=atof((char *)num);
               //printf("xueyang =%s\r\n",num);
               Uart1_SendStr(num);
               Uart1_SendStr("\r\n");
               
               loc = strstr(start,numR);
               strncpy((char*)r, loc+2,3);
					*data=atof((char *)r);             
               
               loc1 = strstr(loc,numG);
               strncpy((char*)g, loc1+2,3);
					*data=atof((char *)g);
               
               loc2 = strstr(loc1,numB);
               strncpy((char*)b, loc2+2,3);
					*data=atof((char *)b);
               
               num1 = atoi(r);
               num2 = atoi(g);
               num3 = atoi(b);
               
				}
				AT_Clear();										//��ջ���
				return 0;
			}
			else if(strstr((const char *)uartRxFifo.message, "ERROR_3") != NULL)
			{
				AT_Clear();										//��ջ���
				return 2;
			}
		}
      
		delay_ms(1);
	}
	AT_Clear();										//��ջ���
	return 1;
}

//==========================================================
//	�������ƣ�AT_WriteCmd
//
//	�������ܣ�	��������AT����
//
//	��ڲ�����	cmd������
//				res����Ҫ���ķ���ָ��
//
//	���ز�����	0-�ɹ�	1-ʧ��  2ʧ��:���ݳ���
//
//	˵����		
//==========================================================
uint8_t AT_WriteCmd(char *cmd, char *res)
{
	uint16_t timeOut = 300;
	
	UartSendStr_at((char *)cmd);
	
	while(timeOut--)
	{
		if(uartRxFifo.over == REV_OK)							//����յ�����
		{
			if(strstr((const char *)uartRxFifo.message, res) != NULL)		//����������ؼ���
			{				
				AT_Clear();										//��ջ���
				return 0;
			}
			else if(strstr((const char *)uartRxFifo.message, "ERROR_3") != NULL)
			{
				AT_Clear();										//��ջ���
				return 2;
			}
		}
		
		delay_ms(1);
	}
	AT_Clear();										//��ջ���
	return 1;
}
