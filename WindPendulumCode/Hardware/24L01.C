#include "24L01.h"
#if __24L01_ENABLE

u8 Statu;   
u8 RX_DATA[6]="NO";

//u8 Dev0_TX[TX_ADR_WIDTH]={0x11,0x35,0x02,0xAA,0xCF};
//u8 Dev0_RX[RX_ADR_WIDTH]={0X11,0X35,0X02,0XBB,0XDE};
//u8 Dev1_TX[TX_ADR_WIDTH]={0x11,0x35,0x02,0xAA,0xCF};
//u8 Dev1_RX[RX_ADR_WIDTH]={0X11,0X35,0X02,0XBB,0XDE};
u8 Dev_TX[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x02};
u8 Dev_RX[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x02};
/***************************************************************************
�������ƣ�NRF24L01_CSN_ON
�������ܣ�NRFƬѡʹ��
������ע��
***************************************************************************/
void NRF24L01_CSN_ON(u8 X)
{
  switch(X)
	{
    case 0:NRF24L01_CSN0 = 0;break;
		case 1:NRF24L01_CSN1 = 0;break;
		default:break;
	}
}

/***************************************************************************
�������ƣ�NRF24L01_CSN_OFF
�������ܣ�NRFƬѡʧ��
������ע��
***************************************************************************/
void NRF24L01_CSN_OFF(u8 X)
{
  switch(X)
	{
    case 0:NRF24L01_CSN0 = 1;break;
		case 1:NRF24L01_CSN1 = 1;break;
		default:break;
	}
}

/***************************************************************************
�������ƣ�SPI��ʱ����
�������ܣ�NRF24L01��SPI��ʱ��
������ע��
***************************************************************************/
u8 NRF24L01_Read_Reg(u8 CMD)
{
	u8 value;
	NRF24L01_CSN0=0;              
	SPI2_ReadWriteByte(CMD);            
	value = SPI2_ReadWriteByte(0xff);  
  NRF24L01_CSN0=1;	
	
	return(value);        
}

/***************************************************************************
�������ƣ�NRF24L01��д�Ĵ�������
�������ܣ�д��NRF24L01�ļĴ����ʹ�NRF24L01�Ĵ����ж���
������ע������״̬
***************************************************************************/
u8 NRF24L01_Write_Reg(u8 CMD, u8 value)
{	         
  u8 status;	
	NRF24L01_CSN0=0;  
	status = SPI2_ReadWriteByte(CMD);     
	SPI2_ReadWriteByte(value);
  NRF24L01_CSN0=1;  
  
  return	status;
}

/***************************************************************************
�������ƣ�NRF24L01���Ĵ������ݺ���
�������ܣ���NRF24L01�Ĵ����ж�������
������ע��CMD��Ϊ�Ĵ�����ַ��pBuf��Ϊ���������ݵ�ַ��u8s���������ݵĸ���
***************************************************************************/
u8 NRF24L01_Read_Buf(u8 CMD, u8 *pBuf, u8 num)
{
	u8 status,i;
	
	
  NRF24L01_CSN0=0;							 
	status = SPI2_ReadWriteByte( CMD);       		
	for(i=0;i<num;i++)
		pBuf[i] = SPI2_ReadWriteByte(0xff);
  NRF24L01_CSN0=1;		
	
	return(status);                    
}

/***************************************************************************
�������ƣ�NRF24L01д���ݵ��Ĵ�������
�������ܣ���NRF24L01�Ĵ�����д������
������ע��CMD��Ϊ�Ĵ�����ַ��pBuf��Ϊ��д�����ݵ�ַ��u8s��д�����ݵĸ���
***************************************************************************/
u8 NRF24L01_Write_Buf(u8 CMD, u8 *pBuf, u8 num)
{
	u8 i,status;  
  NRF24L01_CSN0=0;	
	status = SPI2_ReadWriteByte(CMD);   
	for(i=0; i<num; i++)  
		SPI2_ReadWriteByte(*pBuf++);
	NRF24L01_CSN0=1;
	return status;
}

