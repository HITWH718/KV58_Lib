#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include "MKV58F24.h"
#include <stdint.h>

typedef struct
{
  uint32_t CSR;
  uint32_t RVR;
  uint32_t CVR;
  uint32_t CALIB;
}Systick_type;

#define SysTick_BASE_PTR                         ((Systick_type*)0xE000E010u)
#define SysTick_RVR_RELOAD_MASK                  0xFFFFFFu
#define SysTick_CSR_ENABLE_MASK                  0x1u
#define SysTick_CSR_TICKINT_MASK                 0x2u
#define SysTick_CSR_CLKSOURCE_MASK               0x4u
#define SysTick_CSR_COUNTFLAG_MASK               0x10000u


extern void Systick_delay_ms(uint32_t ms);
#endif