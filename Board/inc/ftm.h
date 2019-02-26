#ifndef _FTM_H_
#define _FTM_H_

#include "fsl_common.h"

#define FTM0_PRECISON 1000u
#define FTM1_PRECISON 1000u
#define FTM2_PRECISON 1000u
#define FTM3_PRECISON 1000u

typedef enum _FTMn
{
    ftm0,
    ftm1,
    ftm2,
    ftm3,
}FTMn_e;

typedef enum _FTM_CHn
{
    ftm_ch0,
    ftm_ch1,
    ftm_ch2,
    ftm_ch3,
    ftm_ch4,
    ftm_ch5,
    ftm_ch6,
    ftm_ch7,
}FTM_CHn_e;


extern void ftm_pwm_init(FTMn_e ftmn, FTM_CHn_e ch, uint32_t freq);
extern void ftm_pwm_duty(FTMn_e ftmn, FTM_CHn_e ch, uint16_t duty);
extern void ftm_quad_init(FTMn_e ftmn);
extern uint16_t ftm_quad_get(FTMn_e ftmn);
extern void ftm_quad_clean(FTMn_e ftmn);
#endif
