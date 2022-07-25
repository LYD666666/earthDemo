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


////m^n函数
//u32 mypow(u8 m, u8 n)
//{
//  u32 result = 1;
//  while(n--)result *= m;
//  return result;
//}

//==========================================================
//	函数名称：	AT_Clear
//
//	函数功能：	清空缓存
//
//	入口参数：	无
//
//	返回参数：	无
//
//	说明：		
//==========================================================
void AT_Clear(void)
{
	memset(uartRxFifo.message, 0, uartRxFifo.len);
	uartRxFifo.len = 0;
  uartRxFifo.over = 0;

}
//==========================================================
//	函数名称：AT_ReadCmd
//
//	函数功能：	发送查询AT命令
//
//	入口参数：	cmd：命令
//				res：需要检查的返回指令
//        data：返回的数据
//
//	返回参数：	0-成功	1-失败  2失败:数据超限
//
//	说明：		
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
      
		if(uartRxFifo.over == REV_OK)							//如果收到数据
		{
			if(strstr((const char *)uartRxFifo.message, res) != NULL)		//如果检索到关键词
			{
				
				//start=strchr((const char *)uartRxFifo.message, '=');		//数据整理
            start=strchr((const char *)uartRxFifo.message, '=');	
				if(start!=NULL)
				{
               //问题
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
				AT_Clear();										//清空缓存
				return 0;
			}
			else if(strstr((const char *)uartRxFifo.message, "ERROR_3") != NULL)
			{
				AT_Clear();										//清空缓存
				return 2;
			}
		}
      
		delay_ms(1);
	}
	AT_Clear();										//清空缓存
	return 1;
}

//==========================================================
//	函数名称：AT_WriteCmd
//
//	函数功能：	发送设置AT命令
//
//	入口参数：	cmd：命令
//				res：需要检查的返回指令
//
//	返回参数：	0-成功	1-失败  2失败:数据超限
//
//	说明：		
//==========================================================
uint8_t AT_WriteCmd(char *cmd, char *res)
{
	uint16_t timeOut = 300;
	
	UartSendStr_at((char *)cmd);
	
	while(timeOut--)
	{
		if(uartRxFifo.over == REV_OK)							//如果收到数据
		{
			if(strstr((const char *)uartRxFifo.message, res) != NULL)		//如果检索到关键词
			{				
				AT_Clear();										//清空缓存
				return 0;
			}
			else if(strstr((const char *)uartRxFifo.message, "ERROR_3") != NULL)
			{
				AT_Clear();										//清空缓存
				return 2;
			}
		}
		
		delay_ms(1);
	}
	AT_Clear();										//清空缓存
	return 1;
}
