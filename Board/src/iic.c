#include "iic.h"

volatile I2C_Type *I2Cx[2] = I2C_BASE_PTRS; //定义两个指针数组保存 I2Cx 的地址

//启动信号
#define I2C_Start(I2Cn)             ((I2Cx[I2Cn])->C1) |= I2C_C1_TX_MASK+I2C_C1_MST_MASK    //MST 由0变1，产生起始信号，TX = 1 进入发送模式
                                    
//暂停信号
#define I2C_Stop(I2Cn)              ((I2Cx[I2Cn])->C1) &= ~(I2C_C1_MST_MASK+I2C_C1_TX_MASK) //MST 由1变0，产生停止信号，TX = 0 进入接收模式
                                    
//重复启动
#define I2C_RepeatedStart(I2Cn)     ((I2Cx[I2Cn])->C1) |= I2C_C1_RSTA_MASK

//进入接收模式(应答,需要接收多个数据，接收最后一个字节前需要禁用应答I2C_DisableAck)
#define I2C_EnterRxMode(I2Cn)       ((I2Cx[I2Cn])->C1) &= ~(I2C_C1_TX_MASK | I2C_C1_TXAK_MASK)
                                    
//进入接收模式(不应答)
#define I2C_PutinRxMode(I2Cn)       ((I2Cx[I2Cn])->C1) &= ~I2C_C1_TX_MASK; ((I2Cx[I2Cn])->C1) |= I2C_C1_TXAK_MASK

//禁用应答(接收最后一个字节)
#define I2C_DisableAck(I2Cn)        ((I2Cx[I2Cn])->C1) |= I2C_C1_TXAK_MASK

///等待 I2C0_S
#define I2C_Wait(I2Cn)              while(( ((I2Cx[I2Cn])->S) & I2C_S_IICIF_MASK)==0) {} \
                                    ((I2Cx[I2Cn])->S) |= I2C_S_IICIF_MASK;

//写一个字节
#define I2C_write_byte(I2Cn,data)   (((I2Cx[I2Cn])->D) = (data));I2C_Wait(I2Cn)


