#ifndef _CLOCK_H_
#define _CLOCK_H_

#include "MKV58F24.h"


extern uint32_t mcgout_clk_khz;
extern uint32_t system_clk_khz;         //core, RAM, DMA, GPIO, FMC
extern uint32_t fastperiphral_clk_khz;  //UARTs, SPI, eFlexPWM, FTMs, PDBs, ENC, FlexCAN, XBARA, ADC
extern uint32_t flexbus_clk_khz;        //Flexbus
extern uint32_t bus_clk_khz;            //Flash, I2C, WDOG, EWM, PIT, LPTIMER, OSC, MCG, PMC, XBARB/AOI, CMP

extern void pll_init();
extern void osc_init();
extern void mcg_init();

#endif

