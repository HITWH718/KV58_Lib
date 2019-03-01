#ifndef __DMA_H__
#define __DMA_H__


#include "gpio.h"

/* DMA - Register accessors */
#define DMA_CR_REG(base)                         ((base)->CR)
#define DMA_ES_REG(base)                         ((base)->ES)
#define DMA_ERQ_REG(base)                        ((base)->ERQ)
#define DMA_EEI_REG(base)                        ((base)->EEI)
#define DMA_CEEI_REG(base)                       ((base)->CEEI)
#define DMA_SEEI_REG(base)                       ((base)->SEEI)
#define DMA_CERQ_REG(base)                       ((base)->CERQ)
#define DMA_SERQ_REG(base)                       ((base)->SERQ)
#define DMA_CDNE_REG(base)                       ((base)->CDNE)
#define DMA_SSRT_REG(base)                       ((base)->SSRT)
#define DMA_CERR_REG(base)                       ((base)->CERR)
#define DMA_CINT_REG(base)                       ((base)->CINT)
#define DMA_INT_REG(base)                        ((base)->INT)
#define DMA_ERR_REG(base)                        ((base)->ERR)
#define DMA_HRS_REG(base)                        ((base)->HRS)
#define DMA_EARS_REG(base)                       ((base)->EARS)
#define DMA_DCHPRI3_REG(base)                    ((base)->DCHPRI3)
#define DMA_DCHPRI2_REG(base)                    ((base)->DCHPRI2)
#define DMA_DCHPRI1_REG(base)                    ((base)->DCHPRI1)
#define DMA_DCHPRI0_REG(base)                    ((base)->DCHPRI0)
#define DMA_DCHPRI7_REG(base)                    ((base)->DCHPRI7)
#define DMA_DCHPRI6_REG(base)                    ((base)->DCHPRI6)
#define DMA_DCHPRI5_REG(base)                    ((base)->DCHPRI5)
#define DMA_DCHPRI4_REG(base)                    ((base)->DCHPRI4)
#define DMA_DCHPRI11_REG(base)                   ((base)->DCHPRI11)
#define DMA_DCHPRI10_REG(base)                   ((base)->DCHPRI10)
#define DMA_DCHPRI9_REG(base)                    ((base)->DCHPRI9)
#define DMA_DCHPRI8_REG(base)                    ((base)->DCHPRI8)
#define DMA_DCHPRI15_REG(base)                   ((base)->DCHPRI15)
#define DMA_DCHPRI14_REG(base)                   ((base)->DCHPRI14)
#define DMA_DCHPRI13_REG(base)                   ((base)->DCHPRI13)
#define DMA_DCHPRI12_REG(base)                   ((base)->DCHPRI12)
#define DMA_DCHPRI19_REG(base)                   ((base)->DCHPRI19)
#define DMA_DCHPRI18_REG(base)                   ((base)->DCHPRI18)
#define DMA_DCHPRI17_REG(base)                   ((base)->DCHPRI17)
#define DMA_DCHPRI16_REG(base)                   ((base)->DCHPRI16)
#define DMA_DCHPRI23_REG(base)                   ((base)->DCHPRI23)
#define DMA_DCHPRI22_REG(base)                   ((base)->DCHPRI22)
#define DMA_DCHPRI21_REG(base)                   ((base)->DCHPRI21)
#define DMA_DCHPRI20_REG(base)                   ((base)->DCHPRI20)
#define DMA_DCHPRI27_REG(base)                   ((base)->DCHPRI27)
#define DMA_DCHPRI26_REG(base)                   ((base)->DCHPRI26)
#define DMA_DCHPRI25_REG(base)                   ((base)->DCHPRI25)
#define DMA_DCHPRI24_REG(base)                   ((base)->DCHPRI24)
#define DMA_DCHPRI31_REG(base)                   ((base)->DCHPRI31)
#define DMA_DCHPRI30_REG(base)                   ((base)->DCHPRI30)
#define DMA_DCHPRI29_REG(base)                   ((base)->DCHPRI29)
#define DMA_DCHPRI28_REG(base)                   ((base)->DCHPRI28)
#define DMA_SADDR_REG(base,index)                ((base)->TCD[index].SADDR)