/***************************************************************************
�������ƣ�NRF24L01���պ���
�������ܣ����ݶ�ȡ�����rx_buf���ջ�������
������ע��
***************************************************************************/
u8 NRF24L01_RxPacket(u8* rx_buf)
{
  u8 flag = 0;
	NRF24L01_CSN0=0;
	NRF24L01_CE=0;
	Delay_ms(1);
	Statu=NRF24L01_Read_Reg(RF_READ_REG + STATUS);                      // ��ȡ״̬�Ĵ������ж����ݽ���״��	
	NRF24L01_Write_Reg(RF_WRITE_REG+STATUS,Statu); //���TX_DS��MAX_RT�жϱ�־
	if(RX_DR)						                           // �ж��Ƿ���յ�����  �����1��˵���ӵ����ݲ��ҷ����ڽ��ջ�����
	{
		NRF24L01_Read_Buf(R_RX_PAYLOAD,rx_buf,RX_PAYLOAD_WIDTH); //ȥ���������
		flag =1;			                               	           //��ȡ������ɱ�־
	}
	
	NRF24L01_Write_Reg(FLUSH_RX,0xff);                       //���RX FIFO�Ĵ��� 
	NRF24L01_CSN0=1;
  NRF24L01_CE=1;
	return flag;
}

/***************************************************************************
�������ƣ�NRF24L01���亯��
�������ܣ�����tx_buf������
������ע��
***************************************************************************/
u8 NRF24L01_TxPacket(unsigned char * tx_buf)
{
	NRF24L01_CE=0;
	NRF24L01_CSN0=0;
	NRF24L01_Write_Reg(FLUSH_TX,0XFF);
	NRF24L01_Write_Buf(W_TX_PAYLOAD, tx_buf, TX_PAYLOAD_WIDTH);       //װ������  �����ݷ��뷢�ͻ�����	
	NRF24L01_CSN0=0;
	NRF24L01_CE=1;
		while(NRF24L01_IRQ0 !=0);                                                    //�ȴ��������
	Statu=NRF24L01_Read_Reg(STATUS);                                           //��ȡ״̬�Ĵ�����ֵ	   
    NRF24L01_Write_Reg(RF_WRITE_REG | STATUS,Statu);                     //���TX_DS��MAX_RT�жϱ�־
    if(Statu&0x10)//�ﵽ����ط�����
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);//���TX FIFO�Ĵ��� 
		return 1; 
	}                                                      //ʹNRF24L01���ڹ���ģʽ
	if(Statu&TX_OK)//�������
	{
		return TX_OK;
	}
	return 0;
} 

/***************************************************************************
�������ƣ�NRF24L01����ģʽ
�������ܣ���ʼ��NRF24L01
������ע���ڵ���NRF24L01��Ϊ����ʱ����Ҫ�ȵ��øú�����NRF24L01���г�ʼ��
***************************************************************************/
void NRF24L01_RX_Mode(u8 *_RX_ADDRESS)
{	
	SPI2_SetSpeed(SPI_BaudRatePrescaler_4); 
	NRF24L01_CE=0;
	NRF24L01_Write_Buf(RF_WRITE_REG + RX_ADDR_P0, _RX_ADDRESS, RX_ADR_WIDTH);    //д���ն˵�ַ
	NRF24L01_Write_Reg(RF_WRITE_REG + EN_AA, ENAA_P0);                          //Ƶ��0�Զ�	ACKӦ������	  ҪӦ��˵����Ҫ���÷��Ͳ���Ӧ��
	NRF24L01_Write_Reg(RF_WRITE_REG + EN_RXADDR, ERX_P0);                       //�������յ�ַֻ��Ƶ��0��
	NRF24L01_Write_Reg(RF_WRITE_REG + RF_CH, 40);                               //�����ŵ�����Ϊ2.4GHZ + 40���շ�����һ�£�
	NRF24L01_Write_Reg(RF_WRITE_REG + RX_PW_P0, RX_PAYLOAD_WIDTH);              //���ý������ݳ��ȣ���������Ϊ32�ֽ�
	NRF24L01_Write_Reg(RF_WRITE_REG + RF_SETUP, 0X0F);   					     //���÷�������Ϊ2MHZ�����书��Ϊ���ֵ18dB
	NRF24L01_Write_Reg(RF_WRITE_REG + CONFIG, 0x0F);   		                 //IRQ�շ�����ж���Ӧ��16λCRC	��������	
	NRF24L01_CE=1;
}

