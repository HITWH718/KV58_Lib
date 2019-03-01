#include "iic.h"

volatile I2C_Type *I2Cx[2] = I2C_BASE_PTRS; //��������ָ�����鱣�� I2Cx �ĵ�ַ

//�����ź�
#define I2C_Start(I2Cn)             ((I2Cx[I2Cn])->C1) |= I2C_C1_TX_MASK+I2C_C1_MST_MASK    //MST ��0��1��������ʼ�źţ�TX = 1 ���뷢��ģʽ
                                    
//��ͣ�ź�
#define I2C_Stop(I2Cn)              ((I2Cx[I2Cn])->C1) &= ~(I2C_C1_MST_MASK+I2C_C1_TX_MASK) //MST ��1��0������ֹͣ�źţ�TX = 0 �������ģʽ
                                    
//�ظ�����
#define I2C_RepeatedStart(I2Cn)     ((I2Cx[I2Cn])->C1) |= I2C_C1_RSTA_MASK

//�������ģʽ(Ӧ��,��Ҫ���ն�����ݣ��������һ���ֽ�ǰ��Ҫ����Ӧ��I2C_DisableAck)
#define I2C_EnterRxMode(I2Cn)       ((I2Cx[I2Cn])->C1) &= ~(I2C_C1_TX_MASK | I2C_C1_TXAK_MASK)
                                    
//�������ģʽ(��Ӧ��)
#define I2C_PutinRxMode(I2Cn)       ((I2Cx[I2Cn])->C1) &= ~I2C_C1_TX_MASK; ((I2Cx[I2Cn])->C1) |= I2C_C1_TXAK_MASK

//����Ӧ��(�������һ���ֽ�)
#define I2C_DisableAck(I2Cn)        ((I2Cx[I2Cn])->C1) |= I2C_C1_TXAK_MASK

///�ȴ� I2C0_S
#define I2C_Wait(I2Cn)              while(( ((I2Cx[I2Cn])->S) & I2C_S_IICIF_MASK)==0) {} \
                                    ((I2Cx[I2Cn])->S) |= I2C_S_IICIF_MASK;

//дһ���ֽ�
#define I2C_write_byte(I2Cn,data)   (((I2Cx[I2Cn])->D) = (data));I2C_Wait(I2Cn)


