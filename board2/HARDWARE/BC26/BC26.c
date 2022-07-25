#include "bc26.h"
#include "string.h"
#include "usart.h"
#include "wdg.h"
#include "led.h"
char *strx,*extstrx;
extern unsigned char  RxBuffer[255],RxCounter;
BC26 BC26_Status;
////�����Ƶ���Ԫ��,���豸���涼���Բ�ѯ��//////////////////////
#define ProductKey     "gta2hCyBCZR"             //��ƷKEY
#define DeviceName          "dmxt001"      //
#define DeviceSecret    "55148c5cfbd01f22126e17c08e2c8b51"  //
#define PubTopic         "/sys/gta2hCyBCZR/dmxt001/thing/event/property/post"//��������
#define SubTopic        "/gta2hCyBCZR/dmxt001/user/get"//��������
void Clear_Buffer(void)//��ջ���
{
		u8 i;
		Uart1_SendStr((char*)RxBuffer);
		for(i=0;i<100;i++)
		RxBuffer[i]=0;//����
		RxCounter=0;
		IWDG_Feed();//ι��
	
}

void  AT_Init(void)
{
	Clear_Buffer();
    printf("AT\r\n");
    delay_ms(500);
    printf("AT\r\n");
    delay_ms(500);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    while(strx==NULL)
    {
        Clear_Buffer();
        printf("AT\r\n");
        delay_ms(500);
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    }
    printf("ATE0\r\n"); //�رջ���
    delay_ms(500);
    Clear_Buffer();
    printf("AT+CSQ\r\n"); //���CSQ
    delay_ms(500);
    
    printf("AT+CPIN?\r\n");//���SIM���Ƿ���λ
    delay_ms(500);
    strx=strstr((const char*)RxBuffer,(const char*)"+CPIN: READY");//�鿴�Ƿ񷵻�ready
    while(strx==NULL)
    {
        Clear_Buffer();
        printf("AT+CPIN?\r\n");
        delay_ms(500);
        strx=strstr((const char*)RxBuffer,(const char*)"+CPIN: READY");//���SIM���Ƿ���λ���ȴ�����λ�������ʶ�𲻵���ʣ��Ĺ�����û������
    }
    Clear_Buffer();
    ///
    printf("AT+QMTCLOSE=0\r\n");//�Ͽ�MQTT����
    delay_ms(500);
}

void  MQTT_Init(void)
{
	Clear_Buffer();
    printf("AT+CGPADDR=1\r\n");//�鿴�Ƿ��ȡ��IP
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
    printf("AT+QMTCFG=\"aliauth\",0,\"gta2hCyBCZR\",\"dmxt001\",\"55148c5cfbd01f22126e17c08e2c8b51\"\r\n");//���ð������豸��Ϣ
    delay_ms(500);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");
    while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"OK");
    }
    Clear_Buffer();	
    printf("AT+QMTOPEN=0,\"iot-as-mqtt.cn-shanghai.aliyuncs.com\",1883\r\n");  //���Ӱ����Ʒ�������
    delay_ms(500);
    strx=strstr((const char*)RxBuffer,(const char*)"+QMTOPEN: 0,0");
    while(strx==NULL)
    {
		delay_ms(500);
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTOPEN: 0,0");
    }
    Clear_Buffer();
    printf("AT+QMTCONN=0,\"BC26\"\r\n");//�ԽӰ�����������ƽ̨�������豸
    delay_ms(500);
    strx=strstr((const char*)RxBuffer,(const char*)"+QMTCONN: 0,0,0");//���·���״̬
    while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTCONN: 0,0,0");//���·���״̬
    }
    Clear_Buffer();
    printf("AT+QMTSUB=0,1,\"/gta2hCyBCZR/BC26/user/get\",0\r\n");//���ĸ�����
    delay_ms(500);
    strx=strstr((const char*)RxBuffer,(const char*)"+QMTSUB: 0,1,0");//���ĳɹ�
    while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTSUB: 0,1,0");//���ĳɹ�
    }
    Clear_Buffer();
}



void aliyunMQTT_PUBdata(u8 temp,u8 humi)
{
     u8 t_payload[200],len;
     printf("AT+QMTPUB=0,0,0,0,%s\r\n",PubTopic);//��������
     delay_ms(300);
     len=Mqttaliyun_Savedata(t_payload,temp,humi);
     t_payload[len]=0;
     printf("%s",t_payload);
  while((USART1->SR&0X40)==0){}//ѭ������,ֱ���������   
     USART1->DR = (u8) 0x1A;    
     delay_ms(100);
     strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUB: 0,0,0");//���·���״̬
  while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUB: 0,0,0");//���·���״̬
    }
    Clear_Buffer(); 
}



//���ʰ�������Ҫ�ύJSON����
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
     printf("AT+QMTPUB=0,0,0,0,%s\r\n",PubTopic);//��������
     delay_ms(300);
     len=Mqttaliyun_Savedata_distance1(t_payload,humi);
     t_payload[len]=0;
     printf("%s",t_payload);
  while((USART1->SR&0X40)==0){}//ѭ������,ֱ���������   
     USART1->DR = (u8) 0x1A;    
     delay_ms(100);
     strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUB: 0,0,0");//���·���״̬
  if(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUB: 0,0,0");//���·���״̬
    }
    Clear_Buffer(); 
}

void aliyunMQTT_PUBdata_distance2(u8 humi)
{
     u8 t_payload[200],len;
     printf("AT+QMTPUB=0,0,0,0,%s\r\n",PubTopic);//��������
     delay_ms(300);
     len=Mqttaliyun_Savedata_distance2(t_payload,humi);
     t_payload[len]=0;
     printf("%s",t_payload);
  while((USART1->SR&0X40)==0){}//ѭ������,ֱ���������   
     USART1->DR = (u8) 0x1A;    
     delay_ms(100);
     strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUB: 0,0,0");//���·���״̬
  if(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUB: 0,0,0");//���·���״̬
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
     printf("AT+QMTPUB=0,0,0,0,%s\r\n",PubTopic);//��������
     delay_ms(300);
     len=Mqttaliyun_Savedata_distance3(t_payload,humi);
     t_payload[len]=0;
     printf("%s",t_payload);
  while((USART1->SR&0X40)==0){}//ѭ������,ֱ���������   
     USART1->DR = (u8) 0x1A;    
     delay_ms(100);
     strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUB: 0,0,0");//���·���״̬
  if(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUB: 0,0,0");//���·���״̬
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
     printf("AT+QMTPUB=0,0,0,0,%s\r\n",PubTopic);//��������
     delay_ms(300);
     len=Mqttaliyun_Savedata_distance4(t_payload,humi);
     t_payload[len]=0;
     printf("%s",t_payload);
  while((USART1->SR&0X40)==0){}//ѭ������,ֱ���������   
     USART1->DR = (u8) 0x1A;    
     delay_ms(100);
     strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUB: 0,0,0");//���·���״̬
  if(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUB: 0,0,0");//���·���״̬
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
     printf("AT+QMTPUB=0,0,0,0,%s\r\n",PubTopic);//��������
     delay_ms(300);
     len=Mqttaliyun_Savedata_distance5(t_payload,humi);
     t_payload[len]=0;
     printf("%s",t_payload);
  while((USART1->SR&0X40)==0){}//ѭ������,ֱ���������   
     USART1->DR = (u8) 0x1A;    
     delay_ms(100);
     strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUB: 0,0,0");//���·���״̬
  if(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUB: 0,0,0");//���·���״̬
    }
    Clear_Buffer(); 
}