//-------------------------------------------------------------------------*
//函数名: IIC_init                                                        
//功  能: 初始化IIC                                                       
//参  数: i2cn:端口名 I2C0,I2C1                                                                                     
//返  回: 无                                                              
//简  例: IIC_init(I2C1);              
//-------------------------------------------------------------------------*
void I2C_Init(I2Cn i2cn)
{    
  if(i2cn == I2C_0)
  {
    /* 开启时钟 */
    SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;         //开启 I2C0时钟
    
    /* 配置 I2C0功能的 GPIO 接口 */
    if(I2C0_SCL == PTB0)
    {  
      PORT_PCR_REG(PORTB, 0) = PORT_PCR_MUX(2);
      PORT_PCR_REG(PORTB, 0) = PORT_PCR_REG(PORTB, 0)|(1<<5);//设置为开漏模式
    }
    else if(I2C0_SCL == PTB2)
    {
      PORT_PCR_REG(PORTB, 2) = PORT_PCR_MUX(2);
      PORT_PCR_REG(PORTB, 2) = PORT_PCR_REG(PORTB, 2)|(1<<5);//设置为开漏模式
    }
    else if(I2C0_SCL == PTD8)
    {
      PORT_PCR_REG(PORTD, 8) = PORT_PCR_MUX(2);
      PORT_PCR_REG(PORTD, 8) = PORT_PCR_REG(PORTD, 8)|(1<<5);//设置为开漏模式
    }
    else if(I2C0_SCL == PTE19)
    {
      PORT_PCR_REG(PORTE, 19) = PORT_PCR_MUX(4);
      PORT_PCR_REG(PORTE, 19) = PORT_PCR_REG(PORTE, 19)|(1<<5);//设置为开漏模式
    }
    else if(I2C0_SCL == PTE24)
    {
      PORT_PCR_REG(PORTE, 24) = PORT_PCR_MUX(5);
      PORT_PCR_REG(PORTE, 24) = PORT_PCR_REG(PORTE, 24)|(1<<5);//设置为开漏模式
    }
    else if(I2C0_SCL == PTA12)
    {
      PORT_PCR_REG(PORTA, 12) = PORT_PCR_MUX(8);
      PORT_PCR_REG(PORTA, 12) = PORT_PCR_REG(PORTA, 12)|(1<<5);//设置为开漏模式
    }
    else if(I2C0_SCL == PTC6)
    {
      PORT_PCR_REG(PORTC, 6) = PORT_PCR_MUX(7);
      PORT_PCR_REG(PORTC, 6) = PORT_PCR_REG(PORTC, 6)|(1<<5);//设置为开漏模式
    }
    else if(I2C0_SCL == PTC14)
    {
      PORT_PCR_REG(PORTC, 14) = PORT_PCR_MUX(3);
      PORT_PCR_REG(PORTC, 14) = PORT_PCR_REG(PORTC, 14)|(1<<5);//设置为开漏模式
    }
    else if(I2C0_SCL == PTD2)
    {
      PORT_PCR_REG(PORTD, 2) = PORT_PCR_MUX(7);
      PORT_PCR_REG(PORTD, 2) = PORT_PCR_REG(PORTD, 2)|(1<<5);//设置为开漏模式
    }
    
    
    if(I2C0_SDA == PTB1)
    {
      PORT_PCR_REG(PORTB, 1) = PORT_PCR_MUX(2);
      PORT_PCR_REG(PORTB, 1) = PORT_PCR_REG(PORTB, 1)|(1<<5);//设置为开漏模式
    }
    else if(I2C0_SDA == PTB3)
    {  
      PORT_PCR_REG(PORTB, 3) = PORT_PCR_MUX(2);
      PORT_PCR_REG(PORTB, 3) = PORT_PCR_REG(PORTB, 3)|(1<<5);//设置为开漏模式
    }
    else if(I2C0_SDA == PTD9)
    {
      PORT_PCR_REG(PORTD, 9) = PORT_PCR_MUX(2);
      PORT_PCR_REG(PORTD, 9) = PORT_PCR_REG(PORTD, 9)|(1<<5);//设置为开漏模式
    }  
    else if(I2C0_SDA == PTE18)
    {
      PORT_PCR_REG(PORTE, 18) = PORT_PCR_MUX(4);
      PORT_PCR_REG(PORTE, 18) = PORT_PCR_REG(PORTE, 18)|(1<<5);//设置为开漏模式
    } 
    else if(I2C0_SDA == PTE25)
    {
      PORT_PCR_REG(PORTE, 25) = PORT_PCR_MUX(5);
      PORT_PCR_REG(PORTE, 25) = PORT_PCR_REG(PORTE, 25)|(1<<5);//设置为开漏模式
    } 
    else if(I2C0_SDA == PTA11)
    {
      PORT_PCR_REG(PORTA, 11) = PORT_PCR_MUX(8);
      PORT_PCR_REG(PORTA, 11) = PORT_PCR_REG(PORTA, 11)|(1<<5);//设置为开漏模式
    }
    else if(I2C0_SDA == PTC7)
    {
      PORT_PCR_REG(PORTC, 7) = PORT_PCR_MUX(8);
      PORT_PCR_REG(PORTC, 7) = PORT_PCR_REG(PORTC, 7)|(1<<5);//设置为开漏模式
    }
    else if (I2C0_SDA == PTC15)
    {
      PORT_PCR_REG(PORTC, 15) = PORT_PCR_MUX(3);
      PORT_PCR_REG(PORTC, 15) = PORT_PCR_REG(PORTC, 15) | (1<<5);//设置为开漏模式
    }
    else if (I2C0_SDA == PTD3)
    {
      PORT_PCR_REG(PORTD, 3) = PORT_PCR_MUX(7);
      PORT_PCR_REG(PORTD, 3) = PORT_PCR_REG(PORTD, 3) | (1<<5);//设置为开漏模式
    }
  }
  else
  {
    /* 开启时钟 */
    SIM_SCGC4 |= SIM_SCGC4_I2C1_MASK;         //开启 I2C1时钟
    
    /* 配置 I2C1功能的 GPIO 接口 */
    if(I2C1_SCL == PTE1)
    { 
      PORT_PCR_REG(PORTE, 1) = PORT_PCR_MUX(6);
      PORT_PCR_REG(PORTE, 1) = PORT_PCR_REG(PORTE, 1)|(1<<5);//设置为开漏模式
    }
    else if(I2C1_SCL == PTC10)
    {
      PORT_PCR_REG(PORTC, 10) = PORT_PCR_MUX(2);
      PORT_PCR_REG(PORTC, 10) = PORT_PCR_REG(PORTC, 10)|(1<<5);//设置为开漏模式
    }
    else if(I2C1_SCL == PTC14)
    {
      PORT_PCR_REG(PORTC, 14) = PORT_PCR_MUX(2);
      PORT_PCR_REG(PORTC, 14) = PORT_PCR_REG(PORTC, 14)|(1<<5);//设置为开漏模式
    }
    else if(I2C1_SCL == PTA14)
    {
      PORT_PCR_REG(PORTA, 14) = PORT_PCR_MUX(8);
      PORT_PCR_REG(PORTA, 14) = PORT_PCR_REG(PORTA, 14)|(1<<5);//设置为开漏模式
    }
    
    if(I2C1_SDA == PTE0)
    { 
      PORT_PCR_REG(PORTE, 0) = PORT_PCR_MUX(6);
      PORT_PCR_REG(PORTE, 0) = PORT_PCR_REG(PORTE, 0) | (1<<5);//设置成开漏模式   只设置SDA即可，两个都设置也没事
    }
    else if (I2C1_SDA == PTC11)
    {
      PORT_PCR_REG(PORTC, 11) = PORT_PCR_MUX(2);
      PORT_PCR_REG(PORTC, 11) = PORT_PCR_REG(PORTC, 11) | (1<<5);//设置为开漏模式
    }
    else if (I2C1_SDA == PTC15)
    {
      PORT_PCR_REG(PORTC, 15) = PORT_PCR_MUX(2);
      PORT_PCR_REG(PORTC, 15) = PORT_PCR_REG(PORTC, 15) | (1<<5);//设置为开漏模式
    }
    else if (I2C1_SDA == PTA13)
    {
      PORT_PCR_REG(PORTA, 13) = PORT_PCR_MUX(8);
      PORT_PCR_REG(PORTA, 13) = PORT_PCR_REG(PORTA, 13) | (1<<5);//设置为开漏模式
    } 
  }
  
  /* 设置频率 */
  ((I2Cx[i2cn])->F)  = I2C_F_MULT(2) | I2C_F_ICR(0x17) ; 
  // MULT=00  即  mul = 1
  // ICR =14  ICR为Clock rate时钟频率
  // 从《k16 reference manual.pdf》P1460 可得：
  // ICR    SCL Divider   SDA Hold Value    SCL Hold (Start) Value    SCL Hold (Stop) Value
  //  29       384              33                   190                        193
  //  17       128              21                   58                         65
  // I2C baud rate = bus speed (Hz)/(mul × SCL divider)  即这里 135MHz/(4×160)=210.9375kHz
  
  /* 使能 IIC1 */
  ((I2Cx[i2cn])->C1) = I2C_C1_IICEN_MASK;
}

