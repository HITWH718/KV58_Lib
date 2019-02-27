#include "MKV58F24.h"


typedef unsigned char	        uint8;    // 8 bits 
typedef unsigned short int	uint16;   // 16 bits 
typedef unsigned long int	uint32;   //32 bits 


//LPTMR�����������ܽ�ѡ��
typedef enum
{
    LPT0_ALT1 = 1,      // PTA19
    LPT0_ALT2 = 2       // PTC5
} LPT0_ALTn;
//LPTMR�������������ʽ
typedef enum LPT_CFG
{
    LPT_Rising  = 0,    //�����ش���
    LPT_Falling = 1     //�½��ش���
} LPT_CFG;

#define LPTMR_Flag_Clear()  (((LPTMR0)->CSR) |= LPTMR_CSR_TCF_MASK)



extern void     LPTMR_delay_ms(uint16 ms);      //��ʱ(ms)
extern void     LPTMR_delay_us(uint16 us);      //��ʱ(us)

extern void     LPTMR_timing_ms(uint16 ms);     //��ʱ(ms)
extern void     LPTMR_timing_us(uint16 ms);     //��ʱ(us)

extern void     LPTMR_time_start_ms(void);      //��ʼ��ʱ(ms)
extern uint32   LPTMR_time_get_ms(void);        //��ȡ��ʱʱ�� 

extern void     LPTMR_time_start_us(void);      //��ʼ��ʱ(ns)
extern uint32   LPTMR_time_get_us(void);        //��ȡ��ʱʱ�� 

extern void     LPTMR_time_close();             //�رռ�ʱ��

extern void     LPTMR_Pulse_Init  (LPT0_ALTn, uint16 count, LPT_CFG);   //��������ʼ������
extern uint16   LPTMR_Pulse_Get   (void);                               //��ȡ����ֵ
extern void     LPTMR_Pulse_Clean (void);                               //��ռ���ֵ