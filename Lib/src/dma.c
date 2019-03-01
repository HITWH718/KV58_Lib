#include "dma.h"


//static void dma_gpio_input_init(void *SADDR,uint8 BYTEs)
//{
//  uint8 n, tmp;
//  uint8 ptxn;
//  //SADDR 实际上就是 GPIO的 输入寄存器 PDIR 的地址
//  //GPIOA、GPIOB、GPIOC、GPIOD、GPIOE 的地址 分别是 0x400FF000u 、0x400FF040u 、0x400FF080u、 0x400FF0C0u、0x400FF100u
//  //sizeof(GPIO_MemMap) = 0x18
//  //每个GPIO地址 &0x1C0 后，得到 0x000 , 0x040 , 0x080 ,0x0C0 ,0x100
//  //再 /0x40 后得到 0 、 1 、 2、 3、4 ，刚好就是 PTA、PTB、PTC 、PTD 、PTE
//  //再 *32 就等于 PTA0、PTB0、PTC0 、PTD0 、PTE0
//  uint8 ptx0 = ((((uint32)SADDR) & 0x1C0) / 0x40 ) * 32;
//  //每个GPIO 对应的寄存器地址， &0x 3F 后得到的值都是相同的。
//    //&GPIO_PDIR_REG(GPIOA) 即 GPIOA 的 输入寄存器 PDIR 的 地址
//    // (SADDR & 0x3f - &GPIO_PDIR_REG(GPIOA) & 0x3f) 等效于 (SADDR - PTA_B0_IN) & 0x3f
//    //假设需要采集的位 为 0~7、8~15、16~23、24~31 ，则 上面式子对应的值 为 0、1、2、3
//    //刚好是  0~7、8~15、16~23、24~31 位的地址偏移，再 * 8 就变成 0、8、16、24
//  n = (uint8)(((uint32)SADDR - ((uint32)(&GPIO_PDIR_REG(GPIOA)))) & 0x3f) * 8;       //最小的引脚号
//  
//    ptxn = ptx0 + n;
//    tmp = ptxn + (BYTEs * 8 ) - 1;                                          //最大的引脚号
//    while(ptxn <= tmp)
//    {
//        port_init((PTXn_e )ptxn , ALT1 | PULLDOWN );    //输入源默认配置为下拉，默认读取到的是0
//        //这里加入 GPIO 初始化为输入
//        gpio_init((PTXn_e )ptxn, GPI, 0);               //设置为输入
//        ptxn ++;
//    }
//}
/*!
 *  @brief      DMA初始化，由IO口请求传输输入端口的数据到内存
 *  @param      DMA_CHn         通道号（DMA_CH0 ~ DMA_CH15）
 *  @param      SADDR           源地址
 *  @param      DADDR           目的地址
 *  @param      PTxn            触发端口
 *  @param      DMA_BYTEn       每次DMA传输字节数
 *  @param      count           一个主循环传输字节数
 *  @param      cfg             DMA传输配置，从DMA_cfg里选择
 *  @note       DMA PTXn触发源默认上升沿触发传输，若需修改，则初始化后调用 port_init 配置DMA 触发方式
                初始化后，需要调用 DMA_EN 来实现
 *  Sample usage:   uint8 buff[10];
                    dma_portx2buff_init(DMA_CH0, &GPIO_PDIR_REG(GPIOD), buff, PTA7, DMA_BYTE1, 10 ,DADDR_RECOVER,DMA_PERIPHERAL_TO_MEMORY);
                    //DMA初始化，源地址：&GPIO_PDIR_REG(GPIOD)，目的地址：buff,PTA7触发(默认上升沿)，每次传输1字节，共传输 10次 ，传输结束后恢复地址

                    port_init(PTA7,AsGpio | DMA_FALLING);             //默认触发源是上升沿，此处改为 下降沿触发

                    DMA_EN(DMA_CH0);                //需要使能 DMA 后才能传输数据
 */
