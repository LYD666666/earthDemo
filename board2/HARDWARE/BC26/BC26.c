#include "bc26.h"
#include "string.h"
#include "usart.h"
#include "wdg.h"
#include "led.h"
char *strx,*extstrx;
extern unsigned char  RxBuffer[255],RxCounter;
BC26 BC26_Status;
////阿里云的三元素,在设备里面都可以查询到//////////////////////
#define ProductKey     "gta2hCyBCZR"             //产品KEY
#define DeviceName          "dmxt001"      //
#define DeviceSecret    "55148c5cfbd01f22126e17c08e2c8b51"  //
#define PubTopic         "/sys/gta2hCyBCZR/dmxt001/thing/event/property/post"//发布主题
#define SubTopic        "/gta2hCyBCZR/dmxt001/user/get"//订阅主题
void Clear_Buffer(void)//清空缓存
{
		u8 i;
		Uart1_SendStr((char*)RxBuffer);
		for(i=0;i<100;i++)
		RxBuffer[i]=0;//缓存
		RxCounter=0;
		IWDG_Feed();//喂狗
	
}

void  AT_Init(void)
{
	Clear_Buffer();
    printf("AT\r\n");
    delay_ms(500);
    printf("AT\r\n");
    delay_ms(500);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
    while(strx==NULL)
    {
        Clear_Buffer();
        printf("AT\r\n");
        delay_ms(500);
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
    }
    printf("ATE0\r\n"); //关闭回显
    delay_ms(500);
    Clear_Buffer();
    printf("AT+CSQ\r\n"); //检查CSQ
    delay_ms(500);
    
    printf("AT+CPIN?\r\n");//检查SIM卡是否在位
    delay_ms(500);
    strx=strstr((const char*)RxBuffer,(const char*)"+CPIN: READY");//查看是否返回ready
    while(strx==NULL)
    {
        Clear_Buffer();
        printf("AT+CPIN?\r\n");
        delay_ms(500);
        strx=strstr((const char*)RxBuffer,(const char*)"+CPIN: READY");//检查SIM卡是否在位，等待卡在位，如果卡识别不到，剩余的工作就没法做了
    }
    Clear_Buffer();
    ///
    printf("AT+QMTCLOSE=0\r\n");//断开MQTT连接
    delay_ms(500);
}

void  MQTT_Init(void)
{
	Clear_Buffer();
    printf("AT+CGPADDR=1\r\n");//查看是否获取到IP
    delay_ms(500);
    strx=strstr((const char*)RxBuffer,(const char*)"+CGPADDR:");
    IWDG_Feed();
    while(strx==NULL)
    {
        Clear_Buffer();
        printf("AT+CGPADDR=1\r\n");
        delay_ms(500);
        strx=strstr((const char*)RxBuffer,(const char*)"+CGPADDR:");
    }
    Clear_Buffer();	
    printf("AT+QMTCFG=\"aliauth\",0,\"gta2hCyBCZR\",\"dmxt001\",\"55148c5cfbd01f22126e17c08e2c8b51\"\r\n");//设置阿里云设备信息
    delay_ms(500);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");
    while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"OK");
    }
    Clear_Buffer();	
    printf("AT+QMTOPEN=0,\"iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883\r\n");  //连接阿里云服务器端
    delay_ms(500);
    strx=strstr((const char*)RxBuffer,(const char*)"+QMTOPEN: 0,0");
    while(strx==NULL)
    {
		delay_ms(500);
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTOPEN: 0,0");
    }
    Clear_Buffer();
    printf("AT+QMTCONN=0,\"BC26\"\r\n");//对接阿里云物联网平台创建的设备
    delay_ms(500);
    strx=strstr((const char*)RxBuffer,(const char*)"+QMTCONN: 0,0,0");//看下返回状态
    while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTCONN: 0,0,0");//看下返回状态
    }
    Clear_Buffer();
    printf("AT+QMTSUB=0,1,\"/gta2hCyBCZR/BC26/user/get\",0\r\n");//订阅个主题
    delay_ms(500);
    strx=strstr((const char*)RxBuffer,(const char*)"+QMTSUB: 0,1,0");//订阅成功
    while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTSUB: 0,1,0");//订阅成功
    }
    Clear_Buffer();
}



