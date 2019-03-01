/*!
 * @file       lptmr.c
 * @brief      lptmr����������������ʱ����ʱ���������
 * @author     Sylvia_Li
 * @version    v1.0
 * @date       2019-01-22
 */
#include "lptmr.h"
#include "MKV58F24.h"
//-------------------------------------------------------------------------*
//������: LPTMR_pulse_init                                                        
//��  ��: LPTMR���������ʼ��                                                        
//��  ��: LPT0_ALTn:LPTMR��������ܽ�
//        count   :LPTMR����Ƚ�ֵ
//        LPT_CFG :LPTMR���������ʽ�������ؼ������½��ؼ���
//��  ��: ��                                                              
//��  ��:  LPTMR_pulse_init(LPT0_ALT1,32768,LPT_Rising);                                
//-------------------------------------------------------------------------*
void LPTMR_Pulse_Init(LPT0_ALTn altn, uint16 count, LPT_CFG cfg)
{    

  
    ((SIM)->SCGC5) |= SIM_SCGC5_LPTMR_MASK;   //ʹ��LPTģ��ʱ��
    
    if(altn == LPT0_ALT1)  //��������ܽ�
    {
           ((PORTA)->PCR[19])= PORT_PCR_MUX(0x6); //�ܽ�PORTA19
    }
    else if(altn == LPT0_ALT2)
    {
           ((PORTC)->PCR[5])= PORT_PCR_MUX(0x4);  //�ܽ�PORTC5
    }
    else                                    //�����ܷ����¼�
    {
       ;  
    }

     // ��״̬�Ĵ���
     ((LPTMR0)->CSR)= 0x00;    //��Ҫ�ȹر�LPT����ʱ�ӷ�Ƶ ����ʱ�ӷ�Ƶ��

     
      //ѡ��ʱ��Դ
      ((LPTMR0)->PSR)  =   ( 0  | LPTMR_PSR_PCS(1)                  //ѡ��ʱ��Դ�� 0 Ϊ MCGIRCLK ��1Ϊ LPO��1KHz�� ��2Ϊ ERCLK32K ��3Ϊ OSCERCLK
                                | LPTMR_PSR_PBYP_MASK               //��· Ԥ��Ƶ/�����˲��� ,������ Ԥ��Ƶ/�����˲���(ע���˱�ʾʹ��Ԥ��Ƶ/�����˲���)
                      //| LPTMR_PSR_PRESCALE(1)           //Ԥ��Ƶֵ = 2^(n+1) ,n = 0~ 0xF
                            );


     // �����ۼӼ���ֵ
     ((LPTMR0)->CMR) =   LPTMR_CMR_COMPARE(count);                   //���ñȽ�ֵ

     // �ܽ����á�ʹ���ж�
     ((LPTMR0)->CSR) =  (0
                    | LPTMR_CSR_TPS(altn)       // ѡ������ܽ� ѡ��
                    | LPTMR_CSR_TMS_MASK        // ѡ��������� (ע���˱�ʾʱ�����ģʽ)
                    | ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //���������������ʽѡ��0Ϊ�ߵ�ƽ��Ч�������ؼ�1
                    | LPTMR_CSR_TEN_MASK        //ʹ��LPT(ע���˱�ʾ����)
                    | LPTMR_CSR_TIE_MASK        //�ж�ʹ��
                    //| LPTMR_CSR_TFC_MASK      //0:����ֵ���ڱȽ�ֵ�͸�λ��1�������λ��ע�ͱ�ʾ0��
                   );
}

//-------------------------------------------------------------------------*
//������: LPTMR_pulse_get                                                        
//��  ��: ��ȡLPTMR�������ֵ                                                        
//��  ��: �������ֵ                                                              
//��  ��: ��                                
//-------------------------------------------------------------------------*
uint16 LPTMR_Pulse_Get(void)
{
    uint16 data;
    if(((LPTMR0)->CSR) & LPTMR_CSR_TCF_MASK)     //�Ѿ������
    {
        data = ~0;                              //���� 0xffffffff ��ʾ����
    }
    else
    {
        data = ((LPTMR0)->CNR);
    }
    return data;
}


