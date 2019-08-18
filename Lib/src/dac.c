#include "dac.h"
#include "port.h"
#include "assert.h"

/*!
 *  @brief      DAC初始化
 *  Sample usage:       dac_init ();    //初始化DAC
 */
void dac_init()
{
    port_init(PTE30, Disabled); //初始化端口

    /* 使能时钟 */
    SIM_SCGC6 |= SIM_SCGC6_DAC0_MASK;    //使能DAC模块

    /*  配置DAC寄存器  */

    //配置DAC_C0寄存器
    DAC0->C0 = ( 0
                |  DAC_C0_DACTRGSEL_MASK                //选择软件触发
                |  DAC_C0_DACRFS_MASK                   //选择参考VDD电压(3.3V)
                |  DAC_C0_DACEN_MASK                    //使能DAC模块
              );

    //配置DAC_C1寄存器
    DAC0->C1 = ( 0
                ) ;

    //配置DAC_C2寄存器
    DAC0->C2 = ( 0
               | DAC_C2_DACBFRP(0)             //设置缓冲区读指针指向0
                );

    DAC0->DAT[0].DATH = 0;  //默认输出最低电压
    DAC0->DAT[0].DATL = 0;
}

/*!
 *  @brief      DAC输出
 *  @param      val         输出模拟量所对应的数字量（12bit）
 *  Sample usage:       dac_out (0x100);    //DAC0输出 0x100 数字量对应的模拟量
 */
void dac_out(uint16_t val)
{
    assert(val < 0x1000);               //val 为 12bit

    DAC0->DAT[0].DATH = (val >> 8);   //输出电压
    DAC0->DAT[0].DATL = (val & 0xFF);
}

