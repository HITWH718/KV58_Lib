#ifndef _DAC_H_
#define _DAC_H_

#include "MKV58F24.h"

extern void dac_init();               //DAC一次转换初始化
extern void dac_out(uint16_t val);    //DAC一次转换操作

#endif
