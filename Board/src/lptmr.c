/*!
 * @file       lptmr.c
 * @brief      lptmr驱动函数，包括延时，定时，脉冲计数
 * @author     Sylvia_Li
 * @version    v1.0
 * @date       2019-01-22
 */
#include "lptmr.h"
#include "MKV58F24.h"
//-------------------------------------------------------------------------*
//函数名: LPTMR_pulse_init                                                        
//功  能: LPTMR脉冲计数初始化                                                        
//参  数: LPT0_ALTn:LPTMR脉冲计数管脚
//        count   :LPTMR脉冲比较值
//        LPT_CFG :LPTMR脉冲计数方式：上升沿计数或下降沿计数
//返  回: 无                                                              
//简  例:  LPTMR_pulse_init(LPT0_ALT1,32768,LPT_Rising);                                
//-------------------------------------------------------------------------*
void LPTMR_Pulse_Init(LPT0_ALTn altn, uint16 count, LPT_CFG cfg)
{    

  
    ((SIM)->SCGC5) |= SIM_SCGC5_LPTMR_MASK;   //使能LPT模块时钟
    
    if(altn == LPT0_ALT1)  //设置输入管脚
    {
           ((PORTA)->PCR[19])= PORT_PCR_MUX(0x6); //管脚PORTA19
    }
    else if(altn == LPT0_ALT2)
    {
           ((PORTC)->PCR[5])= PORT_PCR_MUX(0x4);  //管脚PORTC5
    }
    else                                    //不可能发生事件
    {
       ;  
    }

     // 清状态寄存器
     ((LPTMR0)->CSR)= 0x00;    //需要先关闭LPT配置时钟分频 设置时钟分频等

     
      //选择时钟源
      ((LPTMR0)->PSR)  =   ( 0  | LPTMR_PSR_PCS(1)                  //选择时钟源： 0 为 MCGIRCLK ，1为 LPO（1KHz） ，2为 ERCLK32K ，3为 OSCERCLK
                                | LPTMR_PSR_PBYP_MASK               //旁路 预分频/干扰滤波器 ,即不用 预分频/干扰滤波器(注释了表示使用预分频/干扰滤波器)
                      //| LPTMR_PSR_PRESCALE(1)           //预分频值 = 2^(n+1) ,n = 0~ 0xF
                            );


     // 设置累加计数值
     ((LPTMR0)->CMR) =   LPTMR_CMR_COMPARE(count);                   //设置比较值

     // 管脚设置、使能中断
     ((LPTMR0)->CSR) =  (0
                    | LPTMR_CSR_TPS(altn)       // 选择输入管脚 选择
                    | LPTMR_CSR_TMS_MASK        // 选择脉冲计数 (注释了表示时间计数模式)
                    | ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //脉冲计数器触发方式选择：0为高电平有效，上升沿加1
                    | LPTMR_CSR_TEN_MASK        //使能LPT(注释了表示禁用)
                    | LPTMR_CSR_TIE_MASK        //中断使能
                    //| LPTMR_CSR_TFC_MASK      //0:计数值等于比较值就复位；1：溢出复位（注释表示0）
                   );
}

//-------------------------------------------------------------------------*
//函数名: LPTMR_pulse_get                                                        
//功  能: 获取LPTMR脉冲计数值                                                        
//返  回: 脉冲计数值                                                              
//简  例: 无                                
//-------------------------------------------------------------------------*
uint16 LPTMR_Pulse_Get(void)
{
    uint16 data;
    if(((LPTMR0)->CSR) & LPTMR_CSR_TCF_MASK)     //已经溢出了
    {
        data = ~0;                              //返回 0xffffffff 表示错误
    }
    else
    {
        data = ((LPTMR0)->CNR);
    }
    return data;
}


//-------------------------------------------------------------------------*
//函数名: LPTMR_pulse_clean                                                        
//功  能: 清空LPTMR脉冲计数                                                        
//参  数: 无
//返  回: 无                                                              
//简  例: 无                                
//-------------------------------------------------------------------------*
void LPTMR_Pulse_Clean(void)
{
    ((LPTMR0)->CSR)  &= ~LPTMR_CSR_TEN_MASK;     //禁用LPT的时候就会自动清计数器的值
    ((LPTMR0)->CSR)  |=  LPTMR_CSR_TEN_MASK;     //相当于先关闭再开启
}