//-------------------------------------------------------------------------*
//������: LPTMR_pulse_clean                                                        
//��  ��: ���LPTMR�������                                                        
//��  ��: ��
//��  ��: ��                                                              
//��  ��: ��                                
//-------------------------------------------------------------------------*
void LPTMR_Pulse_Clean(void)
{
    ((LPTMR0)->CSR)  &= ~LPTMR_CSR_TEN_MASK;     //����LPT��ʱ��ͻ��Զ����������ֵ
    ((LPTMR0)->CSR)  |=  LPTMR_CSR_TEN_MASK;     //�൱���ȹر��ٿ���
}

//-------------------------------------------------------------------------*
//������: LPTMR_delay_ms                                                        
//��  ��: LPTMR��ʱ������ms��                                                        
//��  ��: ms    ����
//��  ��: ��                                                              
//��  ��: LPTMR_delay_ms(32);     // LPTMR ��ʱ32ms                                
//-------------------------------------------------------------------------*
void LPTMR_delay_ms(uint16 ms)
{
    if(ms == 0)
    {
        return;
    }

    ((SIM)->SCGC5) |= SIM_SCGC5_LPTMR_MASK;    //ʹ��LPTģ��ʱ��

    ((LPTMR0)->CSR) = 0x00;                      //�ȹ���LPT���Զ����������ֵ

    ((LPTMR0)->CMR) = ms;                        //���ñȽ�ֵ������ʱʱ��

    //ѡ��ʱ��Դ
    ((LPTMR0)->PSR) =   ( 0
                      | LPTMR_PSR_PCS(1)                  //ѡ��ʱ��Դ�� 0 Ϊ MCGIRCLK ��1Ϊ LPO��1KHz�� ��2Ϊ ERCLK32K ��3Ϊ OSCERCLK
                      | LPTMR_PSR_PBYP_MASK               //��· Ԥ��Ƶ/�����˲��� ,������ Ԥ��Ƶ/�����˲���(ע���˱�ʾʹ��Ԥ��Ƶ/�����˲���)
                      //| LPTMR_PSR_PRESCALE(1)           //Ԥ��Ƶֵ = 2^(n+1) ,n = 0~ 0xF
                    );

    //ʹ�� LPT
    ((LPTMR0)->CSR)  =  (0
                    //| LPTMR_CSR_TPS(1)        // ѡ������ܽ� ѡ��
                    //| LPTMR_CSR_TMS_MASK      // ѡ��������� (ע���˱�ʾʱ�����ģʽ)
                    //| ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //���������������ʽѡ��0Ϊ�ߵ�ƽ��Ч�������ؼ�1
                     | LPTMR_CSR_TEN_MASK        //ʹ��LPT(ע���˱�ʾ����)
                    //| LPTMR_CSR_TIE_MASK      //�ж�ʹ��
                    //| LPTMR_CSR_TFC_MASK      //0:����ֵ���ڱȽ�ֵ�͸�λ��1�������λ��ע�ͱ�ʾ0��
                    );

    while (!(((LPTMR0)->CMR) & LPTMR_CSR_TCF_MASK)); //�ȴ��Ƚ�ֵ�����ֵ��ȣ���ʱ�䵽��

    ((LPTMR0)->CSR) &= ~LPTMR_CSR_TEN_MASK;          //����Ƚϱ�־λ

    return;
}

