#ifndef _IIC_H_
#define _IIC_H_

#include "MKV58F24.h"
#include "port.h"

/**********************************  IIC(引脚复用) ***************************************/

#define I2C0_SCL    PTB0       // PTB0、PTB2、PTE19 PTE24 PTA12 PTC6 PTC14 PTD2
#define I2C0_SDA    PTB1       // PTB1、PTB3、PTE18 PTE25 PTA11 PTC7 PTC15 PTD3

#define I2C1_SCL    PTE1       // PTD8 PTE1、PTC10 PTA14 PTC14 
#define I2C1_SDA    PTE0       // PTD9 PTE0、PTC11 PTA13 PTC15

/**********************************  IIC(引脚复用) ***************************************/

//定义模块号
typedef enum I2Cn
{
    I2C_0  = 0,
    I2C_1  = 1
} I2Cn;

//定义读写选项
typedef enum MSmode
{
    write =   0x00,  /* 主机写模式  */
    read =   0x01   /* 主机读模式  */
} MSmode;


void I2C_Init(I2Cn A);                                        //初始化I2C
void I2C_WriteAddr(I2Cn, uint8_t SlaveID, uint8_t Addr, uint8_t Data);       //往地址里写入内容
void I2C_WriteAddr16(I2Cn i2cn, uint8_t SlaveID, uint8_t Addr, uint16_t Data);
void I2C_StartTransmission (I2Cn, uint8_t SlaveID, MSmode);        //启动传输
uint8_t I2C_ReadAddr(I2Cn, uint8_t SlaveID, uint8_t Addr);                 //读取地址里的内容


#endif