//-------------------------------------------------------------------------*
//������: IIC_init                                                        
//��  ��: ��ʼ��IIC                                                       
//��  ��: i2cn:�˿��� I2C0,I2C1                                                                                     
//��  ��: ��                                                              
//��  ��: IIC_init(I2C1);              
//-------------------------------------------------------------------------*
void I2C_Init(I2Cn i2cn)
{    
  if(i2cn == I2C_0)
  {
    /* ����ʱ�� */
    SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;         //���� I2C0ʱ��
    
    /* ���� I2C0���ܵ� GPIO �ӿ� */
    if(I2C0_SCL == PTB0)
    {  
      PORT_PCR_REG(PORTB, 0) = PORT_PCR_MUX(2);
      PORT_PCR_REG(PORTB, 0) = PORT_PCR_REG(PORTB, 0)|(1<<5);//����Ϊ��©ģʽ
    }
    else if(I2C0_SCL == PTB2)
    {
      PORT_PCR_REG(PORTB, 2) = PORT_PCR_MUX(2);
      PORT_PCR_REG(PORTB, 2) = PORT_PCR_REG(PORTB, 2)|(1<<5);//����Ϊ��©ģʽ
    }
    else if(I2C0_SCL == PTD8)
    {
      PORT_PCR_REG(PORTD, 8) = PORT_PCR_MUX(2);
      PORT_PCR_REG(PORTD, 8) = PORT_PCR_REG(PORTD, 8)|(1<<5);//����Ϊ��©ģʽ
    }
    else if(I2C0_SCL == PTE19)
    {
      PORT_PCR_REG(PORTE, 19) = PORT_PCR_MUX(4);
      PORT_PCR_REG(PORTE, 19) = PORT_PCR_REG(PORTE, 19)|(1<<5);//����Ϊ��©ģʽ
    }
    else if(I2C0_SCL == PTE24)
    {
      PORT_PCR_REG(PORTE, 24) = PORT_PCR_MUX(5);
      PORT_PCR_REG(PORTE, 24) = PORT_PCR_REG(PORTE, 24)|(1<<5);//����Ϊ��©ģʽ
    }
    else if(I2C0_SCL == PTA12)
    {
      PORT_PCR_REG(PORTA, 12) = PORT_PCR_MUX(8);
      PORT_PCR_REG(PORTA, 12) = PORT_PCR_REG(PORTA, 12)|(1<<5);//����Ϊ��©ģʽ
    }
    else if(I2C0_SCL == PTC6)
    {
      PORT_PCR_REG(PORTC, 6) = PORT_PCR_MUX(7);
      PORT_PCR_REG(PORTC, 6) = PORT_PCR_REG(PORTC, 6)|(1<<5);//����Ϊ��©ģʽ
    }
    else if(I2C0_SCL == PTC14)
    {
      PORT_PCR_REG(PORTC, 14) = PORT_PCR_MUX(3);
      PORT_PCR_REG(PORTC, 14) = PORT_PCR_REG(PORTC, 14)|(1<<5);//����Ϊ��©ģʽ
    }
    else if(I2C0_SCL == PTD2)
    {
      PORT_PCR_REG(PORTD, 2) = PORT_PCR_MUX(7);
      PORT_PCR_REG(PORTD, 2) = PORT_PCR_REG(PORTD, 2)|(1<<5);//����Ϊ��©ģʽ
    }
    
    
    if(I2C0_SDA == PTB1)
    {
      PORT_PCR_REG(PORTB, 1) = PORT_PCR_MUX(2);
      PORT_PCR_REG(PORTB, 1) = PORT_PCR_REG(PORTB, 1)|(1<<5);//����Ϊ��©ģʽ
    }
    else if(I2C0_SDA == PTB3)
    {  
      PORT_PCR_REG(PORTB, 3) = PORT_PCR_MUX(2);
      PORT_PCR_REG(PORTB, 3) = PORT_PCR_REG(PORTB, 3)|(1<<5);//����Ϊ��©ģʽ
    }
    else if(I2C0_SDA == PTD9)
    {
      PORT_PCR_REG(PORTD, 9) = PORT_PCR_MUX(2);
      PORT_PCR_REG(PORTD, 9) = PORT_PCR_REG(PORTD, 9)|(1<<5);//����Ϊ��©ģʽ
    }  
    else if(I2C0_SDA == PTE18)
    {
      PORT_PCR_REG(PORTE, 18) = PORT_PCR_MUX(4);
      PORT_PCR_REG(PORTE, 18) = PORT_PCR_REG(PORTE, 18)|(1<<5);//����Ϊ��©ģʽ
    } 
    else if(I2C0_SDA == PTE25)
    {
      PORT_PCR_REG(PORTE, 25) = PORT_PCR_MUX(5);
      PORT_PCR_REG(PORTE, 25) = PORT_PCR_REG(PORTE, 25)|(1<<5);//����Ϊ��©ģʽ
    } 
    else if(I2C0_SDA == PTA11)
    {
      PORT_PCR_REG(PORTA, 11) = PORT_PCR_MUX(8);
      PORT_PCR_REG(PORTA, 11) = PORT_PCR_REG(PORTA, 11)|(1<<5);//����Ϊ��©ģʽ
    }
    else if(I2C0_SDA == PTC7)
    {
      PORT_PCR_REG(PORTC, 7) = PORT_PCR_MUX(8);
      PORT_PCR_REG(PORTC, 7) = PORT_PCR_REG(PORTC, 7)|(1<<5);//����Ϊ��©ģʽ
    }
    else if (I2C0_SDA == PTC15)
    {
      PORT_PCR_REG(PORTC, 15) = PORT_PCR_MUX(3);
      PORT_PCR_REG(PORTC, 15) = PORT_PCR_REG(PORTC, 15) | (1<<5);//����Ϊ��©ģʽ
    }
    else if (I2C0_SDA == PTD3)
    {
      PORT_PCR_REG(PORTD, 3) = PORT_PCR_MUX(7);
      PORT_PCR_REG(PORTD, 3) = PORT_PCR_REG(PORTD, 3) | (1<<5);//����Ϊ��©ģʽ
    }
  }
  else
  {
    /* ����ʱ�� */
    SIM_SCGC4 |= SIM_SCGC4_I2C1_MASK;         //���� I2C1ʱ��
    
    /* ���� I2C1���ܵ� GPIO �ӿ� */
    if(I2C1_SCL == PTE1)
    { 
      PORT_PCR_REG(PORTE, 1) = PORT_PCR_MUX(6);
      PORT_PCR_REG(PORTE, 1) = PORT_PCR_REG(PORTE, 1)|(1<<5);//����Ϊ��©ģʽ
    }
    else if(I2C1_SCL == PTC10)
    {
      PORT_PCR_REG(PORTC, 10) = PORT_PCR_MUX(2);
      PORT_PCR_REG(PORTC, 10) = PORT_PCR_REG(PORTC, 10)|(1<<5);//����Ϊ��©ģʽ
    }
    else if(I2C1_SCL == PTC14)
    {
      PORT_PCR_REG(PORTC, 14) = PORT_PCR_MUX(2);
      PORT_PCR_REG(PORTC, 14) = PORT_PCR_REG(PORTC, 14)|(1<<5);//����Ϊ��©ģʽ
    }
    else if(I2C1_SCL == PTA14)
    {
      PORT_PCR_REG(PORTA, 14) = PORT_PCR_MUX(8);
      PORT_PCR_REG(PORTA, 14) = PORT_PCR_REG(PORTA, 14)|(1<<5);//����Ϊ��©ģʽ
    }
    
    if(I2C1_SDA == PTE0)
    { 
      PORT_PCR_REG(PORTE, 0) = PORT_PCR_MUX(6);
      PORT_PCR_REG(PORTE, 0) = PORT_PCR_REG(PORTE, 0) | (1<<5);//���óɿ�©ģʽ   ֻ����SDA���ɣ�����������Ҳû��
    }
    else if (I2C1_SDA == PTC11)
    {
      PORT_PCR_REG(PORTC, 11) = PORT_PCR_MUX(2);
      PORT_PCR_REG(PORTC, 11) = PORT_PCR_REG(PORTC, 11) | (1<<5);//����Ϊ��©ģʽ
    }
    else if (I2C1_SDA == PTC15)
    {
      PORT_PCR_REG(PORTC, 15) = PORT_PCR_MUX(2);
      PORT_PCR_REG(PORTC, 15) = PORT_PCR_REG(PORTC, 15) | (1<<5);//����Ϊ��©ģʽ
    }
    else if (I2C1_SDA == PTA13)
    {
      PORT_PCR_REG(PORTA, 13) = PORT_PCR_MUX(8);
      PORT_PCR_REG(PORTA, 13) = PORT_PCR_REG(PORTA, 13) | (1<<5);//����Ϊ��©ģʽ
    } 
  }
  
  /* ����Ƶ�� */
  ((I2Cx[i2cn])->F)  = I2C_F_MULT(2) | I2C_F_ICR(0x17) ; 
  // MULT=00  ��  mul = 1
  // ICR =14  ICRΪClock rateʱ��Ƶ��
  // �ӡ�k16 reference manual.pdf��P1460 �ɵã�
  // ICR    SCL Divider   SDA Hold Value    SCL Hold (Start) Value    SCL Hold (Stop) Value
  //  29       384              33                   190                        193
  //  17       128              21                   58                         65
  // I2C baud rate = bus speed (Hz)/(mul �� SCL divider)  ������ 135MHz/(4��160)=210.9375kHz
  
  /* ʹ�� IIC1 */
  ((I2Cx[i2cn])->C1) = I2C_C1_IICEN_MASK;
}

