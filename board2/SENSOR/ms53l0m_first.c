/**
  ****************************************************************************************************
  * @file       ms53l0m.c
  * @author     ����ԭ���Ŷ�(ALIENTEK)
  * @version    V1.0
  * @date       2021-04-17
  * @brief      ms53l0mģ������
  * @license   	Copyright (c) 2021-2032, ������������ӿƼ����޹�˾
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
  
#include "ms53l0m_first.h"
#include "ms53l0m.h"
#include "usart3.h"
#include "string.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "bc26.h"
#include "wdg.h"

_Tof_Info Tof_Info2;
_Tof_Data Tof_Data2;

/**
  * @brief  ����CRCУ���
  * @param   *buff �����׵�ַ
  * @param   len  ���ݳ���
  * @retval  uint16_t CRCУ���ֵ
  */
inline static uint16_t CRC_Check_Sum(uint8_t* buff, uint16_t len)
{
    uint16_t checksum=0;
    
    while(len--) checksum+=*buff++;

    return checksum ;
}

/**
  * @brief  ֡���ݰ�����
  * @param  data: ��ȡ������
  * @retval uint8_t ����״̬ 0,�ɹ� >0,ʧ��
  */
inline static uint8_t Ms53l0m_Unpack(uint16_t *data)
{  
    uint16_t i=0;
    uint8_t res =0; /*״̬����*/
    uint16_t framh_index=0;/*��¼֡ͷλ��*/
    uint16_t fram_len=0; /*֡����*/
    uint8_t oper_type=0;/*�������� 0X00-�� 0X01-д 0XFF-�쳣*/
    uint16_t data_len=0;/*��ȡ֡���ݳ���*/
    uint16_t rx_len=0; /*��ȡ���ڽ��ճ���*/
    uint16_t rxchkval=0;/*���յ���CRCУ��ֵ*/
    uint16_t calchkval=0;/*����õ���CRCУ��ֵ*/
    
    while(!Tof_Data2.rx_ok) /*�ȴ��������*/
    {
         delay_ms(1);
         i++;
        if(i==5000) /*���ճ�ʱ*/
        {
            res = PACK_RXTIMEOUT;/*���ճ�ʱ*/
            goto error;

        }
    }
    rx_len = Tof_Data2.rx_len;
    
    if((rx_len<RPACK_MIN_LEN) || rx_len>RPACK_MAX_LEN) /*���ճ����쳣*/
    {
        res=PACK_RXLEN_ERROR;/*�����쳣*/
        goto error;
    }

    i=0;
    do
    {
       /*����֡ͷλ��*/
       if((Tof_Data2.rx_buff[i]==SFRAME_H)&&(Tof_Data2.rx_buff[i+1]==SENSOR_TYPE)) break;
       if(i!=(rx_len-2))
          i++;
        else  /*�����Ҳ���֡ͷλ��*/
        {
            res=PACK_ERROR; /*֡�쳣*/
            goto error; 
        } 

    }while(1);
    
    
    framh_index = i; /*��¼֡ͷλ��*/
    fram_len = rx_len - framh_index;/*֡ͷ��β�˵ĳ��ȴ�С*/
    
    if((fram_len<RPACK_MIN_LEN) || fram_len>RPACK_MAX_LEN) /*���ճ����쳣*/
    {
        res=PACK_RXLEN_ERROR;/*�����쳣*/
        goto error;
    }
    
    oper_type =  Tof_Data2.rx_buff[framh_index+4];/*��ȡ��������*/
    
    if(oper_type == 0x00)/*������*/
    {
        
        data_len = Tof_Data2.rx_buff[framh_index+7];/*��ȡ���ݳ���*/
        
        if((data_len+10)>fram_len) /*���յ�һ֡����С�ڶ���������֡�ĳ���*/
        {
            res=PACK_ERROR; /*֡�쳣*/
            goto error;    
        }
        
        calchkval = CRC_Check_Sum(&Tof_Data2.rx_buff[framh_index],data_len+8);/*����CRCУ���*/
        rxchkval = ((u16)Tof_Data2.rx_buff[framh_index+data_len+8]<<8) + Tof_Data2.rx_buff[framh_index+data_len+9] ; /*����֡��CRCУ���*/
        
        if(calchkval == rxchkval)/*У�����ȷ*/
        {                          
              /*��ȡ����*/
              if(Tof_Data2.rx_buff[framh_index+7]==1)/*���ݳ���Ϊ1*/
                *data =  Tof_Data2.rx_buff[framh_index+8];
              else if(Tof_Data2.rx_buff[framh_index+7]==2)/*���ݳ���Ϊ2*/
                *data = ((u16)Tof_Data2.rx_buff[framh_index+8]<<8) + Tof_Data2.rx_buff[framh_index+9];  
              
              res=PACK_OK;
              
        }else 
          res=PACK_CRCERROR; /*CRC����*/


    }else if(oper_type == 0x01)/*д����*/
    {
        
         calchkval = CRC_Check_Sum(&Tof_Data2.rx_buff[framh_index],6);/*����CRCУ���*/
         rxchkval = ((u16)Tof_Data2.rx_buff[framh_index+6]<<8) + Tof_Data2.rx_buff[framh_index+7] ; /*����֡��CRCУ���*/
        
         if(calchkval == rxchkval)/*У�����ȷ*/
           res=PACK_OK;
         else 
           res=PACK_CRCERROR; /*CRC����*/  
        
        
    }else if(oper_type == 0xff)/*�쳣����*/
    {
        if((Tof_Data2.rx_buff[framh_index+2]==0XFF) && (Tof_Data2.rx_buff[framh_index+3]==0XFF))
        {
               calchkval = CRC_Check_Sum(&Tof_Data2.rx_buff[framh_index],6);/*����CRCУ���*/
               rxchkval = ((u16)Tof_Data2.rx_buff[framh_index+6]<<8) + Tof_Data2.rx_buff[framh_index+7] ; /*����֡��CRCУ���*/ 
            
               if(calchkval == rxchkval)/*У�����ȷ*/
               {
                   
                  res=PACK_EXCEPTION_CODE;/*��������*/ 
              //    printf("ERROR_CODE:%d\r\n",Tof_Data2.rx_buff[framh_index+5]); /*��ӡ������*/
                   
               }
                  
               else 
               {
                    res=PACK_CRCERROR; /*CRC����*/  
               }
                                 
        }else 
          res=PACK_ERROR; /*֡�쳣*/
      
    }else  /*�������*/
    {
        res=PACK_ERROR;/*֡�쳣*/
        
    }
    error:

/*��ӡ֡Ӧ�������*/
    
//    if(res == PACK_OK)
//    {
//        if(oper_type == 0x01)/*д����*/
//           fram_len = 8;
//        else /*������*/ 
//           fram_len = fram_len+framh_index;               
//        
//        printf("rp:");
//        for(i=framh_index; i< fram_len;i++)
//        {
//            printf("%2x ",Tof_Data2.rx_buff[i]);
//        }
//        printf("\r\n");
//    }

    Tof_Data2.rx_ok=0;
    Tof_Data2.rx_len=0;
    
    return res;
}

