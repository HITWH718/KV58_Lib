#ifndef _PORT_H_
#define _PORT_H_

#include "fsl_common.h"

typedef enum _port_mux
{
    Disabled = 0U,         /*!< Corresponding pin is disabled, but is used as an analog pin. */
    AsGpio = 1U,           /*!< Corresponding pin is configured as GPIO. */
    Alt2 = 2U,             /*!< Chip-specific */
    Alt3 = 3U,             /*!< Chip-specific */
    Alt4 = 4U,             /*!< Chip-specific */
    Alt5 = 5U,             /*!< Chip-specific */
    Alt6 = 6U,             /*!< Chip-specific */
    Alt7 = 7U,             /*!< Chip-specific */
    Alt8 = 8U,             /*!< Chip-specific */
    Alt9 = 9U,             /*!< Chip-specific */
    Alt10 = 10U,           /*!< Chip-specific */
    Alt11 = 11U,           /*!< Chip-specific */
    Alt12 = 12U,           /*!< Chip-specific */
    Alt13 = 13U,           /*!< Chip-specific */
    Alt14 = 14U,           /*!< Chip-specific */
    Alt15 = 15U,           /*!< Chip-specific */
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

extern void port_init(PTXn_e ptxn, port_mux_t mux);
extern void port_cfg();

#endif