//-------------------------------------------------------------------------*
//������: LPTMR_timing_ms                                                        
//��  ��: LPTMR��ʱ������ms��                                                        
//��  ��: ms          LPTMR��ʱʱ��(0~65535)
//��  ��: ��                                                              
//��  ��:  LPTMR_timing_ms(32);                                // LPTMR ��ʱ 32ms
//         set_vector_handler(LPTimer_VECTORn,lptmr_hander);   // �����жϸ�λ�������ж���������
//         enable_irq(LPTimer_IRQn);                           // ʹ��LPTMR�ж�                            
//-------------------------------------------------------------------------*
void LPTMR_timing_ms(uint16 ms)
{
    if(ms == 0)
    {
        return;
    }

    ((SIM)->SCGC5) |= SIM_SCGC5_LPTMR_MASK;    //ʹ��LPTģ��ʱ��

    ((LPTMR0)->CSR)= 0x00;                      //�ȹ���LPT���Զ����������ֵ

    ((LPTMR0)->CMR) = ms;                        //���ñȽ�ֵ������ʱʱ��

    //ѡ��ʱ��Դ
    ((LPTMR0)->PSR)  =   ( 0
                      | LPTMR_PSR_PCS(1)                  //ѡ��ʱ��Դ�� 0 Ϊ MCGIRCLK ��1Ϊ LPO��1KHz�� ��2Ϊ ERCLK32K ��3Ϊ OSCERCLK
                      | LPTMR_PSR_PBYP_MASK               //��· Ԥ��Ƶ/�����˲��� ,������ Ԥ��Ƶ/�����˲���(ע���˱�ʾʹ��Ԥ��Ƶ/�����˲���)
                      //| LPTMR_PSR_PRESCALE(1)           //Ԥ��Ƶֵ = 2^(n+1) ,n = 0~ 0xF
                    );

    //ʹ�� LPT
    ((LPTMR0)->CSR)  =  (0
                    //| LPTMR_CSR_TPS(1)        // ѡ������ܽ� ѡ��
                    //| LPTMR_CSR_TMS_MASK      // ѡ��������� (ע���˱�ʾʱ�����ģʽ)
                    //| ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //���������������ʽѡ��0Ϊ�ߵ�ƽ��Ч�������ؼ�1
                    | LPTMR_CSR_TEN_MASK        //ʹ��LPT(ע���˱�ʾ����)
                    | LPTMR_CSR_TIE_MASK        //�ж�ʹ��
                    //| LPTMR_CSR_TFC_MASK      //0:����ֵ���ڱȽ�ֵ�͸�λ��1�������λ��ע�ͱ�ʾ0��
                   );

    return;                    
}


//-------------------------------------------------------------------------*
//������: LPTMR_delay_us                                                        
//��  ��: LPTMR��ʱ������us��                                                        
//��  ��: us          LPTMR��ʱʱ��(0~41942)
//��  ��: ��                                                              
//��  ��:  LPTMR_delay_us(32);     // LPTMR ��ʱ32us                              
//-------------------------------------------------------------------------*
void LPTMR_delay_us(uint16 us)
{

    if(us == 0)
    {
        return;
    }

    ((OSC0)->CR) |= OSC_CR_ERCLKEN_MASK;                              //ʹ�� OSCERCLK


    ((SIM)->SCGC5) |= SIM_SCGC5_LPTMR_MASK;                        //ʹ��LPTģ��ʱ��

    ((LPTMR0)->CSR)  = 0x00;                                          //�ȹ���LPT���Զ����������ֵ

    ((LPTMR0)->CMR) = (us * 50 + 16) / 32;                 //���ñȽ�ֵ������ʱʱ��

    //ѡ��ʱ��Դ
   ((LPTMR0)->PSR) =   ( 0
                      | LPTMR_PSR_PCS(3)          //ѡ��ʱ��Դ�� 0 Ϊ MCGIRCLK ��1Ϊ LPO��1KHz�� ��2Ϊ ERCLK32K ��3Ϊ OSCERCLK
                      //| LPTMR_PSR_PBYP_MASK     //��· Ԥ��Ƶ/�����˲��� ,������ Ԥ��Ƶ/�����˲���(ע���˱�ʾʹ��Ԥ��Ƶ/�����˲���)
                      | LPTMR_PSR_PRESCALE(4)     //Ԥ��Ƶֵ = 2^(n+1) ,n = 0~ 0xF
                    );

    //ʹ�� LPT
    ((LPTMR0)->CSR) =  (0
                    //| LPTMR_CSR_TPS(1)        // ѡ������ܽ� ѡ��
                    //| LPTMR_CSR_TMS_MASK      // ѡ��������� (ע���˱�ʾʱ�����ģʽ)
                    //| ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //���������������ʽѡ��0Ϊ�ߵ�ƽ��Ч�������ؼ�1
                    | LPTMR_CSR_TEN_MASK        //ʹ��LPT(ע���˱�ʾ����)
                    //| LPTMR_CSR_TIE_MASK      //�ж�ʹ��
                    //| LPTMR_CSR_TFC_MASK      //0:����ֵ���ڱȽ�ֵ�͸�λ��1�������λ��ע�ͱ�ʾ0��
                   );

    while (!(((LPTMR0)->CSR) & LPTMR_CSR_TCF_MASK)); //�ȴ��Ƚ�ֵ�����ֵ��ȣ���ʱ�䵽��

   ((LPTMR0)->CSR) &= ~LPTMR_CSR_TEN_MASK;          //����Ƚϱ�־λ

    
}