/**
  * @brief  ��ģ�鹦���������
  * @param  addr:�豸��ַ
  * @param  reg: �������ַ
  * @param  datalen: ���ݳ��� ֻ����1��2
  * @param  data: ��ȡ����
  * @retval uint8_t 
  */
uint8_t Ms53l0m_RData_first(uint16_t addr,uint8_t reg,uint8_t datalen,uint16_t *data)
{
    uint8_t res=0;
    uint16_t calchkval = 0;
    
    Tof_Data2.tx_buff[0] = MFRAME_H;/*֡ͷ*/
    Tof_Data2.tx_buff[1] = SENSOR_TYPE;/**/
    Tof_Data2.tx_buff[2] = (u8)(addr>>8);/*��λ��ַ*/
    Tof_Data2.tx_buff[3] = (u8)(addr & 0XFF); /*��λ��ַ*/
    Tof_Data2.tx_buff[4] = 0x00; /*������*/
    Tof_Data2.tx_buff[5] = reg; /*������*/
    Tof_Data2.tx_buff[6] = datalen; /*��ȡ�ĳ���*/
    
    calchkval =  CRC_Check_Sum(Tof_Data2.tx_buff,7);/*CRCУ��*/
    
    Tof_Data2.tx_buff[7]  = (calchkval>>8) & 0xff;  /*CRC��λǰ*/
    Tof_Data2.tx_buff[8] =  (calchkval & 0xff); /*CRC��λ��*/

/*��ӡ���͵�����*/    
//    int i=0;
//    printf("\r\nrd:");
//    for(i=0;i<9;i++)
//    {
//        printf("%2x ",Tof_Data2.tx_buff[i]);
//    }
//    printf("\r\n");
    
    Tof_Data2.rx_ok=0;
    Tof_Data2.rx_len=0;
    
    Uart_Send_first(Tof_Data2.tx_buff,9);/*���ڷ���*/
    
    res = Ms53l0m_Unpack(data);/*Ӧ�������*/
    
    return res;
    
}

