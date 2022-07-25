#include "light.h"
#include "delay.h"
#include "usart.h"





void IIC_init(void)
{
	uint8_t dat=0;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	SCL_OUT();
	SDA_OUT();
	IIC_EN_OUT();
	IIC_EN=1;
	IIC_SCL=1;
	IIC_SDA=1;
	delay_ms(100);
	
	dat+=IIC_writecfg(0x04,0x00);
}


void IIC_start(void)
{
	SDA_OUT();
	IIC_SDA=1;
	IIC_SCL=1;
	delay_us(5);
	IIC_SDA=0;
	delay_us(5);
	IIC_SCL=0;
}


void IIC_stop(void)
{
	SDA_OUT();
	IIC_SDA=0;
	IIC_SCL=1;
	delay_us(5);
	IIC_SDA=1;
	delay_us(5);
	IIC_SCL=0;
	delay_us(5);
}


u16 IIC_rec_ack(void)
{
	u32 CY=0;
	IIC_SDA=1;
	SDA_IN();
    IIC_SCL=1;	
	delay_us(5);
	CY|=READ_SDA;
	delay_us(5);
	IIC_SCL=0;
	SDA_OUT();
	return CY;
}
void IIC_send_ack(uint8_t ack)
{
	SDA_OUT();
	if(ack&0x01) IIC_SDA=1;
	else IIC_SDA=0;
	
	IIC_SCL=1;
	delay_us(5);
	IIC_SCL=0;
	IIC_SDA=1;
	delay_us(5);
}

uint8_t IIC_send_byte(u8 txd)
{
	u8 t;
	SDA_OUT();
	IIC_SCL=0;
	for(t=0;t<8;t++)
	{
		if(txd&0x80)
			IIC_SDA=1;
		else
			IIC_SDA=0;
		delay_us(5);  
		IIC_SCL=1;
		delay_us(5); 
		IIC_SCL=0;	
		delay_us(5);
		txd<<=1; 
	}
	return IIC_rec_ack();
}

u8 IIC_read_byte(void)
{
	uint8_t i,dat=0;
	SDA_IN();IIC_SDA=1;
    for(i=0;i<8;i++ )
	{
        IIC_SCL=1;
	    delay_us(5);
		dat|=READ_SDA;
		IIC_SCL=0;
		delay_us(5);
		if(i<7) dat<<=1;
    }	
	SDA_OUT();
	return dat;
}
 
uint8_t IIC_writecfg( uint8_t vregaddr,uint8_t vdat)
{
	uint8_t vrval=0x00;
	IIC_start();
	vrval+=IIC_send_byte(0X94);
	vrval+=IIC_send_byte(vregaddr);
	vrval+=IIC_send_byte(vdat);
	return vrval;
}
	
uint8_t IIC_getLux(uint32_t *vLux)
{
	uint8_t vrval=0;
	uint8_t vBUF[129];
	uint32_t val32=0;
	float temp=0.0;
	uint8_t i=0;
	
	IIC_start();
	vrval+=IIC_send_byte(0X94);
	vrval+=IIC_send_byte(0X00);
	
	IIC_start();
	vrval+=IIC_send_byte(0X95);
	delay_ms(10);
	for(i=0;i<=4;i++)
	{
		vBUF[i]=IIC_read_byte();
		if(i==4)
			IIC_send_ack(1);
		else
			IIC_send_ack(0);
	}
	IIC_stop();
	val32=vBUF[3];val32<<=8;
	val32|=vBUF[2];val32<<=8;
	val32|=vBUF[1];val32<<=8;
	val32|=vBUF[0];
	temp=(float)val32*1.4;
	*vLux=(uint32_t)(temp);
	
	return vrval;
}






















