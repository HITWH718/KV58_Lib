#ifndef _GPIO_H_
#define _GPIO_H_

#include "fsl_common.h"
#include "port.h"


/**
 *  @brief 变量的位清0和置1
 */
#define BIT_CLEAN(var,n)        (var) &= ~(1<<(n))   //变量var 的n位（即第n+1位）清0
#define BIT_SET(var,n)          (var) |=  (1<<(n))   //变量var 的n位（即第n+1位）置1
#define BIT_GET(var,n)          (((var)>>(n))&0x01)  //读取变量var 的n位（即第n+1位）


typedef enum //枚举端口状态
{
    GPI = 0, //定义管脚输入方向
    GPO = 1, //定义管脚输出方向
}GPIO_CFG;

extern void gpio_init(PTXn_e ptxn, GPIO_CFG cfg, uint8_t data);
extern void gpio_ddr (PTXn_e ptxn, GPIO_CFG cfg);
extern void gpio_set (PTXn_e ptxn, uint8_t data);
extern void gpio_turn (PTXn_e ptxn);
extern uint8_t gpio_get(PTXn_e ptxn);

#endif