/***************************************************************************
�������ƣ�NRF24L01����ģʽ
�������ܣ���ʼ��NRF24L01
������ע���ڵ���NRF24L01��Ϊ����ʱ����Ҫ�ȵ��øú�����NRF24L01���г�ʼ��
***************************************************************************/
void NRF24L01_TX_Mode(u8 *_TX_ADDRESS,u8 *_RX_ADDRESS)
{
	SPI2_SetSpeed(SPI_BaudRatePrescaler_4); 
	NRF24L01_CE=0;
	NRF24L01_Write_Buf(RF_WRITE_REG + TX_ADDR, _TX_ADDRESS, TX_ADR_WIDTH);       //д���ص�ַ	
	NRF24L01_Write_Buf(RF_WRITE_REG + RX_ADDR_P0, _RX_ADDRESS, RX_ADR_WIDTH);    //д���ն˵�ַ
	
	NRF24L01_Write_Reg(RF_WRITE_REG + EN_AA, ENAA_P0);                          //Ƶ��0�Զ�	ACKӦ������	  ҪӦ��˵����Ҫ���÷��Ͳ���Ӧ��
	NRF24L01_Write_Reg(RF_WRITE_REG + EN_RXADDR, ERX_P0);                       //�������յ�ַֻ��Ƶ��0��
	NRF24L01_Write_Reg(RF_WRITE_REG + SETUP_RETR, 0x1a);				    	 // 500us + 86us, 10���ط�
	NRF24L01_Write_Reg(RF_WRITE_REG + RF_CH, 40);                               //�����ŵ�����Ϊ2.4GHZ + 40���շ�����һ�£�
	NRF24L01_Write_Reg(RF_WRITE_REG + RX_PW_P0, RX_PAYLOAD_WIDTH);              //���ý������ݳ��ȣ���������Ϊ32�ֽ�
	NRF24L01_Write_Reg(RF_WRITE_REG + RF_SETUP, 0X0F);   					     //���÷�������Ϊ2MHZ�����书��Ϊ���ֵ18dB
	NRF24L01_Write_Reg(RF_WRITE_REG + CONFIG, 0x0E);   		                 //IRQ�շ�����ж���Ӧ��16λCRC	��������	
	NRF24L01_CE=1;
}


/***************************************************************************
�������ƣ�NRF24L01��ʼ������
�������ܣ���ʼ��NRF24L01
������ע���ڵ���NRF24L01��Ϊ����ʱ����Ҫ�ȵ��øú�����NRF24L01���г�ʼ��
***************************************************************************/
void NRF24L01_Init(void)
{  
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOB,Gʱ��
	
  //GPIOB14��ʼ������:�������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��PB14
	
	//GPIOG6,7�������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��PG6,7
	
	//GPIOG.8��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//����
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��PE5,6
	
	SPIx2_Init();
	SPI2_SetSpeed(SPI_BaudRatePrescaler_4);                                           //spi�ٶ�Ϊ10.5/2Mhz��24L01�����SPIʱ��Ϊ10Mhz��   
	
 	NRF24L01_CE   =0;   														                                  //ʹNRF24L01���ڿ���ģʽ
 	NRF24L01_CSN0 =1;                                                                 //ʧ��NRF24L01
//	NRF24L01_TX_Mode(Dev_TX,Dev_RX);
//  NRF24L01_RX_Mode(Dev_RX);
	NRF24L01_CE   =1; 

}

/***************************************************************************
�������ƣ�NRF24���Ժ���
�������ܣ����ڲ���NRF24�Ƿ�����
������ע������ 1 ��ʾʧ�ܣ����� 0 ��ʾ�ɹ�
***************************************************************************/
u8 NRF24L01_Check(void)
{
  u8 buf[5]={0xA5,0xA5,0xA5,0xA5,0xA5};
  u8 i;
	SPI2_SetSpeed(SPI_BaudRatePrescaler_4); 
  NRF24L01_Write_Buf(RF_WRITE_REG + TX_ADDR ,buf,5);
  NRF24L01_Read_Buf(TX_ADDR,buf,5);  
  for(i=0;i<5;i++)
  {
    if(buf[i]!=0xA5)
       break;        
  }                
  if(i!=5)
    return 1;        
  else
		return 0;   	
}




#endif  //____24L01_ENABLE