//-------------------------------------------------------------------------*
//函数名: LPTMR_delay_ms                                                        
//功  能: LPTMR延时函数（ms）                                                        
//参  数: ms    毫秒
//返  回: 无                                                              
//简  例: LPTMR_delay_ms(32);     // LPTMR 延时32ms                                
//-------------------------------------------------------------------------*
void LPTMR_delay_ms(uint16 ms)
{
    if(ms == 0)
    {
        return;
    }

    ((SIM)->SCGC5) |= SIM_SCGC5_LPTMR_MASK;    //使能LPT模块时钟

    ((LPTMR0)->CSR) = 0x00;                      //先关了LPT，自动清计数器的值

    ((LPTMR0)->CMR) = ms;                        //设置比较值，即延时时间

    //选择时钟源
    ((LPTMR0)->PSR) =   ( 0
                      | LPTMR_PSR_PCS(1)                  //选择时钟源： 0 为 MCGIRCLK ，1为 LPO（1KHz） ，2为 ERCLK32K ，3为 OSCERCLK
                      | LPTMR_PSR_PBYP_MASK               //旁路 预分频/干扰滤波器 ,即不用 预分频/干扰滤波器(注释了表示使用预分频/干扰滤波器)
                      //| LPTMR_PSR_PRESCALE(1)           //预分频值 = 2^(n+1) ,n = 0~ 0xF
                    );

    //使能 LPT
    ((LPTMR0)->CSR)  =  (0
                    //| LPTMR_CSR_TPS(1)        // 选择输入管脚 选择
                    //| LPTMR_CSR_TMS_MASK      // 选择脉冲计数 (注释了表示时间计数模式)
                    //| ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //脉冲计数器触发方式选择：0为高电平有效，上升沿加1
                     | LPTMR_CSR_TEN_MASK        //使能LPT(注释了表示禁用)
                    //| LPTMR_CSR_TIE_MASK      //中断使能
                    //| LPTMR_CSR_TFC_MASK      //0:计数值等于比较值就复位；1：溢出复位（注释表示0）
                    );

    while (!(((LPTMR0)->CMR) & LPTMR_CSR_TCF_MASK)); //等待比较值与计数值相等，即时间到了

    ((LPTMR0)->CSR) &= ~LPTMR_CSR_TEN_MASK;          //清除比较标志位

    return;
}

//-------------------------------------------------------------------------*
//函数名: LPTMR_timing_ms                                                        
//功  能: LPTMR定时函数（ms）                                                        
//参  数: ms          LPTMR定时时间(0~65535)
//返  回: 无                                                              
//简  例:  LPTMR_timing_ms(32);                                // LPTMR 定时 32ms
//         set_vector_handler(LPTimer_VECTORn,lptmr_hander);   // 设置中断复位函数到中断向量表里
//         enable_irq(LPTimer_IRQn);                           // 使能LPTMR中断                            
//-------------------------------------------------------------------------*
void LPTMR_timing_ms(uint16 ms)
{
    if(ms == 0)
    {
        return;
    }

    ((SIM)->SCGC5) |= SIM_SCGC5_LPTMR_MASK;    //使能LPT模块时钟

    ((LPTMR0)->CSR)= 0x00;                      //先关了LPT，自动清计数器的值

    ((LPTMR0)->CMR) = ms;                        //设置比较值，即延时时间

    //选择时钟源
    ((LPTMR0)->PSR)  =   ( 0
                      | LPTMR_PSR_PCS(1)                  //选择时钟源： 0 为 MCGIRCLK ，1为 LPO（1KHz） ，2为 ERCLK32K ，3为 OSCERCLK
                      | LPTMR_PSR_PBYP_MASK               //旁路 预分频/干扰滤波器 ,即不用 预分频/干扰滤波器(注释了表示使用预分频/干扰滤波器)
                      //| LPTMR_PSR_PRESCALE(1)           //预分频值 = 2^(n+1) ,n = 0~ 0xF
                    );

    //使能 LPT
    ((LPTMR0)->CSR)  =  (0
                    //| LPTMR_CSR_TPS(1)        // 选择输入管脚 选择
                    //| LPTMR_CSR_TMS_MASK      // 选择脉冲计数 (注释了表示时间计数模式)
                    //| ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //脉冲计数器触发方式选择：0为高电平有效，上升沿加1
                    | LPTMR_CSR_TEN_MASK        //使能LPT(注释了表示禁用)
                    | LPTMR_CSR_TIE_MASK        //中断使能
                    //| LPTMR_CSR_TFC_MASK      //0:计数值等于比较值就复位；1：溢出复位（注释表示0）
                   );

    return;                    
}


