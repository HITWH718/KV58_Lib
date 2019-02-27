#include "MKV58F24.h"


typedef unsigned char	        uint8;    // 8 bits 
typedef unsigned short int	uint16;   // 16 bits 
typedef unsigned long int	uint32;   //32 bits 


//LPTMR脉冲计数输入管脚选项
typedef enum
{
    LPT0_ALT1 = 1,      // PTA19
    LPT0_ALT2 = 2       // PTC5
} LPT0_ALTn;
//LPTMR脉冲计数触发方式
typedef enum LPT_CFG
{
    LPT_Rising  = 0,    //上升沿触发
    LPT_Falling = 1     //下降沿触发
} LPT_CFG;

#define LPTMR_Flag_Clear()  (((LPTMR0)->CSR) |= LPTMR_CSR_TCF_MASK)



extern void     LPTMR_delay_ms(uint16 ms);      //延时(ms)
extern void     LPTMR_delay_us(uint16 us);      //延时(us)

extern void     LPTMR_timing_ms(uint16 ms);     //定时(ms)
extern void     LPTMR_timing_us(uint16 ms);     //定时(us)

extern void     LPTMR_time_start_ms(void);      //开始计时(ms)
extern uint32   LPTMR_time_get_ms(void);        //获取计时时间 

extern void     LPTMR_time_start_us(void);      //开始计时(ns)
extern uint32   LPTMR_time_get_us(void);        //获取计时时间 

extern void     LPTMR_time_close();             //关闭计时器

extern void     LPTMR_Pulse_Init  (LPT0_ALTn, uint16 count, LPT_CFG);   //计数器初始化设置
extern uint16   LPTMR_Pulse_Get   (void);                               //获取计数值
extern void     LPTMR_Pulse_Clean (void);                               //清空计数值