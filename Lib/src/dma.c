#include "dma.h"
#include "MKV58F24.h"
//#include "common.h"
#include "port.h"







//dma��û�г�ʼ���������ţ���Ҫ������ʼ����Դ��ַʹ��GPIO_BYT0_PDIR(gpio)  ��GPIO_BYT1_PDIR(gpio)  ��



/*!
 *  @brief      DMA��ʼ������IO������������˿ڵ����ݵ��ڴ�
 *  @param      DMA_CHn         ͨ���ţ�DMA_CH0 ~ DMA_CH15��
 *  @param      SADDR           Դ��ַ
 *  @param      DADDR           Ŀ�ĵ�ַ
 *  @param      PTxn            �����˿�
 *  @param      DMA_BYTEn       ÿ��DMA�����ֽ���
 *  @param      count           һ����ѭ�������ֽ���
 *  @param      cfg             DMA�������ã���DMA_cfg��ѡ��
 *  @note       DMA PTXn����ԴĬ�������ش������䣬�����޸ģ����ʼ������� port_init ����DMA ������ʽ
                ��ʼ������Ҫ���� DMA_EN ��ʵ��
 *  Sample usage:   uint8 buff[10];
                    dma_portx2buff_init(DMA_CH0, &GPIO_PDIR_REG(GPIOD), buff, PTA7, DMA_BYTE1, 10 ,DADDR_RECOVER,DMA_PERIPHERAL_TO_MEMORY);
                    //DMA��ʼ����Դ��ַ��&GPIO_PDIR_REG(GPIOD)��Ŀ�ĵ�ַ��buff,PTA7����(Ĭ��������)��ÿ�δ���1�ֽڣ������� 10�� �����������ָ���ַ

                    port_init(PTA7,ALT1 | DMA_FALLING);             //Ĭ�ϴ���Դ�������أ��˴���Ϊ �½��ش���

                    DMA_EN(DMA_CH0);                //��Ҫʹ�� DMA ����ܴ�������
 */
