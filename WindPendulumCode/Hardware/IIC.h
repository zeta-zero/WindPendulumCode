#ifndef __IIC_H_
#define __IIC_H_
#include "sys.h"
#if __IIC_ENABLE


#define I2CSIDE_ADDRESS 0xd0   //MPU6050��ַ
#define IIC_SCL   PAout(4)
#define IIC_SDA   PAout(5)
#define READ_SDA  PAin(5)

#define SDA_IN()  {GPIOA->MODER&=~(3<<(5*2));GPIOA->MODER|=0<<(5*2);}	//PA5����ģʽ
#define SDA_OUT() {GPIOA->MODER&=~(3<<(5*2));GPIOA->MODER|=1<<(5*2);} //PA5���ģʽ

//IIC���в�������
void IIC_SimulationConfig(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);               //����IIC��ʼ�ź�
void IIC_Stop(void);                //����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);         //IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void);              //IIC�ȴ�ACK�ź�
void IIC_Ack(void);	                //IIC����ACK�ź�
void IIC_NAck(void);                //IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte(u8 daddr,u8 addr);	
#endif  //__IIC_ENABLE
#endif  //__IIC_H_