#ifndef _IIC_SOFTWARE_H_
#define _IIC_SOFTWARE_H_

#include "gpio.h"

//IIC 引脚宏定义
#define IIC_SCL        PTC16
#define IIC_SDA        PTC17

//IIC 引脚控制宏定义
#define SDA()          gpio_get(IIC_SDA)

#define SCL_SET(x)     gpio_set(IIC_SCL,x)  //IO口输出低电平或低电平
#define SDA_SET(x)     gpio_set(IIC_SDA,x)  //IO口输出低电平或低电平

#define DIR_OUT()      gpio_ddr(IIC_SDA, GPO)    //输出方向
#define DIR_IN()       gpio_ddr(IIC_SDA, GPI)    //输入方向

//IIC 应答
#define ACK          1  //主应答
#define NO_ACK       0  //从应答

extern void IIC_write_reg(uint8_t dev_add, uint8_t reg, uint8_t data);
extern uint8_t IIC_read_reg(uint8_t dev_add, uint8_t reg);

#endif


