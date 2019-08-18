#include "dac.h"
#include "port.h"
#include "assert.h"

/*!
 *  @brief      DAC��ʼ��
 *  Sample usage:       dac_init ();    //��ʼ��DAC
 */
void dac_init()
{
    port_init(PTE30, Disabled); //��ʼ���˿�

    /* ʹ��ʱ�� */
    SIM_SCGC6 |= SIM_SCGC6_DAC0_MASK;    //ʹ��DACģ��

    /*  ����DAC�Ĵ���  */

    //����DAC_C0�Ĵ���
    DAC0->C0 = ( 0
                |  DAC_C0_DACTRGSEL_MASK                //ѡ���������
                |  DAC_C0_DACRFS_MASK                   //ѡ��ο�VDD��ѹ(3.3V)
                |  DAC_C0_DACEN_MASK                    //ʹ��DACģ��
              );

    //����DAC_C1�Ĵ���
    DAC0->C1 = ( 0
                ) ;

    //����DAC_C2�Ĵ���
    DAC0->C2 = ( 0
               | DAC_C2_DACBFRP(0)             //���û�������ָ��ָ��0
                );

    DAC0->DAT[0].DATH = 0;  //Ĭ�������͵�ѹ
    DAC0->DAT[0].DATL = 0;
}

/*!
 *  @brief      DAC���
 *  @param      val         ���ģ��������Ӧ����������12bit��
 *  Sample usage:       dac_out (0x100);    //DAC0��� 0x100 ��������Ӧ��ģ����
 */
void dac_out(uint16_t val)
{
    assert(val < 0x1000);               //val Ϊ 12bit

    DAC0->DAT[0].DATH = (val >> 8);   //�����ѹ
    DAC0->DAT[0].DATL = (val & 0xFF);
}