//-------------------------------------------------------------------------*
//������: I2C_StartTransmission                                                        
//��  ��: ����IIC����                                                        
//��  ��: i2cn    :�˿��� I2C0,I2C1 
//        SlaveID :�ӻ���ַ 
//        Mode    :����ģʽ
//��  ��: ��                                                              
//��  ��: IIC_init(I2C1);              
//-------------------------------------------------------------------------*
void I2C_StartTransmission (I2Cn i2cn, uint8_t SlaveID, MSmode Mode)
{
  SlaveID = ( SlaveID << 1 ) | Mode ;            //ȷ��д��ַ�Ͷ���ַ
  
  /* send start signal */
  I2C_Start(i2cn);
  
  /* send ID with W/R bit */
  I2C_write_byte(i2cn, SlaveID);
}

//-------------------------------------------------------------------------*
//������: Pause                                                        
//��  ��: ��ʱ                                                        
//��  ��: ��
//��  ��: ��                                                              
//��  ��: Pause;              
//-------------------------------------------------------------------------*
void Pause(void)
{
  uint16_t n;
  for(n = 1; n < 50000; n++)      //����̫С
  {
    asm("nop");
  }
}


//-------------------------------------------------------------------------*
//������: I2C_ReadAddr                                                        
//��  ��: ��ȡIIC�豸ָ����ַ�Ĵ���������                                                        
//��  ��: i2cn    :�˿��� I2C0,I2C1 
//        SlaveID :�ӻ���ַ 
//        Addr    :�ӻ��ļĴ�����ַ
//��  ��: result                                                              
//��  ��: p[0]  = I2C_ReadAddr(i2cn,SlaveID,OUT_X_MSB_REG);              
//-------------------------------------------------------------------------*
uint8_t I2C_ReadAddr(I2Cn i2cn, uint8_t SlaveID, uint8_t Addr)
{
  uint8_t result;
  
  /* Send Slave Address */
  I2C_StartTransmission (i2cn, SlaveID, write);
  I2C_Wait(i2cn);
  
  /* Write Register Address */
  I2C_write_byte(i2cn, Addr);
  I2C_Wait(i2cn);
  
  /* Do a repeated start */
  I2C_RepeatedStart(i2cn);
  
  /* Send Slave Address */
  I2C_write_byte(i2cn, ( SlaveID << 1) | read );
  I2C_Wait(i2cn);
  
  /* Put in Rx Mode */
  I2C_PutinRxMode(i2cn);
  
  /* Turn off ACK since this is second to last byte being read*/
  I2C_DisableAck(i2cn); //��Ӧ��
  
  /* Dummy read ��ٶ�ȡ*/
  result = ((I2Cx[i2cn])->D);
  I2C_Wait(i2cn);
  
  /* Send stop since about to read last byte */
  I2C_Stop(i2cn);
  
  /* Read byte */
  result = ((I2Cx[i2cn])->D);
  
  return result;
}