//-------------------------------------------------------------------------*
//函数名: I2C_StartTransmission                                                        
//功  能: 启动IIC传输                                                        
//参  数: i2cn    :端口名 I2C0,I2C1 
//        SlaveID :从机地址 
//        Mode    :传输模式
//返  回: 无                                                              
//简  例: IIC_init(I2C1);              
//-------------------------------------------------------------------------*
void I2C_StartTransmission (I2Cn i2cn, uint8_t SlaveID, MSmode Mode)
{
  SlaveID = ( SlaveID << 1 ) | Mode ;            //确定写地址和读地址
  
  /* send start signal */
  I2C_Start(i2cn);
  
  /* send ID with W/R bit */
  I2C_write_byte(i2cn, SlaveID);
}

//-------------------------------------------------------------------------*
//函数名: Pause                                                        
//功  能: 延时                                                        
//参  数: 无
//返  回: 无                                                              
//简  例: Pause;              
//-------------------------------------------------------------------------*
void Pause(void)
{
  uint16_t n;
  for(n = 1; n < 50000; n++)      //不可太小
  {
    asm("nop");
  }
}


//-------------------------------------------------------------------------*
//函数名: I2C_ReadAddr                                                        
//功  能: 读取IIC设备指定地址寄存器的数据                                                        
//参  数: i2cn    :端口名 I2C0,I2C1 
//        SlaveID :从机地址 
//        Addr    :从机的寄存器地址
//返  回: result                                                              
//简  例: p[0]  = I2C_ReadAddr(i2cn,SlaveID,OUT_X_MSB_REG);              
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
  I2C_DisableAck(i2cn); //不应答
  
  /* Dummy read 虚假读取*/
  result = ((I2Cx[i2cn])->D);
  I2C_Wait(i2cn);
  
  /* Send stop since about to read last byte */
  I2C_Stop(i2cn);
  
  /* Read byte */
  result = ((I2Cx[i2cn])->D);
  
  return result;
}


//-------------------------------------------------------------------------*
//函数名: I2C_ReadAddr                                                        
//功  能: 读取IIC设备指定地址寄存器的数据                                                        
//参  数: i2cn    :端口名 I2C0,I2C1 
//        SlaveID :从机地址 
//        Addr    :从机的寄存器地址
//        Data    :数据
//返  回: 无                                                              
//简  例: I2C_WriteAddr(I2C1, SlaveAddress2100, CTRL_REG1_2100, 0x02);             
//-------------------------------------------------------------------------*
void I2C_WriteAddr(I2Cn i2cn, uint8_t SlaveID, uint8_t Addr, uint8_t Data)
{
  /* send data to slave */
  I2C_StartTransmission(i2cn, SlaveID, write);    //启动传输
  I2C_Wait(i2cn);
  
  I2C_write_byte(i2cn, Addr);                    //写地址
  I2C_Wait(i2cn);
  
  I2C_write_byte(i2cn, Data);                    //写数据
  I2C_Wait(i2cn);
  
  I2C_Stop(i2cn);
  
  Pause();                                        //延时太短的话，可能写出错
}
void I2C_WriteAddr16(I2Cn i2cn, uint8_t SlaveID, uint8_t Addr, uint16_t Data)
{
  /* send data to slave */
  I2C_StartTransmission(i2cn, SlaveID, write);    //启动传输
  I2C_Wait(i2cn);
  
  I2C_write_byte(i2cn, Addr);                    //写地址
  I2C_Wait(i2cn);
  
  I2C_write_byte(i2cn, (uint8_t)(Data>>8));                    //写数据
  I2C_Wait(i2cn);
  I2C_write_byte(i2cn, (uint8_t)Data);                    //写数据
  I2C_Wait(i2cn);
  
  I2C_Stop(i2cn);
  
  Pause();                                        //延时太短的话，可能写出错
}

