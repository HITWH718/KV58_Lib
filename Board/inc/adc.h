#ifndef _ADC_H_
#define _ADC_H_

#include "headfile.h"
#include "MKV58F24.h"

typedef enum
{
    //ADC 的A通道既可以软件触发也可以硬件触发
    //ADC 的B通道只能硬件触发
    //参考手册p910 40.1.3.1 ADC0 Channel Assignment for 144-Pin Package
    // -------------------------------ADC0 20181218确认-------------------------
    ADC0_DP0=0,    //ADC0_SE0
    ADC0_DP1=1,    //PTE16
    ADC0_DP2=2,    //PTE4
    ADC0_DP3=3,    //PTE11
    //ADC0_CFG2 &=~ADC_CFG2_MUXSEL_MASK   0选择a  0 ADxxa channels are selected.
    ADC0_SE4a=4,   //PTE6
    ADC0_SE5a=5,   //PTE18
    ADC0_SE6a=6,   //PTE19
    ADC0_SE7a=7,   //HSADC0A_CH6
    //ADC0_CFG2 |=ADC_CFG2_MUXSEL_MASK    1选择b  1 ADxxb channels are selected.
    ADC0_SE4b=12,  //HSADC0A_CH7
    ADC0_SE5b=13,  //PTE20
    ADC0_SE6b=14,  //PTB4
    ADC0_SE7b=15,  //PTB5
    ADC0_SE8=8,    //ADC0_SE8
    ADC0_SE9=9,    //PTE17
    ADC0_SE10=10,  //PTE5
    ADC0_SE11=11,  //PTE12
}ADCn_Ch_e;

//精度位数
typedef enum ADC_nbit
{
    ADC_8bit   = 0x00,
    ADC_10bit  = 0x02,
    ADC_12bit  = 0x01,
    ADC_16bit  = 0x03
} ADC_nbit;


//外部函数接口声明
extern void adc_init (ADCn_Ch_e);                //ADC初始化
extern uint16_t adc_once (ADCn_Ch_e, ADC_nbit);      //采集一次一路模拟量的AD值
extern uint16_t adc_average (ADCn_Ch_e adcn_ch, ADC_nbit bit, uint8_t times);
extern void adc_stop();                   //停止ADC转换

#endif
