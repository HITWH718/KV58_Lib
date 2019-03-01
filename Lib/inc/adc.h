#ifndef _ADC_H_
#define _ADC_H_

#include "headfile.h"
#include "MKV58F24.h"

typedef enum
{
    //ADC ��Aͨ���ȿ����������Ҳ����Ӳ������
    //ADC ��Bͨ��ֻ��Ӳ������
    //�ο��ֲ�p910 40.1.3.1 ADC0 Channel Assignment for 144-Pin Package
    // -------------------------------ADC0 20181218ȷ��-------------------------
    ADC0_DP0=0,    //ADC0_SE0
    ADC0_DP1=1,    //PTE16
    ADC0_DP2=2,    //PTE4
    ADC0_DP3=3,    //PTE11
    //ADC0_CFG2 &=~ADC_CFG2_MUXSEL_MASK   0ѡ��a  0 ADxxa channels are selected.
    ADC0_SE4a=4,   //PTE6
    ADC0_SE5a=5,   //PTE18
    ADC0_SE6a=6,   //PTE19
    ADC0_SE7a=7,   //HSADC0A_CH6
    //ADC0_CFG2 |=ADC_CFG2_MUXSEL_MASK    1ѡ��b  1 ADxxb channels are selected.
    ADC0_SE4b=12,  //HSADC0A_CH7
    ADC0_SE5b=13,  //PTE20
    ADC0_SE6b=14,  //PTB4
    ADC0_SE7b=15,  //PTB5
    ADC0_SE8=8,    //ADC0_SE8
    ADC0_SE9=9,    //PTE17
    ADC0_SE10=10,  //PTE5
    ADC0_SE11=11,  //PTE12
}ADCn_Ch_e;

//����λ��
typedef enum ADC_nbit
{
    ADC_8bit   = 0x00,
    ADC_10bit  = 0x02,
    ADC_12bit  = 0x01,
    ADC_16bit  = 0x03
} ADC_nbit;


//�ⲿ�����ӿ�����
extern void adc_init (ADCn_Ch_e);                //ADC��ʼ��
extern uint16_t adc_once (ADCn_Ch_e, ADC_nbit);      //�ɼ�һ��һ·ģ������ADֵ
extern uint16_t adc_average (ADCn_Ch_e adcn_ch, ADC_nbit bit, uint8_t times);
extern void adc_stop();                   //ֹͣADCת��

#endif
