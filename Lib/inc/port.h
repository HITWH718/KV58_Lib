#ifndef _PORT_H_
#define _PORT_H_

#include "MKV58F24.h"

static PORT_Type *const PORTx[] = PORT_BASE_PTRS;

//PORT寄存器
#define PORT_PCR_REG(base,index)                 ((base)->PCR[index])
#define PORT_GPCLR_REG(base)                     ((base)->GPCLR)
#define PORT_GPCHR_REG(base)                     ((base)->GPCHR)
#define PORT_ISFR_REG(base)                      ((base)->ISFR)
#define PORT_DFER_REG(base)                      ((base)->DFER)
#define PORT_DFCR_REG(base)                      ((base)->DFCR)
#define PORT_DFWR_REG(base)                      ((base)->DFWR)

typedef enum _port_mux
{

  Disabled = PORT_PCR_MUX(0x00),         /*!< Corresponding pin is disabled, but is used as an analog pin. */
  AsGpio = PORT_PCR_MUX(0x01),           /*!< Corresponding pin is configured as GPIO. */
  Alt2 = PORT_PCR_MUX(0x02),             /*!< Chip-specific */
  Alt3 = PORT_PCR_MUX(0x03),             /*!< Chip-specific */
  Alt4 = PORT_PCR_MUX(0x04),             /*!< Chip-specific */
  Alt5 = PORT_PCR_MUX(0x05),             /*!< Chip-specific */
  Alt6 = PORT_PCR_MUX(0x06),             /*!< Chip-specific */
  Alt7 = PORT_PCR_MUX(0x07),             /*!< Chip-specific */
  Alt8 = PORT_PCR_MUX(0x08),             /*!< Chip-specific */
  Alt9 = PORT_PCR_MUX(0x09),             /*!< Chip-specific */
  Alt10 = PORT_PCR_MUX(0x0A),           /*!< Chip-specific */
  Alt11 = PORT_PCR_MUX(0x0B),           /*!< Chip-specific */
  Alt12 = PORT_PCR_MUX(0x0C),           /*!< Chip-specific */
  Alt13 = PORT_PCR_MUX(0x0D),           /*!< Chip-specific */
  Alt14 = PORT_PCR_MUX(0x0E),           /*!< Chip-specific */
  Alt15 = PORT_PCR_MUX(0x0F),           /*!< Chip-specific */

  IRQ_ZERO     = PORT_PCR_IRQC(0x08),     //低电平触发
  IRQ_RISING   = PORT_PCR_IRQC(0x09),    //上升沿触发
  IRQ_FALLING  = PORT_PCR_IRQC(0x0A),   //下降沿触发
  IRQ_EITHER   = PORT_PCR_IRQC(0x0B),  //跳变沿触发
  IRQ_ONE      = PORT_PCR_IRQC(0x0C),  //高电平触发

  //DMA请求
  DMA_RISING   = PORT_PCR_IRQC(0x01),   //上升沿触发
  DMA_FALLING  = PORT_PCR_IRQC(0x02),   //下降沿触发
  DMA_EITHER   = PORT_PCR_IRQC(0x03),   //跳变沿触发

  HDS          = PORT_PCR_DSE(0x01),    //输出高驱动能力
  OD           = PORT_PCR_ODE(0x01),    //漏极输出
  PF           = PORT_PCR_PFE(0x01),    //带无源滤波器
  SSR          = PORT_PCR_SRE(0x01),     //输出慢变化率

  //下拉上拉选择
  PULLDOWN     = 0x02 << PORT_PCR_PS_SHIFT,     //下拉
  PULLUP       = 0x03 << PORT_PCR_PS_SHIFT,     //上拉
} port_mux_t;

typedef enum _PTXn
{
    PTA0,  PTA1,  PTA2,  PTA3,  PTA4,  PTA5,  PTA6,  PTA7,  PTA8,  PTA9,  PTA10, PTA11, PTA12, PTA13, PTA14, PTA15,
    PTA16, PTA17, PTA18, PTA19, PTA20, PTA21, PTA22, PTA23, PTA24, PTA25, PTA26, PTA27, PTA28, PTA29, PTA30, PTA31,

    PTB0,  PTB1,  PTB2,  PTB3,  PTB4,  PTB5,  PTB6,  PTB7,  PTB8,  PTB9,  PTB10, PTB11, PTB12, PTB13, PTB14, PTB15,
    PTB16, PTB17, PTB18, PTB19, PTB20, PTB21, PTB22, PTB23, PTB24, PTB25, PTB26, PTB27, PTB28, PTB29, PTB30, PTB31,

    PTC0,  PTC1,  PTC2,  PTC3,  PTC4,  PTC5,  PTC6,  PTC7,  PTC8,  PTC9,  PTC10, PTC11, PTC12, PTC13, PTC14, PTC15,
    PTC16, PTC17, PTC18, PTC19, PTC20, PTC21, PTC22, PTC23, PTC24, PTC25, PTC26, PTC27, PTC28, PTC29, PTC30, PTC31,

    PTD0,  PTD1,  PTD2,  PTD3,  PTD4,  PTD5,  PTD6,  PTD7,  PTD8,  PTD9,  PTD10, PTD11, PTD12, PTD13, PTD14, PTD15,
    PTD16, PTD17, PTD18, PTD19, PTD20, PTD21, PTD22, PTD23, PTD24, PTD25, PTD26, PTD27, PTD28, PTD29, PTD30, PTD31,

    PTE0,  PTE1,  PTE2,  PTE3,  PTE4,  PTE5,  PTE6,  PTE7,  PTE8,  PTE9,  PTE10, PTE11, PTE12, PTE13, PTE14, PTE15,
    PTE16, PTE17, PTE18, PTE19, PTE20, PTE21, PTE22, PTE23, PTE24, PTE25, PTE26, PTE27, PTE28, PTE29, PTE30, PTE31,
} PTXn_e;

typedef enum _PTX
{
    PTA, PTB, PTC, PTD, PTE,

    PTX_MAX,
} PTX_e;

typedef enum _PTn
{
    PT0 , PT1 , PT2 , PT3 , PT4 , PT5 , PT6 , PT7 ,
    PT8 , PT9 , PT10, PT11, PT12, PT13, PT14, PT15,
    PT16, PT17, PT18, PT19, PT20, PT21, PT22, PT23,
    PT24, PT25, PT26, PT27, PT28, PT29, PT30, PT31,
} PTn_e;


//PTx = PTxn / 32 ; PTn = PTxn & 31
#define PTX(PTxn)           ((PTxn)>>5)
#define PTn(PTxn)           ((PTxn)&0x1f)
#define PORTX_BASE(PTxn)     PORTx[PTX(PTxn)]       //PORT模块的地址

extern void port_init(PTXn_e ptxn, uint32_t cfg);
extern void port_init_NoALT(PTXn_e ptxn, uint32_t cfg);

#endif