//-------------------------------------------------------------------------*
//函数名: LPTMR_delay_us                                                        
//功  能: LPTMR延时函数（us）                                                        
//参  数: us          LPTMR延时时间(0~41942)
//返  回: 无                                                              
//简  例:  LPTMR_delay_us(32);     // LPTMR 延时32us                              
//-------------------------------------------------------------------------*
void LPTMR_delay_us(uint16 us)
{

    if(us == 0)
    {
        return;
    }

    ((OSC0)->CR) |= OSC_CR_ERCLKEN_MASK;                              //使能 OSCERCLK


    ((SIM)->SCGC5) |= SIM_SCGC5_LPTMR_MASK;                        //使能LPT模块时钟

    ((LPTMR0)->CSR)  = 0x00;                                          //先关了LPT，自动清计数器的值

    ((LPTMR0)->CMR) = (us * 50 + 16) / 32;                 //设置比较值，即延时时间

    //选择时钟源
   ((LPTMR0)->PSR) =   ( 0
                      | LPTMR_PSR_PCS(3)          //选择时钟源： 0 为 MCGIRCLK ，1为 LPO（1KHz） ，2为 ERCLK32K ，3为 OSCERCLK
                      //| LPTMR_PSR_PBYP_MASK     //旁路 预分频/干扰滤波器 ,即不用 预分频/干扰滤波器(注释了表示使用预分频/干扰滤波器)
                      | LPTMR_PSR_PRESCALE(4)     //预分频值 = 2^(n+1) ,n = 0~ 0xF
                    );

    //使能 LPT
    ((LPTMR0)->CSR) =  (0
                    //| LPTMR_CSR_TPS(1)        // 选择输入管脚 选择
                    //| LPTMR_CSR_TMS_MASK      // 选择脉冲计数 (注释了表示时间计数模式)
                    //| ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //脉冲计数器触发方式选择：0为高电平有效，上升沿加1
                    | LPTMR_CSR_TEN_MASK        //使能LPT(注释了表示禁用)
                    //| LPTMR_CSR_TIE_MASK      //中断使能
                    //| LPTMR_CSR_TFC_MASK      //0:计数值等于比较值就复位；1：溢出复位（注释表示0）
                   );

    while (!(((LPTMR0)->CSR) & LPTMR_CSR_TCF_MASK)); //等待比较值与计数值相等，即时间到了

   ((LPTMR0)->CSR) &= ~LPTMR_CSR_TEN_MASK;          //清除比较标志位

    
}