//-------------------------------------------------------------------------*
//������: I2C_ReadAddr                                                        
//��  ��: ��ȡIIC�豸ָ����ַ�Ĵ���������                                                        
//��  ��: i2cn    :�˿��� I2C0,I2C1 
//        SlaveID :�ӻ���ַ 
//        Addr    :�ӻ��ļĴ�����ַ
//        Data    :����
//��  ��: ��                                                              
//��  ��: I2C_WriteAddr(I2C1, SlaveAddress2100, CTRL_REG1_2100, 0x02);             
//-------------------------------------------------------------------------*
void I2C_WriteAddr(I2Cn i2cn, uint8_t SlaveID, uint8_t Addr, uint8_t Data)
{
  /* send data to slave */
  I2C_StartTransmission(i2cn, SlaveID, write);    //��������
  I2C_Wait(i2cn);
  
  I2C_write_byte(i2cn, Addr);                    //д��ַ
  I2C_Wait(i2cn);
  
  I2C_write_byte(i2cn, Data);                    //д����
  I2C_Wait(i2cn);
  
  I2C_Stop(i2cn);
  
  Pause();                                        //��ʱ̫�̵Ļ�������д����
}
void I2C_WriteAddr16(I2Cn i2cn, uint8_t SlaveID, uint8_t Addr, uint16_t Data)
{
  /* send data to slave */
  I2C_StartTransmission(i2cn, SlaveID, write);    //��������
  I2C_Wait(i2cn);
  
  I2C_write_byte(i2cn, Addr);                    //д��ַ
  I2C_Wait(i2cn);
  
  I2C_write_byte(i2cn, (uint8_t)(Data>>8));                    //д����
  I2C_Wait(i2cn);
  I2C_write_byte(i2cn, (uint8_t)Data);                    //д����
  I2C_Wait(i2cn);
  
  I2C_Stop(i2cn);
  
  Pause();                                        //��ʱ̫�̵Ļ�������д����
}

