#ifndef _IIC_SOFTWARE_H_
#define _IIC_SOFTWARE_H_

#include "gpio.h"

//IIC ���ź궨��
#define IIC_SCL        PTC16
#define IIC_SDA        PTC17

//IIC ���ſ��ƺ궨��
#define SDA()          gpio_get(IIC_SDA)

#define SCL_SET(x)     gpio_set(IIC_SCL,x)  //IO������͵�ƽ��͵�ƽ
#define SDA_SET(x)     gpio_set(IIC_SDA,x)  //IO������͵�ƽ��͵�ƽ

#define DIR_OUT()      gpio_ddr(IIC_SDA, GPO)    //�������
#define DIR_IN()       gpio_ddr(IIC_SDA, GPI)    //���뷽��

//IIC Ӧ��
#define ACK          1  //��Ӧ��
#define NO_ACK       0  //��Ӧ��

extern void IIC_write_reg(uint8_t dev_add, uint8_t reg, uint8_t data);
extern uint8_t IIC_read_reg(uint8_t dev_add, uint8_t reg);

#endif