/**
  * @brief  ��ģ�鹦����д����
  * @param  addr:�豸��ַ
  * @param  reg: �������ַ
  * @param  data:д������
  * @retval uint8_t Ӧ�������״̬ 
  */
uint8_t Ms53l0m_WData_first(uint16_t addr,uint8_t reg, uint8_t data)
{
    uint8_t res=0;
    uint16_t calchkval = 0;
    
    Tof_Data2.tx_buff[0] = MFRAME_H;/*֡ͷ*/
    Tof_Data2.tx_buff[1] = SENSOR_TYPE;/*����������2M TOF*/
    Tof_Data2.tx_buff[2] = (u8)(addr>>8);/*��λ��ַ*/
    Tof_Data2.tx_buff[3] = (u8)(addr & 0XFF); /*��λ��ַ*/
    Tof_Data2.tx_buff[4] = 0x01; /*д*/
    Tof_Data2.tx_buff[5] =  reg; /*������*/
    Tof_Data2.tx_buff[6] = 0x01;
    Tof_Data2.tx_buff[7] = data; /*����*/
    
    calchkval =  CRC_Check_Sum(Tof_Data2.tx_buff,8);/*CRCУ��*/
    
    Tof_Data2.tx_buff[8]  = (calchkval>>8) & 0xff;  /*CRC��λǰ*/
    Tof_Data2.tx_buff[9] =  (calchkval & 0xff); /*CRC��λ��*/

/*��ӡ���͵�����*/     
//    printf("\r\nwd:");
//    int i=0;
//    for(i=0;i<10;i++)
//    {
//        printf("%2x ",Tof_Data2.tx_buff[i]);
//    }
//    printf("\r\n");
    
    Tof_Data2.rx_ok=0;
    Tof_Data2.rx_len=0;

    Uart_Send_first(Tof_Data2.tx_buff,10);/*���ڷ���*/
    
    res = Ms53l0m_Unpack(NULL); /*Ӧ�������*/

    return res;
    
}

/**
  * @brief  ������2��ʼ��
  * @param  
  * @retval  
  */
void Ms53l0m_Init_first(void)
{
    uint16_t addr=0;
    uint8_t i=0;

    Uart_Init_first(115200);/*��������115200*/
    
    delay_ms(10);/*�ô����ȶ�*/
    
    while(Ms53l0m_RData_first(0xffff,IDSET_REG,2,&addr) && (i<10)) /*ʹ�ù㲥��ַ��ѯģ���ַ*/
    {
        delay_ms(200);
        i++;
    }
    Tof_Info2.id = addr; /*��¼�豸��ַ*/

   
    i=0;
    while(Ms53l0m_WData_first(Tof_Info2.id,WORKMODE_REG,MODBUS_WMODE) && (i<20)) /*����MODBUS����ģʽ*/
    {
       delay_ms(200);
       i++;
    }
    
    
}

/**
  * @brief  Modbusģʽ���ݻ�ȡ (��Ҫ�޸�)
  * @param  
  * @retval  
  */
void Modbus_DataGet_first(void)
{
    uint16_t data=0;
    uint8_t i=0;
    uint8_t res=0;
    
    res = Ms53l0m_WData_first(Tof_Info2.id,MEAUMODE_REG,HISPEED_DMODE); /*���ø��ٲ���ģʽ*/
    if(!res) ;
    else 
    {
        printf("����ģʽ���� ERROR:%d\r\n",res);
    }
    

       res = Ms53l0m_RData_first(Tof_Info2.id,MEAUDATA_REG,2,&data);
        
       if(!res)
       {
          printf("\r\n�ڶ��鼤��ģ�����:%d mm\r\n",data);   
         IWDG_Feed();//ι��
         aliyunMQTT_PUBdata_distance2(data);          
       }           
       
       delay_ms(100); 

       i++;
       if(i==3)
       {
          i=0; LED0=!LED0;
       }
        
}
    