void aliyunMQTT_PUBdata(u8 temp,u8 humi)
{
     u8 t_payload[200],len;
     printf("AT+QMTPUB=0,0,0,0,%s\r\n",PubTopic);//发布主题
     delay_ms(300);
     len=Mqttaliyun_Savedata(t_payload,temp,humi);
     t_payload[len]=0;
     printf("%s",t_payload);
  while((USART1->SR&0X40)==0){}//循环发送,直到发送完毕   
     USART1->DR = (u8) 0x1A;    
     delay_ms(100);
     strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUB: 0,0,0");//看下返回状态
  while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUB: 0,0,0");//看下返回状态
    }
    Clear_Buffer(); 
}



//访问阿里云需要提交JSON数据
u8 Mqttaliyun_Savedata(u8 *t_payload,u8 temp,u8 humi)
{

    char json[]="{\"id\":\"26\",\"version\":\"1.0\",\"params\":{\"Volume\":{\"value\":%d},\"DetectDistance\":{\"value\":%d}},\"method\":\"thing.event.property.post\"}";	 //     char json[]="{\"datastreams\":[{\"id\":\"location\",\"datapoints\":[{\"value\":{\"lon\":%2.6f,\"lat\":%2.6f}}]}]}";
    char t_json[200];
    unsigned short json_len;
    sprintf(t_json, json, temp, humi);
    json_len = strlen(t_json)/sizeof(char);
  	memcpy(t_payload, t_json, json_len);
    return json_len;
}

u8 Mqttaliyun_Savedata_distance1(u8 *t_payload,u8 humi)
{

    char json[]="{\"id\":\"26\",\"version\":\"1.0\",\"params\":{\"DetectDistance\":{\"value\":%d}},\"method\":\"thing.event.property.post\"}";	 //     char json[]="{\"datastreams\":[{\"id\":\"location\",\"datapoints\":[{\"value\":{\"lon\":%2.6f,\"lat\":%2.6f}}]}]}";
    char t_json[200];
    unsigned short json_len;
    sprintf(t_json, json, humi);
    json_len = strlen(t_json)/sizeof(char);
  	memcpy(t_payload, t_json, json_len);
    return json_len;
}

void aliyunMQTT_PUBdata_distance1(u8 humi)
{
     u8 t_payload[200],len;
     printf("AT+QMTPUB=0,0,0,0,%s\r\n",PubTopic);//发布主题
     delay_ms(300);
     len=Mqttaliyun_Savedata_distance1(t_payload,humi);
     t_payload[len]=0;
     printf("%s",t_payload);
  while((USART1->SR&0X40)==0){}//循环发送,直到发送完毕   
     USART1->DR = (u8) 0x1A;    
     delay_ms(100);
     strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUB: 0,0,0");//看下返回状态
  if(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUB: 0,0,0");//看下返回状态
    }
    Clear_Buffer(); 
}

void aliyunMQTT_PUBdata_distance2(u8 humi)
{
     u8 t_payload[200],len;
     printf("AT+QMTPUB=0,0,0,0,%s\r\n",PubTopic);//发布主题
     delay_ms(300);
     len=Mqttaliyun_Savedata_distance2(t_payload,humi);
     t_payload[len]=0;
     printf("%s",t_payload);
  while((USART1->SR&0X40)==0){}//循环发送,直到发送完毕   
     USART1->DR = (u8) 0x1A;    
     delay_ms(100);
     strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUB: 0,0,0");//看下返回状态
  if(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUB: 0,0,0");//看下返回状态
    }
    Clear_Buffer(); 
}

u8 Mqttaliyun_Savedata_distance2(u8 *t_payload,u8 humi)
{

    char json[]="{\"id\":\"26\",\"version\":\"1.0\",\"params\":{\"DetectDistance2\":{\"value\":%d}},\"method\":\"thing.event.property.post\"}";	 //     char json[]="{\"datastreams\":[{\"id\":\"location\",\"datapoints\":[{\"value\":{\"lon\":%2.6f,\"lat\":%2.6f}}]}]}";
    char t_json[200];
    unsigned short json_len;
    sprintf(t_json, json, humi);
    json_len = strlen(t_json)/sizeof(char);
  	memcpy(t_payload, t_json, json_len);
    return json_len;
}