//-------------------------------------------------------------------------*
//函数名: LPTMR_timing_us                                                        
//功  能: LPTMR定时函数（us）                                                        
//参  数: us        LPTMR定时时间(0~41942)
//返  回: 无                                                              
//简  例:   lptmr_timing_us(32);     // LPTMR 定时32us
//          set_vector_handler(LPTimer_VECTORn,lptmr_hander);   // 设置中断复位函数到中断向量表里
//          enable_irq(LPTimer_IRQn);                           // 使能LPTMR中断                     
//-------------------------------------------------------------------------*
void LPTMR_timing_us(uint16 us)
{
   
    if(us == 0)
    {
        return;
    }


    ((OSC0)->CR)|= OSC_CR_ERCLKEN_MASK;                              //使能 OSCERCLK


    ((SIM)->SCGC5) |= SIM_SCGC5_LPTMR_MASK;                        //使能LPT模块时钟

    ((LPTMR0)->CSR) = 0x00;                                          //先关了LPT，自动清计数器的值

    ((LPTMR0)->CMR) = (us * 50 + 16) / 32;                 //设置比较值，即延时时间，需要经过单位换算

    //选择时钟源
    ((LPTMR0)->PSR)  =   ( 0
                      | LPTMR_PSR_PCS(3)          //选择时钟源： 0 为 MCGIRCLK ，1为 LPO（1KHz） ，2为 ERCLK32K ，3为 OSCERCLK
                      //| LPTMR_PSR_PBYP_MASK     //旁路 预分频/干扰滤波器 ,即不用 预分频/干扰滤波器(注释了表示使用预分频/干扰滤波器)
                      | LPTMR_PSR_PRESCALE(4)     //预分频值 = 2^(n+1) ,n = 0~ 0xF
                    );

    //使能 LPT
    ((LPTMR0)->CSR)  =  (0
                    //| LPTMR_CSR_TPS(1)        // 选择输入管脚 选择
                    //| LPTMR_CSR_TMS_MASK      // 选择脉冲计数 (注释了表示时间计数模式)
                    //| ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //脉冲计数器触发方式选择：0为高电平有效，上升沿加1
                    | LPTMR_CSR_TEN_MASK        //使能LPT(注释了表示禁用)
                    | LPTMR_CSR_TIE_MASK      //中断使能
                    //| LPTMR_CSR_TFC_MASK      //0:计数值等于比较值就复位；1：溢出复位（注释表示0）
                   );

   return;                    
}
//-------------------------------------------------------------------------*
//函数名: LPTMR_time_start_ms                                                        
//功  能: 开始LPTMR计时                                                        
//参  数: 无
//返  回: 无                                                             
//简  例: LPTMR_time_start_ms();                      
//-------------------------------------------------------------------------*
void LPTMR_time_start_ms(void)
{
    ((SIM)->SCGC5)  |= SIM_SCGC5_LPTMR_MASK;    //使能LPT模块时钟

    ((LPTMR0)->CSR) = 0x00;                      //先关了LPT，自动清计数器的值

    ((LPTMR0)->CMR)= ~0;                        //设置比较值

    //选择时钟源
    ((LPTMR0)->PSR)  =   ( 0
                      | LPTMR_PSR_PCS(1)                  //选择时钟源： 0 为 MCGIRCLK ，1为 LPO（1KHz） ，2为 ERCLK32K ，3为 OSCERCLK
                      | LPTMR_PSR_PBYP_MASK               //旁路 预分频/干扰滤波器 ,即不用 预分频/干扰滤波器(注释了表示使用预分频/干扰滤波器)
                      //| LPTMR_PSR_PRESCALE(1)           //预分频值 = 2^(n+1) ,n = 0~ 0xF
                    );

    //使能 LPT
    ((LPTMR0)->CSR) =  (0
                    //| LPTMR_CSR_TPS(1)        // 选择输入管脚 选择
                    //| LPTMR_CSR_TMS_MASK      // 选择脉冲计数 (注释了表示时间计数模式)
                    //| ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //脉冲计数器触发方式选择：0为高电平有效，上升沿加1
                    | LPTMR_CSR_TEN_MASK        //使能LPT(注释了表示禁用)
                    //| LPTMR_CSR_TIE_MASK      //中断使能
                    //| LPTMR_CSR_TFC_MASK      //0:计数值等于比较值就复位；1：溢出复位（注释表示0）
                   );
    
}


//-------------------------------------------------------------------------*
//函数名: LPTMR_time_get_ms                                                        
//功  能: 获取LPTMR计时时间（ms）                                                        
//参  数: 无
//返  回: 计时时间（返回值为 ~0 表示计时超时，其他值在 0~ 65534ms 区间内 ）                                                              
//简  例: 参考 LPTMR_time_start_ms 的调用例子                          
//-------------------------------------------------------------------------*