void dma_portx2buff_init(DMA_CHn CHn, void *SADDR, void *DADDR, PTXn_e ptxn, DMA_BYTEn byten, uint32_t count, uint32_t cfg, DMA_TYPE type)
{
  uint8_t BYTEs = (byten == DMA_BYTE1 ? 1 : (byten == DMA_BYTE2 ? 2 : (byten == DMA_BYTE4 ? 4 : 16 ) ) ); //���㴫���ֽ���
  assert(count < 0x8000);                                                                               //���ԣ����ֻ֧��0x7FFF
  
  SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;                                                                    //��DMAģ��ʱ��
  SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
  //���ô�����ƿ�TCD
  DMA_SADDR_REG(DMA0,CHn) = (uint32_t)SADDR;              //Դ��ַ
  DMA_DADDR_REG(DMA0,CHn) = (uint32_t)DADDR;              //Ŀ���ַ
  //ѡ���䷽��һ��ΪDMA_PERIPHERAL_TO_MEMORY
  switch(type)
  {
  case DMA_PERIPHERAL_TO_MEMORY:
    DMA_SOFF_REG(DMA0,CHn)  =    0x00u;                                //Դ��ַƫ��
    DMA_DOFF_REG(DMA0,CHn)  =    BYTEs;                                //Ŀ�ĵ�ַƫ��
    break;
  case DMA_MEMORY_TO_MEMORY:
    DMA_SOFF_REG(DMA0,CHn)  =    BYTEs;
    DMA_DOFF_REG(DMA0,CHn)  =    BYTEs;
    break; 
  case DMA_MEMORY_TO_PERIPHERAL:
    DMA_SOFF_REG(DMA0,CHn)  =    BYTEs;                              
    DMA_DOFF_REG(DMA0,CHn)  =    0x00u;                              
    break; 
  default:
    assert(0);
  }
  DMA_ATTR_REG(DMA0,CHn)  =    (0
                         | DMA_ATTR_SMOD(0x0)               // Դ��ַģ����ֹ  Source address modulo feature is disabled
                         | DMA_ATTR_SSIZE(byten)            // Դ����λ�� ��DMA_BYTEn  ��    SSIZE = 0 -> 8-bit ��SSIZE = 1 -> 16-bit ��SSIZE = 2 -> 32-bit ��SSIZE = 4 -> 16-byte
                         | DMA_ATTR_DMOD(0x0)               // Ŀ���ַģ����ֹ
                         | DMA_ATTR_DSIZE(byten)            // Ŀ������λ�� ��DMA_BYTEn  ��  ���òο�  SSIZE
                        );
  DMA_CITER_ELINKNO_REG(DMA0,CHn) = DMA_CITER_ELINKNO_CITER(count);     //��ǰ��ѭ������
  DMA_BITER_ELINKNO_REG(DMA0,CHn) = DMA_BITER_ELINKNO_BITER(count);     //��ʼ��ѭ������
  
  DMA_CR_REG(DMA0) &= ~DMA_CR_EMLM_MASK;                              //CR[EMLM] = 0 ,����Сѭ��ӳ��
  DMA_NBYTES_MLNO_REG(DMA0,CHn) = DMA_NBYTES_MLNO_NBYTES(BYTEs); //ÿ�δ����ֽ�����0Ϊ4GB
  
  DMA_SLAST_REG(DMA0,CHn) = 0;                                          //���� Դ��ַ�ĸ���ֵ,��ѭ��������ָ�Դ��ַ
  DMA_DLAST_SGA_REG(DMA0,CHn) = (uint32_t)((cfg & DADDR_KEEPON)==0?(-count):0);    //����Ŀ�ĵ�ַ�ĸ���ֵ,��ѭ��������ָ�Ŀ�ĵ�ַ���߱��ֵ�ַ
   //������ɺ����� 
  DMA_CSR_REG(DMA0,CHn)        =   (0
                             | DMA_CSR_BWC(0)               //�������,ÿ��һ�Σ�eDMA ����ֹͣ 8 �����ڣ�0��ֹͣ��1������2ֹͣ4���ڣ�3ֹͣ8���ڣ�
                             | DMA_CSR_DREQ_MASK            //��ѭ��������ֹͣӲ������
                             | DMA_CSR_INTMAJOR_MASK        //��ѭ������������ж�
                            );
  //����DMA����Դ
  DMAMUX_CHCFG_REG(DMAMUX,CHn) = (0
        | DMAMUX_CHCFG_ENBL_MASK                //ʹ��ͨ��
        //| DMAMUX_CHCFG_TRIG_MASK              //���ڴ���
        | DMAMUX_CHCFG_SOURCE(PTX(ptxn) + DMA_PORTA)              //ͨ������Դ            
                                  );
  DMA_DIS(CHn);                                                 //��ֹͨ��Ӳ��DMA����
  DMA_IRQ_CLEAN(CHn);
  
  //���ô���Դ��Ĭ���� �����ش�����
  port_init(ptxn, AsGpio|DMA_RISING);
  



    //�����ж�
    //DMA_EN(CHn);                                    //ʹ��ͨ��CHn Ӳ������
    //DMA_IRQ_EN(CHn); 
}
/******************************************************************************************************************
* DMATransDataStart
* ����  ��DMA���俪ʼ������һ��
* ����  ��CHn�� �˿ں�  address��Ŀ���ַ     Val��ѭ������
* ���  ����
* ע��  ����
**********************************************************************************************************************/                                  
__ramfunc void DMATransDataStart(uint8_t CHn,uint32_t address,uint32_t Val)
{
  DMA_DADDR_REG(DMA0,CHn) = address;                    //Ŀ���ַ
  DMA_CITER_ELINKNO_REG(DMA0,CHn) = DMA_CITER_ELINKNO_CITER(Val);     //��ǰ��ѭ������
  DMA_BITER_ELINKNO_REG(DMA0,CHn) = DMA_BITER_ELINKNO_BITER(Val);     //��ʼ��ѭ������
  //�����ж�
  DMA_EN(CHn);                                    //ʹ��ͨ��CHn Ӳ������
  //DMA_IRQ_EN(CHn); 
}