u8 Mqttaliyun_Savedata_distance3(u8 *t_payload,u8 humi)
{

    char json[]="{\"id\":\"26\",\"version\":\"1.0\",\"params\":{\"DetectDistance3\":{\"value\":%d}},\"method\":\"thing.event.property.post\"}";	 //     char json[]="{\"datastreams\":[{\"id\":\"location\",\"datapoints\":[{\"value\":{\"lon\":%2.6f,\"lat\":%2.6f}}]}]}";
    char t_json[200];
    unsigned short json_len;
    sprintf(t_json, json, humi);
    json_len = strlen(t_json)/sizeof(char);
  	memcpy(t_payload, t_json, json_len);
    return json_len;
}

void aliyunMQTT_PUBdata_distance3(u8 humi)
{
     u8 t_payload[200],len;
     printf("AT+QMTPUB=0,0,0,0,%s\r\n",PubTopic);//发布主题
     delay_ms(300);
     len=Mqttaliyun_Savedata_distance3(t_payload,humi);
     t_payload[len]=0;
     printf("%s",t_payload);
  while((USART1->SR&0X40)==0){}//循环发送,直到发送完毕   
     USART1->DR = (u8) 0x1A;    
     delay_ms(100);
     strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUB: 0,0,0");//看下返回状态
  if(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUB: 0,0,0");//看下返回状态
    }
    Clear_Buffer(); 
}

u8 Mqttaliyun_Savedata_distance4(u8 *t_payload,u8 humi)
{

    char json[]="{\"id\":\"26\",\"version\":\"1.0\",\"params\":{\"DetectDistance4\":{\"value\":%d}},\"method\":\"thing.event.property.post\"}";	 //     char json[]="{\"datastreams\":[{\"id\":\"location\",\"datapoints\":[{\"value\":{\"lon\":%2.6f,\"lat\":%2.6f}}]}]}";
    char t_json[200];
    unsigned short json_len;
    sprintf(t_json, json, humi);
    json_len = strlen(t_json)/sizeof(char);
  	memcpy(t_payload, t_json, json_len);
    return json_len;
}

void aliyunMQTT_PUBdata_distance4(u8 humi)
{
     u8 t_payload[200],len;
     printf("AT+QMTPUB=0,0,0,0,%s\r\n",PubTopic);//发布主题
     delay_ms(300);
     len=Mqttaliyun_Savedata_distance4(t_payload,humi);
     t_payload[len]=0;
     printf("%s",t_payload);
  while((USART1->SR&0X40)==0){}//循环发送,直到发送完毕   
     USART1->DR = (u8) 0x1A;    
     delay_ms(100);
     strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUB: 0,0,0");//看下返回状态
  if(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUB: 0,0,0");//看下返回状态
    }
    Clear_Buffer(); 
}

u8 Mqttaliyun_Savedata_distance5(u8 *t_payload,u8 humi)
{

    char json[]="{\"id\":\"26\",\"version\":\"1.0\",\"params\":{\"RunningState\":{\"value\":%d}},\"method\":\"thing.event.property.post\"}";	 //     char json[]="{\"datastreams\":[{\"id\":\"location\",\"datapoints\":[{\"value\":{\"lon\":%2.6f,\"lat\":%2.6f}}]}]}";
    char t_json[200];
    unsigned short json_len;
    sprintf(t_json, json, humi);
    json_len = strlen(t_json)/sizeof(char);
  	memcpy(t_payload, t_json, json_len);
    return json_len;
}

void aliyunMQTT_PUBdata_distance5(u8 humi)
{
     u8 t_payload[200],len;
     printf("AT+QMTPUB=0,0,0,0,%s\r\n",PubTopic);//发布主题
     delay_ms(300);
     len=Mqttaliyun_Savedata_distance5(t_payload,humi);
     t_payload[len]=0;
     printf("%s",t_payload);
  while((USART1->SR&0X40)==0){}//循环发送,直到发送完毕   
     USART1->DR = (u8) 0x1A;    
     delay_ms(100);
     strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUB: 0,0,0");//看下返回状态
  if(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUB: 0,0,0");//看下返回状态
    }
    Clear_Buffer(); 
}