uint32 LPTMR_time_get_ms(void)
{
    uint32 data;

    if(((LPTMR0)->CSR) & LPTMR_CSR_TCF_MASK)     //已经溢出了
    {

        data = ~0;                          //返回 0xffffffff 表示错误
    }
    else
    {
        data = ((LPTMR0)->CSR);                  //返回计时时间(此值最大为 0xffff)注意这里计时时间是CSR而计数是CNR
    }

    return data;
}

//-------------------------------------------------------------------------*
//函数名: LPTMR_time_close                                                        
//功  能: 关闭 LPTMR计时                                                        
//参  数: 无
//返  回: 无                                                              
//简  例: 无                        
//-------------------------------------------------------------------------*
void LPTMR_time_close()
{
    ((LPTMR0)->CSR) = 0x00;                                          //先关了LPT，自动清计数器的值，清空溢出标记
}
//-------------------------------------------------------------------------*
//函数名: LPTMR_time_start_us                                                        
//功  能: 开始LPTMR计时                                                        
//参  数: 无
//返  回: 无                                                             
//简  例: LPTMR_time_start_ms();                      
//-------------------------------------------------------------------------*
void LPTMR_time_start_us(void)
{


   ((OSC0)->CR)|= OSC_CR_ERCLKEN_MASK;                              //使能 OSCERCLK

    ((SIM)->SCGC5)  |= SIM_SCGC5_LPTMR_MASK;                        //使能LPT模块时钟

    ((LPTMR0)->CSR) = 0x00;                                          //先关了LPT，自动清计数器的值

    ((LPTMR0)->CMR)= ~0;                                            //设置比较值为最大值

    //选择时钟源
    ((LPTMR0)->PSR) =   ( 0
                      | LPTMR_PSR_PCS(3)          //选择时钟源： 0 为 MCGIRCLK ，1为 LPO（1KHz） ，2为 ERCLK32K ，3为 OSCERCLK
                      //| LPTMR_PSR_PBYP_MASK     //旁路 预分频/干扰滤波器 ,即不用 预分频/干扰滤波器(注释了表示使用预分频/干扰滤波器)
                      | LPTMR_PSR_PRESCALE(4)     //预分频值 = 2^(n+1) ,n = 0~ 0xF
                    );

    //使能 LPT
    ((LPTMR0)->CSR)  =  (0
                    //| LPTMR_CSR_TPS(1)        // 选择输入管脚 选择
                    //| LPTMR_CSR_TMS_MASK      // 选择脉冲计数 (注释了表示时间计数模式)
                    //| ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //脉冲计数器触发方式选择：0为高电平有效，上升沿加1
                    | LPTMR_CSR_TEN_MASK        //使能LPT(注释了表示禁用)
                    //| LPTMR_CSR_TIE_MASK      //中断使能
                    //| LPTMR_CSR_TFC_MASK      //0:计数值等于比较值就复位；1：溢出复位（注释表示0）
                   );
}


//-------------------------------------------------------------------------*
//函数名: LPTMR_time_get_us                                                        
//功  能: 获取LPTMR计时时间（us）                                                        
//参  数: 无
//返  回: 计时时间（返回值为 ~0 表示计时超时，其他值在 0~ 41942us 区间内 ）                                                              
//简  例: 参考 LPTMR_time_start_us 的调用例子                          
//-------------------------------------------------------------------------*
uint32 LPTMR_time_get_us(void)
{
    uint32 data;

    if(((LPTMR0)->CSR) & LPTMR_CSR_TCF_MASK)     //已经溢出了
    {

        data = ~0;                          //返回 0xffffffff 表示错误
    }
    else
    {

        data = (((LPTMR0)->CNR) * 32) / 50; //进行单位换算
    }

    return data;
}



//-------------------------------------------------------------------------*
//函数名: LPTMR_interrupt                                                        
//功  能: LPTMR定时中断函数                                                        
//参  数: 无
//返  回: 无                                                              
//简  例: 无                               
//-------------------------------------------------------------------------*
void LPTMR0_IRQHandler(void)
{
    LPTMR_Flag_Clear();

}