//-------------------------------------------------------------------------*
//������: LPTMR_timing_us                                                        
//��  ��: LPTMR��ʱ������us��                                                        
//��  ��: us        LPTMR��ʱʱ��(0~41942)
//��  ��: ��                                                              
//��  ��:   lptmr_timing_us(32);     // LPTMR ��ʱ32us
//          set_vector_handler(LPTimer_VECTORn,lptmr_hander);   // �����жϸ�λ�������ж���������
//          enable_irq(LPTimer_IRQn);                           // ʹ��LPTMR�ж�                     
//-------------------------------------------------------------------------*
void LPTMR_timing_us(uint16 us)
{
   
    if(us == 0)
    {
        return;
    }


    ((OSC0)->CR)|= OSC_CR_ERCLKEN_MASK;                              //ʹ�� OSCERCLK


    ((SIM)->SCGC5) |= SIM_SCGC5_LPTMR_MASK;                        //ʹ��LPTģ��ʱ��

    ((LPTMR0)->CSR) = 0x00;                                          //�ȹ���LPT���Զ����������ֵ

    ((LPTMR0)->CMR) = (us * 50 + 16) / 32;                 //���ñȽ�ֵ������ʱʱ�䣬��Ҫ������λ����

    //ѡ��ʱ��Դ
    ((LPTMR0)->PSR)  =   ( 0
                      | LPTMR_PSR_PCS(3)          //ѡ��ʱ��Դ�� 0 Ϊ MCGIRCLK ��1Ϊ LPO��1KHz�� ��2Ϊ ERCLK32K ��3Ϊ OSCERCLK
                      //| LPTMR_PSR_PBYP_MASK     //��· Ԥ��Ƶ/�����˲��� ,������ Ԥ��Ƶ/�����˲���(ע���˱�ʾʹ��Ԥ��Ƶ/�����˲���)
                      | LPTMR_PSR_PRESCALE(4)     //Ԥ��Ƶֵ = 2^(n+1) ,n = 0~ 0xF
                    );

    //ʹ�� LPT
    ((LPTMR0)->CSR)  =  (0
                    //| LPTMR_CSR_TPS(1)        // ѡ������ܽ� ѡ��
                    //| LPTMR_CSR_TMS_MASK      // ѡ��������� (ע���˱�ʾʱ�����ģʽ)
                    //| ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //���������������ʽѡ��0Ϊ�ߵ�ƽ��Ч�������ؼ�1
                    | LPTMR_CSR_TEN_MASK        //ʹ��LPT(ע���˱�ʾ����)
                    | LPTMR_CSR_TIE_MASK      //�ж�ʹ��
                    //| LPTMR_CSR_TFC_MASK      //0:����ֵ���ڱȽ�ֵ�͸�λ��1�������λ��ע�ͱ�ʾ0��
                   );

   return;                    
}
//-------------------------------------------------------------------------*
//������: LPTMR_time_start_ms                                                        
//��  ��: ��ʼLPTMR��ʱ                                                        
//��  ��: ��
//��  ��: ��                                                             
//��  ��: LPTMR_time_start_ms();                      
//-------------------------------------------------------------------------*
void LPTMR_time_start_ms(void)
{
    ((SIM)->SCGC5)  |= SIM_SCGC5_LPTMR_MASK;    //ʹ��LPTģ��ʱ��

    ((LPTMR0)->CSR) = 0x00;                      //�ȹ���LPT���Զ����������ֵ

    ((LPTMR0)->CMR)= ~0;                        //���ñȽ�ֵ

    //ѡ��ʱ��Դ
    ((LPTMR0)->PSR)  =   ( 0
                      | LPTMR_PSR_PCS(1)                  //ѡ��ʱ��Դ�� 0 Ϊ MCGIRCLK ��1Ϊ LPO��1KHz�� ��2Ϊ ERCLK32K ��3Ϊ OSCERCLK
                      | LPTMR_PSR_PBYP_MASK               //��· Ԥ��Ƶ/�����˲��� ,������ Ԥ��Ƶ/�����˲���(ע���˱�ʾʹ��Ԥ��Ƶ/�����˲���)
                      //| LPTMR_PSR_PRESCALE(1)           //Ԥ��Ƶֵ = 2^(n+1) ,n = 0~ 0xF
                    );

    //ʹ�� LPT
    ((LPTMR0)->CSR) =  (0
                    //| LPTMR_CSR_TPS(1)        // ѡ������ܽ� ѡ��
                    //| LPTMR_CSR_TMS_MASK      // ѡ��������� (ע���˱�ʾʱ�����ģʽ)
                    //| ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //���������������ʽѡ��0Ϊ�ߵ�ƽ��Ч�������ؼ�1
                    | LPTMR_CSR_TEN_MASK        //ʹ��LPT(ע���˱�ʾ����)
                    //| LPTMR_CSR_TIE_MASK      //�ж�ʹ��
                    //| LPTMR_CSR_TFC_MASK      //0:����ֵ���ڱȽ�ֵ�͸�λ��1�������λ��ע�ͱ�ʾ0��
                   );
    
}