void dma_portx2buff_init(DMA_CHn CHn, void *SADDR, void *DADDR, PTXn_e ptxn, DMA_BYTEn byten, uint32_t count, uint32_t cfg, DMA_TYPE type)
{
  uint8_t BYTEs = (byten == DMA_BYTE1 ? 1 : (byten == DMA_BYTE2 ? 2 : (byten == DMA_BYTE4 ? 4 : 16 ) ) ); //计算传输字节数
  //ASSERT(count < 0x8000,"count too big");                                                                               //断言，最大只支持0x7FFF
  
  SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;                                                                    //打开DMA模块时钟
  SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
  //配置传输控制块TCD
  DMA_SADDR_REG(DMA0,CHn) = (uint32_t)SADDR;              //源地址
  DMA_DADDR_REG(DMA0,CHn) = (uint32_t)DADDR;              //目标地址
  //选择传输方向，一般为DMA_PERIPHERAL_TO_MEMORY
  switch(type)
  {
  case DMA_PERIPHERAL_TO_MEMORY:
    DMA_SOFF_REG(DMA0,CHn)  =    0x00u;                                //源地址偏移
    DMA_DOFF_REG(DMA0,CHn)  =    BYTEs;                                //目的地址偏移
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
                         | DMA_ATTR_SMOD(0x0)               // 源地址模数禁止  Source address modulo feature is disabled
                         | DMA_ATTR_SSIZE(byten)            // 源数据位宽 ：DMA_BYTEn  。    SSIZE = 0 -> 8-bit ，SSIZE = 1 -> 16-bit ，SSIZE = 2 -> 32-bit ，SSIZE = 4 -> 16-byte
                         | DMA_ATTR_DMOD(0x0)               // 目标地址模数禁止
                         | DMA_ATTR_DSIZE(byten)            // 目标数据位宽 ：DMA_BYTEn  。  设置参考  SSIZE
                        );
  DMA_CITER_ELINKNO_REG(DMA0,CHn) = DMA_CITER_ELINKNO_CITER(count);     //当前主循环次数
  DMA_BITER_ELINKNO_REG(DMA0,CHn) = DMA_BITER_ELINKNO_BITER(count);     //起始主循环次数
  
  DMA_CR_REG(DMA0) &= ~DMA_CR_EMLM_MASK;                              //CR[EMLM] = 0 ,禁用小循环映射
  DMA_NBYTES_MLNO_REG(DMA0,CHn) = DMA_NBYTES_MLNO_NBYTES(BYTEs); //每次传播字节数，0为4GB
  
  DMA_SLAST_REG(DMA0,CHn) = 0;                                          //调整 源地址的附加值,主循环结束后恢复源地址
  DMA_DLAST_SGA_REG(DMA0,CHn) = (uint32_t)((cfg & DADDR_KEEPON)==0?(-count):0);    //调整目的地址的附加值,主循环结束后恢复目的地址或者保持地址
   //传输完成后配置 
  DMA_CSR_REG(DMA0,CHn)        =   (0
                             | DMA_CSR_BWC(0)               //带宽控制,每读一次，eDMA 引擎停止 8 个周期（0不停止；1保留；2停止4周期；3停止8周期）
                             | DMA_CSR_DREQ_MASK            //主循环结束后停止硬件请求
                             | DMA_CSR_INTMAJOR_MASK        //主循环结束后产生中断
                            );
  //配置DMA触发源
  DMAMUX_CHCFG_REG(DMAMUX,CHn) = (0
        | DMAMUX_CHCFG_ENBL_MASK                //使能通道
        //| DMAMUX_CHCFG_TRIG_MASK              //周期触发
        | DMAMUX_CHCFG_SOURCE(PTX(ptxn) + DMA_PORTA)              //通道触发源            
                                  );
  DMA_DIS(CHn);                                                 //禁止通道硬件DMA请求
  DMA_IRQ_CLEAN(CHn);
  
  //配置触发源（默认是 上升沿触发）
  port_init(ptxn, AsGpio|DMA_RISING);
  



    //开启中断
    //DMA_EN(CHn);                                    //使能通道CHn 硬件请求
    //DMA_IRQ_EN(CHn); 
}
/******************************************************************************************************************
* DMATransDataStart
* 描述  ：DMA传输开始，传输一行
* 输入  ：CHn： 端口号  address：目标地址     Val：循环次数
* 输出  ：无
* 注意  ：无
**********************************************************************************************************************/                                  
__ramfunc void DMATransDataStart(uint8_t CHn,uint32_t address,uint32_t Val)
{
  DMA_DADDR_REG(DMA0,CHn) = address;                    //目标地址
  DMA_CITER_ELINKNO_REG(DMA0,CHn) = DMA_CITER_ELINKNO_CITER(Val);     //当前主循环次数
  DMA_BITER_ELINKNO_REG(DMA0,CHn) = DMA_BITER_ELINKNO_BITER(Val);     //起始主循环次数
  //开启中断
  DMA_EN(CHn);                                    //使能通道CHn 硬件请求
  //DMA_IRQ_EN(CHn); 
}
