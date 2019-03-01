#ifndef _PIT_H_
#define _PIT_H_

#include "MKV58F24.h"

#define pit_flag_clr(pitn)  PIT->CHANNEL[pitn].TFLG = PIT_TFLG_TIF_MASK

#define pit_delay_ms(pitn,ms)    pit_delay(pitn,ms)
#define pit_delay_us(pitn,us)    pit_delay(pitn,us/1000)

#define pit_init_ms(pitn,ms)    pit_init(pitn,ms)
#define pit_init_us(pitn,us)    pit_init(pitn,us/1000)


typedef enum _PITn
{
    pit0,
    pit1,
    pit2,
    pit3,
}PITn_e;

extern void pit_delay(PITn_e pitn,uint32_t count);
extern void pit_init(PITn_e pitn,uint32_t count);


#endif
