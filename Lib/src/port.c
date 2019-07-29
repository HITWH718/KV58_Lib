#include "port.h"

void port_init(PTXn_e ptxn, uint32_t cfg)
{
    //打开端口时钟
    SIM->SCGC5 |= (SIM_SCGC5_PORTA_MASK << PTX(ptxn));
    //清标志位

    //配置复用功能
    PORTx[PTX(ptxn)]->ISFR = (1 << PTn(ptxn)); // 清空中断状态标志位

    //PORTx[PTX(ptxn)]->PCR[PTn(ptxn)] &= ~PORT_PCR_MUX_MASK;
    PORTx[PTX(ptxn)]->PCR[PTn(ptxn)] = cfg;
}

void port_init_NoALT(PTXn_e ptxn, uint32_t cfg)
{
    SIM->SCGC5 |= (SIM_SCGC5_PORTA_MASK << PTX(ptxn));
    PORTx[PTX(ptxn)]->ISFR = (1 << PTn(ptxn)); // 清空中断状态标志位

    cfg &= ~PORT_PCR_MUX_MASK;                                     //清了MUX 字段（即不需要配置ALT，保持原来的ALT）
    cfg |= (PORTx[PTX(ptxn)]->PCR[PTn(ptxn)] & PORT_PCR_MUX_MASK); //读取寄存器里配置的 MUX

    PORTx[PTX(ptxn)]->PCR[PTn(ptxn)] = cfg;
}