#define DMA_SOFF_REG(base,index)                 ((base)->TCD[index].SOFF)

#define DMA_ATTR_REG(base,index)                 ((base)->TCD[index].ATTR)

#define DMA_NBYTES_MLNO_REG(base,index)          ((base)->TCD[index].NBYTES_MLNO)

#define DMA_NBYTES_MLOFFNO_REG(base,index)       ((base)->TCD[index].NBYTES_MLOFFNO)

#define DMA_NBYTES_MLOFFYES_REG(base,index)      ((base)->TCD[index].NBYTES_MLOFFYES)

#define DMA_SLAST_REG(base,index)                ((base)->TCD[index].SLAST)

#define DMA_DADDR_REG(base,index)                ((base)->TCD[index].DADDR)

#define DMA_DOFF_REG(base,index)                 ((base)->TCD[index].DOFF)

#define DMA_CITER_ELINKNO_REG(base,index)        ((base)->TCD[index].CITER_ELINKNO)

#define DMA_CITER_ELINKYES_REG(base,index)       ((base)->TCD[index].CITER_ELINKYES)

#define DMA_DLAST_SGA_REG(base,index)            ((base)->TCD[index].DLAST_SGA)

#define DMA_CSR_REG(base,index)                  ((base)->TCD[index].CSR)

#define DMA_BITER_ELINKNO_REG(base,index)        ((base)->TCD[index].BITER_ELINKNO)

#define DMA_BITER_ELINKYES_REG(base,index)       ((base)->TCD[index].BITER_ELINKYES)


#define DMAMUX_CHCFG_REG(base,index)             ((base)->CHCFG[index])

