#include "dma.h"


//static void dma_gpio_input_init(void *SADDR,uint8 BYTEs)
//{
//  uint8 n, tmp;
//  uint8 ptxn;
//  //SADDR ʵ���Ͼ��� GPIO�� ����Ĵ��� PDIR �ĵ�ַ
//  //GPIOA��GPIOB��GPIOC��GPIOD��GPIOE �ĵ�ַ �ֱ��� 0x400FF000u ��0x400FF040u ��0x400FF080u�� 0x400FF0C0u��0x400FF100u
//  //sizeof(GPIO_MemMap) = 0x18
//  //ÿ��GPIO��ַ &0x1C0 �󣬵õ� 0x000 , 0x040 , 0x080 ,0x0C0 ,0x100
//  //�� /0x40 ��õ� 0 �� 1 �� 2�� 3��4 ���պþ��� PTA��PTB��PTC ��PTD ��PTE
//  //�� *32 �͵��� PTA0��PTB0��PTC0 ��PTD0 ��PTE0
//  uint8 ptx0 = ((((uint32)SADDR) & 0x1C0) / 0x40 ) * 32;
//  //ÿ��GPIO ��Ӧ�ļĴ�����ַ�� &0x 3F ��õ���ֵ������ͬ�ġ�
//    //&GPIO_PDIR_REG(GPIOA) �� GPIOA �� ����Ĵ��� PDIR �� ��ַ
//    // (SADDR & 0x3f - &GPIO_PDIR_REG(GPIOA) & 0x3f) ��Ч�� (SADDR - PTA_B0_IN) & 0x3f
//    //������Ҫ�ɼ���λ Ϊ 0~7��8~15��16~23��24~31 ���� ����ʽ�Ӷ�Ӧ��ֵ Ϊ 0��1��2��3
//    //�պ���  0~7��8~15��16~23��24~31 λ�ĵ�ַƫ�ƣ��� * 8 �ͱ�� 0��8��16��24
//  n = (uint8)(((uint32)SADDR - ((uint32)(&GPIO_PDIR_REG(GPIOA)))) & 0x3f) * 8;       //��С�����ź�
//  
//    ptxn = ptx0 + n;
//    tmp = ptxn + (BYTEs * 8 ) - 1;                                          //�������ź�
//    while(ptxn <= tmp)
//    {
//        port_init((PTXn_e )ptxn , ALT1 | PULLDOWN );    //����ԴĬ������Ϊ������Ĭ�϶�ȡ������0
//        //������� GPIO ��ʼ��Ϊ����
//        gpio_init((PTXn_e )ptxn, GPI, 0);               //����Ϊ����
//        ptxn ++;
//    }
//}
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

                    port_init(PTA7,AsGpio | DMA_FALLING);             //Ĭ�ϴ���Դ�������أ��˴���Ϊ �½��ش���

                    DMA_EN(DMA_CH0);                //��Ҫʹ�� DMA ����ܴ�������
 */
void dma_portx2buff_init(DMA_CHn CHn, void *SADDR, void *DADDR, PTXn_e ptxn, DMA_BYTEn byten, uint32_t count, uint32_t cfg, DMA_TYPE type)
{
  uint8_t BYTEs = (byten == DMA_BYTE1 ? 1 : (byten == DMA_BYTE2 ? 2 : (byten == DMA_BYTE4 ? 4 : 16 ) ) ); //���㴫���ֽ���
  //ASSERT(count < 0x8000,"count too big");                                                                               //���ԣ����ֻ֧��0x7FFF
  
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
    ;
    //ASSERT(0,"DMA Type Error");
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
