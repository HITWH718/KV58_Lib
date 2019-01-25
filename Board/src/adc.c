#include "adc.h"
#include "port.h"
#include "fsl_common.h"

static void     adc_start   (ADCn_Ch_e, ADC_nbit);    //��ʼadcת��

/*!
 *  @brief      ADC��ʼ��
 *  @param      ADCn_Ch_e    ADCͨ��
 *  @note       �˳�ʼ����֧���������������ÿ��ͨ����֧��ADC ���������
                ����˵���� ADCn_Ch_e ��ע��˵��
 *  Sample usage:       adc_init (ADC0_SE10 );    //��ʼ�� ADC0_SE10
 */
void adc_init(ADCn_Ch_e adcn_ch)
{
    //����ADC0ʱ��
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
        assert(0);      //���ԣ����ݵĹܽŲ�֧�� ADC ��������������뻻 �����ܽ�
        break;
    }
}

/*!
 *  @brief      ��ȡADC����ֵ(��֧��Bͨ��)
 *  @param      ADCn_Ch_e    ADCͨ��
 *  @param      ADC_nbit     ADC���ȣ� ADC_8bit,ADC_12bit, ADC_10bit, ADC_16bit ��
 *  @return     ����ֵ
 *  Sample usage:       uint16_t var = adc_once(ADC0_SE10, ADC_8bit);
 */
uint16_t adc_once(ADCn_Ch_e adcn_ch, ADC_nbit bit) //�ɼ�ĳ·ģ������ADֵ
{
    uint16_t result = 0;

    adc_start(adcn_ch, bit);      //����ADCת��

    while((ADC0->SC1[0] & ADC_SC1_COCO_MASK) != ADC_SC1_COCO_MASK);
    result = ADC0->R[0];
    ADC0->SC1[0] &= ~ADC_SC1_COCO_MASK;    //ADCx_SC1n �Ĵ��� COCOλreset
    return result;
}

/*!
 *  @brief      ��ȡADC����ƽ��ֵ(��֧��Bͨ��)
 *  @param      ADCn_Ch_e    ADCͨ��
 *  @param      ADC_nbit     ADC���ȣ� ADC_8bit,ADC_12bit, ADC_10bit, ADC_16bit ��
 *  @param      times        ��������
 *  @return     ����ƽ��ֵ
 *  Sample usage:       uint16_t var = adc_average(ADC0_SE10, ADC_8bit��3);
 */
uint16_t adc_average(ADCn_Ch_e adcn_ch, ADC_nbit bit, uint8_t times)
{
    assert(times);  //times����Ϊ0
    uint16_t result = 0;
    for(uint8_t i = 0; i < times; i++)
    {
        result += adc_once(adcn_ch, bit);
    }

    return (result / times);
}


/*!
 *  @brief      ����ADC�������
 *  @param      ADCn_Ch_e    ADCͨ��
 *  @param      ADC_nbit     ADC���ȣ� ADC_8bit,ADC_12bit, ADC_10bit, ADC_16bit ��
 *  @note       �˺����ڲ����ã������󼴿ɵȴ����ݲɼ����
 *  Sample usage:       adc_start(ADC0_SE10, ADC_8bit);
 */
static void adc_start(ADCn_Ch_e adcn_ch, ADC_nbit bit)
{
    uint8_t ch = (uint8_t)(adcn_ch & 0x1F);

    //��ʼ��ADCĬ������
    //ADCx_CFG1 �Ĵ���
    ADC0->CFG1 = (0
                    //| ADC_CFG1_ADLPC_MASK         //ADC�������ã�0Ϊ�������ģ�1Ϊ�͹���
                    | ADC_CFG1_ADIV(2)              //ʱ�ӷ�Ƶѡ��,��Ƶϵ��Ϊ 2^n,2bit
                    | ADC_CFG1_ADLSMP_MASK          //����ʱ�����ã�0Ϊ�̲���ʱ�䣬1 Ϊ������ʱ��
                    | ADC_CFG1_MODE(bit)
                    | ADC_CFG1_ADICLK(0)            //0Ϊ����ʱ��,1Ϊ����ʱ��/2,2Ϊ����ʱ�ӣ�ALTCLK����3Ϊ �첽ʱ�ӣ�ADACK����
                    );

    ADC0->CFG2 = (0
                    //| ADC_CFG2_MUXSEL_MASK       //ADC����ѡ��,0Ϊaͨ����1Ϊbͨ����
                    //| ADC_CFG2_ADACKEN_MASK      //�첽ʱ�����ʹ��,0Ϊ��ֹ��1Ϊʹ�ܡ�
                    | ADC_CFG2_ADHSC_MASK          //��������,0Ϊ����ת�����У�1Ϊ����ת������
                    | ADC_CFG2_ADLSTS(0)           //������ʱ��ѡ��ADCKΪ4+n������ѭ��������ѭ����0Ϊ20��1Ϊ12��2Ϊ6��3Ϊ2
                );

    //д�� SC1A ����ת��
    ADC0->SC1[0] = (0
                    | ADC_SC1_AIEN_MASK          // ת������ж�,0Ϊ��ֹ��1Ϊʹ��
                    //| ADC_SC1_DIFF_MASK        // ���ģʽʹ��,0Ϊ���ˣ�1Ϊ���
                    | ADC_SC1_ADCH( ch )
                    );
}

/*!
 *  @brief      ֹͣADC�������
 *  @param      ADCn_e       ADCģ��ţ� ADC0�� ADC1��
 *  Sample usage:       adc_stop(ADC0);
 */
void adc_stop()
{
     //д�� SC1A ����ת��
    ADC0->SC1[0] = (0
                    | ADC_SC1_AIEN_MASK          // ת������ж�,0Ϊ��ֹ��1Ϊʹ��
                    //| ADC_SC1_DIFF_MASK        // ���ģʽʹ��,0Ϊ���ˣ�1Ϊ���
                    | ADC_SC1_ADCH( 0x1F )       //����ͨ��ѡ��,�˴�ѡ���ֹͨ��
                    );
}

