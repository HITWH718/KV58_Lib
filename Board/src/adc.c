#include "adc.h"
#include "port.h"
#include "fsl_common.h"

static void     adc_start   (ADCn_Ch_e, ADC_nbit);    //开始adc转换

/*!
 *  @brief      ADC初始化
 *  @param      ADCn_Ch_e    ADC通道
 *  @note       此初始化仅支持软件触发，不是每个通道都支持ADC 软件触发，
                具体说明见 ADCn_Ch_e 的注释说明
 *  Sample usage:       adc_init (ADC0_SE10 );    //初始化 ADC0_SE10
 */
void adc_init(ADCn_Ch_e adcn_ch)
{
    //开启ADC0时钟
    SIM->SCGC6 |= (SIM_SCGC6_ADC0_MASK);
    SIM->SOPT7 &= ~(SIM_SOPT7_HSADC0AALTTRGEN_MASK  | SIM_SOPT7_HSADC0AALTTRGEN_MASK);
    SIM->SOPT7 |= SIM_SOPT7_HSADC0ATRGSEL(0);

    switch(adcn_ch)
    {
    case ADC0_DP1:
        port_init(PTE16, Disabled);
        break;
    case ADC0_DP2:
        port_init(PTE4, Disabled);
        break;
    case ADC0_DP3:
        port_init(PTE11, Disabled);
        break;
    case ADC0_SE4a:
        port_init(PTE6, Disabled);
        break;
    case ADC0_SE5a:
        port_init(PTE18, Disabled);
        break;
    case ADC0_SE6a:
        port_init(PTE19, Disabled);
        break;
    case ADC0_SE5b:
        port_init(PTE20, Disabled);
        break;
    case ADC0_SE6b:
        port_init(PTB4, Disabled);
        break;
    case ADC0_SE7b:
        port_init(PTB5, Disabled);
        break;
    case ADC0_SE9:
        port_init(PTE17, Disabled);
        break;
    case ADC0_SE10:
        port_init(PTE5, Disabled);
        break;
    case ADC0_SE11:
        port_init(PTE12, Disabled);
        break;

    case ADC0_DP0:
    case ADC0_SE7a:
    case ADC0_SE4b:
    case ADC0_SE8:
        assert(0);
        break;
    default:
        assert(0);      //断言，传递的管脚不支持 ADC 单端软件触发，请换 其他管脚
        break;
    }
}

/*!
 *  @brief      获取ADC采样值(不支持B通道)
 *  @param      ADCn_Ch_e    ADC通道
 *  @param      ADC_nbit     ADC精度（ ADC_8bit,ADC_12bit, ADC_10bit, ADC_16bit ）
 *  @return     采样值
 *  Sample usage:       uint16_t var = adc_once(ADC0_SE10, ADC_8bit);
 */
uint16_t adc_once(ADCn_Ch_e adcn_ch, ADC_nbit bit) //采集某路模拟量的AD值
{
    uint16_t result = 0;

    adc_start(adcn_ch, bit);      //启动ADC转换

    while((ADC0->SC1[0] & ADC_SC1_COCO_MASK) != ADC_SC1_COCO_MASK);
    result = ADC0->R[0];
    ADC0->SC1[0] &= ~ADC_SC1_COCO_MASK;    //ADCx_SC1n 寄存器 COCO位reset
    return result;
}

/*!
 *  @brief      获取ADC采样平均值(不支持B通道)
 *  @param      ADCn_Ch_e    ADC通道
 *  @param      ADC_nbit     ADC精度（ ADC_8bit,ADC_12bit, ADC_10bit, ADC_16bit ）
 *  @param      times        采样次数
 *  @return     采样平均值
 *  Sample usage:       uint16_t var = adc_average(ADC0_SE10, ADC_8bit，3);
 */
uint16_t adc_average(ADCn_Ch_e adcn_ch, ADC_nbit bit, uint8_t times)
{
    assert(times);  //times不能为0
    uint16_t result = 0;
    for(uint8_t i = 0; i < times; i++)
    {
        result += adc_once(adcn_ch, bit);
    }

    return (result / times);
}


/*!
 *  @brief      启动ADC软件采样
 *  @param      ADCn_Ch_e    ADC通道
 *  @param      ADC_nbit     ADC精度（ ADC_8bit,ADC_12bit, ADC_10bit, ADC_16bit ）
 *  @note       此函数内部调用，启动后即可等待数据采集完成
 *  Sample usage:       adc_start(ADC0_SE10, ADC_8bit);
 */
static void adc_start(ADCn_Ch_e adcn_ch, ADC_nbit bit)
{
    uint8_t ch = (uint8_t)(adcn_ch & 0x1F);

    //初始化ADC默认配置
    //ADCx_CFG1 寄存器
    ADC0->CFG1 = (0
                    //| ADC_CFG1_ADLPC_MASK         //ADC功耗配置，0为正常功耗，1为低功耗
                    | ADC_CFG1_ADIV(2)              //时钟分频选择,分频系数为 2^n,2bit
                    | ADC_CFG1_ADLSMP_MASK          //采样时间配置，0为短采样时间，1 为长采样时间
                    | ADC_CFG1_MODE(bit)
                    | ADC_CFG1_ADICLK(0)            //0为总线时钟,1为总线时钟/2,2为交替时钟（ALTCLK），3为 异步时钟（ADACK）。
                    );

    ADC0->CFG2 = (0
                    //| ADC_CFG2_MUXSEL_MASK       //ADC复用选择,0为a通道，1为b通道。
                    //| ADC_CFG2_ADACKEN_MASK      //异步时钟输出使能,0为禁止，1为使能。
                    | ADC_CFG2_ADHSC_MASK          //高速配置,0为正常转换序列，1为高速转换序列
                    | ADC_CFG2_ADLSTS(0)           //长采样时间选择，ADCK为4+n个额外循环，额外循环，0为20，1为12，2为6，3为2
                );

    //写入 SC1A 启动转换
    ADC0->SC1[0] = (0
                    | ADC_SC1_AIEN_MASK          // 转换完成中断,0为禁止，1为使能
                    //| ADC_SC1_DIFF_MASK        // 差分模式使能,0为单端，1为差分
                    | ADC_SC1_ADCH( ch )
                    );
}

/*!
 *  @brief      停止ADC软件采样
 *  @param      ADCn_e       ADC模块号（ ADC0、 ADC1）
 *  Sample usage:       adc_stop(ADC0);
 */
void adc_stop()
{
     //写入 SC1A 启动转换
    ADC0->SC1[0] = (0
                    | ADC_SC1_AIEN_MASK          // 转换完成中断,0为禁止，1为使能
                    //| ADC_SC1_DIFF_MASK        // 差分模式使能,0为单端，1为差分
                    | ADC_SC1_ADCH( 0x1F )       //输入通道选择,此处选择禁止通道
                    );
}