typedef enum      //DMA每次传输字节数
{
    DMA_BYTE1 = 0,
    DMA_BYTE2 = 1,
    DMA_BYTE4 = 2,
    DMA_BYTE16 = 4
} DMA_BYTEn;
typedef enum
{
  DMA_PERIPHERAL_TO_MEMORY = 0,
  DMA_MEMORY_TO_MEMORY,
  DMA_MEMORY_TO_PERIPHERAL
}DMA_TYPE;
typedef enum
{
    DADDR_RECOVER = 0,        //恢复目的地址
    DADDR_KEEPON  = 1,        //目的地址保持不变
} DMA_cfg;
typedef enum
{
    DMA_CH0,
    DMA_CH1,
    DMA_CH2,
    DMA_CH3,
    DMA_CH4,
    DMA_CH5,
    DMA_CH6,
    DMA_CH7,
    DMA_CH8,
    DMA_CH9,
    DMA_CH10,
    DMA_CH11,
    DMA_CH12,
    DMA_CH13,
    DMA_CH14,
    DMA_CH15,
      
    DMA_CH16,
    DMA_CH17,
    DMA_CH18,
    DMA_CH19,
    DMA_CH20,
    DMA_CH21,
    DMA_CH22,
    DMA_CH23,
    DMA_CH24,
    DMA_CH25,
    DMA_CH26,
    DMA_CH27,
    DMA_CH28,
    DMA_CH29,
    DMA_CH30,
    DMA_CH31
} DMA_CHn;
typedef enum
{
  /*DMAMUX Channels 0-15*/
  DMA_CHANNEL_DISABLED  = 0,
  DMA_UART0_RX          = 2,
  DMA_UART0_TX,
  DMA_UART1_RX,
  DMA_UART1_TX,
  DMA_PWM0_WR0,
  DMA_PWM0_WR1,
  DMA_PWM0_WR2,
  DMA_PWM0_WR3,
  DMA_PWM0_CP0,
  DMA_PWM0_CP1,
  DMA_PWM0_CP2,
  DMA_PWM0_CP3,
  DMA_CAN0,
  DMA_CAN1,
  DMA_SPI0_RX,
  DMA_SPI0_TX,
  DMA_XBA_OUT0,
  DMA_XBA_OUT1,
  DMA_XBA_OUT2,
  DMA_XBA_OUT3,
  DMA_I2C0,
  DMA_FTM0_CH0          = 24,
  DMA_FTM0_CH1,
  DMA_FTM0_CH2,
  DMA_FTM0_CH3,
  DMA_FTM0_CH4,
  DMA_FTM0_CH5,
  DMA_FTM0_CH6,
  DMA_FTM0_CH7,
  DMA_FTM1_CH0,
  DMA_FTM1_CH1,
  DMA_CMP3,
  DMA_FTM3_CH0          = 36,
  DMA_FTM3_CH1,
  DMA_FTM3_CH2,
  DMA_FTM3_CH3,
  DMA_HSADC0A_SC,
  DMA_HSADC0B_SC,
  DMA_CMP0,
  DMA_CMP1,
  DMA_CMP2,
  DMA_DAC0,
  DMA_PDB1              = 47,
  DMA_PDB0,
  DMA_PORTA,
  DMA_PORTB,
  DMA_PORTC,
  DMA_PORTD,
  DMA_PORTE,
  DMA_FTM3_CH4,
  DMA_FTM3_CH5,
  DMA_FTM3_CH6,
  DMA_FTM3_CH7,
  DMA_ALWAYS_ENABLED1   = 60,   //常用触发源，用在内存之间搬运
  DMA_ALWAYS_ENABLED2,
  DMA_ALWAYS_ENABLED3,
  DMA_ALWAYS_ENABLED4,
  
  /*DMAMUX Channels 16-31*/
  DMA_UART2_RX          = 2,
  DMA_UART2_TX,
  DMA_UART3_RX,
  DMA_UART3_TX,
  DMA_PWM1_WR0,
  DMA_PWM1_WR1,
  DMA_PWM1_WR2,
  DMA_PWM1_WR3,
  DMA_PWM1_CP0,
  DMA_PWM1_CP1,
  DMA_PWM1_CP2,
  DMA_PWM1_CP3,
  DMA_CAN2,
  DMA_SPI1_RX           = 16,
  DMA_SPI1_TX,
  DMA_I2C1              = 22,
  DMA_FTM2_CH0          = 32,
  DMA_FTM2_CH1,
  DMA_SPI2_RX,
  DMA_SPI2_TX,
  DMA_IEEE1588_TIMER0,
  DMA_IEEE1588_TIMER1,
  DMA_IEEE1588_TIMER2,
  DMA_IEEE1588_TIMER3,
  DMA_HSADC1A_SC,
  DMA_HSADC1B_SC,
  DMA_ADC0              = 45,
  DMA_UART4_Rx          = 54,
  DMA_UART4_Tx,
  DMA_UART5_Rx,
  DMA_UART5_Tx,
}DMA_REQUEST_SOURCE;

#define  DMA_IRQ_EN(DMA_CHn)    NVIC_EnableIRQ((IRQn_Type)((IRQn_Type)DMA_CHn + DMA0_DMA16_IRQn))               //允许DMA通道传输完成中断
#define  DMA_IRQ_DIS(DMA_CHn)   NVIC_DisableIRQ((IRQn_Type)((IRQn_Type)DMA_CHn + DMA0_DMA16_IRQn))              //禁止DMA通道传输完成中断
#define  DMA_IRQ_CLEAN(DMA_CHn) DMA_INT_REG(DMA0)|=(DMA_INT_INT0_MASK<<DMA_CHn)           //清除通道传输中断标志位

#define  DMA_EN(DMA_CHn)        DMA_ERQ_REG(DMA0) |= (DMA_ERQ_ERQ0_MASK<<(DMA_CHn))       //使能通道硬件DMA请求
#define  DMA_DIS(DMA_CHn)       DMA_ERQ_REG(DMA0) &=~(DMA_ERQ_ERQ0_MASK<<(DMA_CHn))       //禁止通道硬件DMA请求

extern void dma_portx2buff_init(DMA_CHn, void *SADDR, void *DADDR, PTXn_e, DMA_BYTEn, uint32_t count, uint32_t cfg, DMA_TYPE);
extern void DMATransDataStart(uint8_t CHn,uint32_t address,uint32_t Val);
#endif