//-------------------------------------------------------------------------*
//������: LPTMR_time_get_ms                                                        
//��  ��: ��ȡLPTMR��ʱʱ�䣨ms��                                                        
//��  ��: ��
//��  ��: ��ʱʱ�䣨����ֵΪ ~0 ��ʾ��ʱ��ʱ������ֵ�� 0~ 65534ms ������ ��                                                              
//��  ��: �ο� LPTMR_time_start_ms �ĵ�������                          
//-------------------------------------------------------------------------*

uint32 LPTMR_time_get_ms(void)
{
    uint32 data;

    if(((LPTMR0)->CSR) & LPTMR_CSR_TCF_MASK)     //�Ѿ������
    {

        data = ~0;                          //���� 0xffffffff ��ʾ����
    }
    else
    {
        data = ((LPTMR0)->CSR);                  //���ؼ�ʱʱ��(��ֵ���Ϊ 0xffff)ע�������ʱʱ����CSR��������CNR
    }

    return data;
}

//-------------------------------------------------------------------------*
//������: LPTMR_time_close                                                        
//��  ��: �ر� LPTMR��ʱ                                                        
//��  ��: ��
//��  ��: ��                                                              
//��  ��: ��                        
//-------------------------------------------------------------------------*
void LPTMR_time_close()
{
    ((LPTMR0)->CSR) = 0x00;                                          //�ȹ���LPT���Զ����������ֵ�����������
}
//-------------------------------------------------------------------------*
//������: LPTMR_time_start_us                                                        
//��  ��: ��ʼLPTMR��ʱ                                                        
//��  ��: ��
//��  ��: ��                                                             
//��  ��: LPTMR_time_start_ms();                      
//-------------------------------------------------------------------------*
void LPTMR_time_start_us(void)
{


   ((OSC0)->CR)|= OSC_CR_ERCLKEN_MASK;                              //ʹ�� OSCERCLK

    ((SIM)->SCGC5)  |= SIM_SCGC5_LPTMR_MASK;                        //ʹ��LPTģ��ʱ��

    ((LPTMR0)->CSR) = 0x00;                                          //�ȹ���LPT���Զ����������ֵ

    ((LPTMR0)->CMR)= ~0;                                            //���ñȽ�ֵΪ���ֵ

    //ѡ��ʱ��Դ
    ((LPTMR0)->PSR) =   ( 0
                      | LPTMR_PSR_PCS(3)          //ѡ��ʱ��Դ�� 0 Ϊ MCGIRCLK ��1Ϊ LPO��1KHz�� ��2Ϊ ERCLK32K ��3Ϊ OSCERCLK
                      //| LPTMR_PSR_PBYP_MASK     //��· Ԥ��Ƶ/�����˲��� ,������ Ԥ��Ƶ/�����˲���(ע���˱�ʾʹ��Ԥ��Ƶ/�����˲���)
                      | LPTMR_PSR_PRESCALE(4)     //Ԥ��Ƶֵ = 2^(n+1) ,n = 0~ 0xF
                    );

    //ʹ�� LPT
    ((LPTMR0)->CSR)  =  (0
                    //| LPTMR_CSR_TPS(1)        // ѡ������ܽ� ѡ��
                    //| LPTMR_CSR_TMS_MASK      // ѡ��������� (ע���˱�ʾʱ�����ģʽ)
                    //| ( cfg == LPT_Falling ?  LPTMR_CSR_TPP_MASK :   0  )  //���������������ʽѡ��0Ϊ�ߵ�ƽ��Ч�������ؼ�1
                    | LPTMR_CSR_TEN_MASK        //ʹ��LPT(ע���˱�ʾ����)
                    //| LPTMR_CSR_TIE_MASK      //�ж�ʹ��
                    //| LPTMR_CSR_TFC_MASK      //0:����ֵ���ڱȽ�ֵ�͸�λ��1�������λ��ע�ͱ�ʾ0��
                   );
}


//-------------------------------------------------------------------------*
//������: LPTMR_time_get_us                                                        
//��  ��: ��ȡLPTMR��ʱʱ�䣨us��                                                        
//��  ��: ��
//��  ��: ��ʱʱ�䣨����ֵΪ ~0 ��ʾ��ʱ��ʱ������ֵ�� 0~ 41942us ������ ��                                                              
//��  ��: �ο� LPTMR_time_start_us �ĵ�������                          
//-------------------------------------------------------------------------*
uint32 LPTMR_time_get_us(void)
{
    uint32 data;

    if(((LPTMR0)->CSR) & LPTMR_CSR_TCF_MASK)     //�Ѿ������
    {

        data = ~0;                          //���� 0xffffffff ��ʾ����
    }
    else
    {

        data = (((LPTMR0)->CNR) * 32) / 50; //���е�λ����
    }

    return data;
}



//-------------------------------------------------------------------------*
//������: LPTMR_interrupt                                                        
//��  ��: LPTMR��ʱ�жϺ���                                                        
//��  ��: ��
//��  ��: ��                                                              
//��  ��: ��                               
//-------------------------------------------------------------------------*
void LPTMR0_IRQHandler(void)
{
    LPTMR_Flag_Clear();

}