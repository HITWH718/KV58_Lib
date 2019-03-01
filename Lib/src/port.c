
#include "port.h"

static PORT_Type * const PORTx[] = PORT_BASE_PTRS;

void port_init(PTXn_e ptxn, uint32_t mux)
{
    //打开端口时钟
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK<<PTX(ptxn);
    //清标志位
    
    //配置复用功能
    PORTx[PTX(ptxn)]->PCR[PTn(ptxn)] &= ~PORT_PCR_MUX_MASK;
    PORTx[PTX(ptxn)]->PCR[PTn(ptxn)] |= PORT_PCR_MUX(mux);
}
