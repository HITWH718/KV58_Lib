#ifndef _IIC_H_
#define _IIC_H_

#include "MKV58F24.h"
#include "port.h"

/**********************************  IIC(���Ÿ���) ***************************************/

#define I2C0_SCL    PTB0       // PTB0��PTB2��PTE19 PTE24 PTA12 PTC6 PTC14 PTD2
#define I2C0_SDA    PTB1       // PTB1��PTB3��PTE18 PTE25 PTA11 PTC7 PTC15 PTD3

#define I2C1_SCL    PTE1       // PTD8 PTE1��PTC10 PTA14 PTC14 
#define I2C1_SDA    PTE0       // PTD9 PTE0��PTC11 PTA13 PTC15

/**********************************  IIC(���Ÿ���) ***************************************/

//����ģ���
typedef enum I2Cn
{
    I2C_0  = 0,
    I2C_1  = 1
} I2Cn;

//�����дѡ��
typedef enum MSmode
{
    write =   0x00,  /* ����дģʽ  */
    read =   0x01   /* ������ģʽ  */
} MSmode;


void I2C_Init(I2Cn A);                                        //��ʼ��I2C
void I2C_WriteAddr(I2Cn, uint8_t SlaveID, uint8_t Addr, uint8_t Data);       //����ַ��д������
void I2C_WriteAddr16(I2Cn i2cn, uint8_t SlaveID, uint8_t Addr, uint16_t Data);
void I2C_StartTransmission (I2Cn, uint8_t SlaveID, MSmode);        //��������
uint8_t I2C_ReadAddr(I2Cn, uint8_t SlaveID, uint8_t Addr);                 //��ȡ��ַ�������


#endif