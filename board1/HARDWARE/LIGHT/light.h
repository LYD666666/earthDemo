#ifndef __LIGHT_H
#define __LIGHT_H
#include "sys.h"


#define uchar unsigned char 
#define SCL_OUT()   {\
							GPIO_InitTypeDef  GPIO_ST; \
							GPIO_ST.GPIO_Pin = GPIO_Pin_5;\
							GPIO_ST.GPIO_Mode = GPIO_Mode_Out_PP; \
							GPIO_ST.GPIO_Speed = GPIO_Speed_50MHz;\
							GPIO_Init(GPIOA, &GPIO_ST); }
#define SCL_IN()   {\
							GPIO_InitTypeDef  GPIO_ST; \
							GPIO_ST.GPIO_Pin = GPIO_Pin_5;\
							GPIO_ST.GPIO_Mode = GPIO_Mode_Out_IPD; \
							GPIO_ST.GPIO_Speed = GPIO_Speed_50MHz;\
							GPIO_Init(GPIOA, &GPIO_ST); }	
#define SDA_OUT()  {\
							GPIO_InitTypeDef  GPIO_ST; \
							GPIO_ST.GPIO_Pin = GPIO_Pin_4;\
							GPIO_ST.GPIO_Mode = GPIO_Mode_Out_PP; \
							GPIO_ST.GPIO_Speed = GPIO_Speed_50MHz;\
							GPIO_Init(GPIOA, &GPIO_ST); }
#define SDA_IN()   {\
							GPIO_InitTypeDef  GPIO_ST; \
							GPIO_ST.GPIO_Pin = GPIO_Pin_4;\
							GPIO_ST.GPIO_Mode = GPIO_Mode_IN_FLOATING; \
							GPIO_ST.GPIO_Speed = GPIO_Speed_50MHz;\
						    GPIO_Init(GPIOA, &GPIO_ST); }
#define IIC_EN_OUT();   {\
							GPIO_InitTypeDef  GPIO_ST; \
							GPIO_ST.GPIO_Pin = GPIO_Pin_6;\
							GPIO_ST.GPIO_Mode = GPIO_Mode_Out_PP; \
							GPIO_ST.GPIO_Speed = GPIO_Speed_50MHz;\
						    GPIO_Init(GPIOA, &GPIO_ST); }

#define IIC_SCL  PAout(5)		//PA5->SCL
#define IIC_SDA  PAout(4)		//PA4->SDA
#define READ_SDA PAin(4)
#define IIC_EN   PAout(6)       //Ê¹ÄÜÎ»PA6

#define SlaveAddress 0x94
void IIC_init(void);
void IIC_start(void);
void IIC_stop(void);
uint8_t IIC_send_byte(u8 txd);

void IIC_send_ack(uint8_t ack);


u8 IIC_read_byte(void);	

uint8_t IIC_writecfg( uint8_t vregaddr,uint8_t vdat);
uint8_t IIC_getLux(uint32_t *vLux);


void LIGHT_single_write(uint8_t Address);
#endif


