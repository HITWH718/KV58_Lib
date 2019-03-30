#include "dma.h"
#include "MKV58F24.h"
//#include "common.h"
#include "port.h"







//dma内没有初始化触发引脚，需要单独初始化，源地址使用GPIO_BYT0_PDIR(gpio)  或GPIO_BYT1_PDIR(gpio)  等



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

                    port_init(PTA7,ALT1 | DMA_FALLING);             //默认触发源是上升沿，此处改为 下降沿触发

                    DMA_EN(DMA_CH0);                //需要使能 DMA 后才能传输数据
 */
void dma_portx2buff_init(DMA_CHn CHn, void *SADDR, void *DADDR, PTXn_e ptxn, DMA_BYTEn byten, uint32_t count, uint32_t cfg, DMA_TYPE type)
{
  uint8_t BYTEs = (byten == DMA_BYTE1 ? 1 : (byten == DMA_BYTE2 ? 2 : (byten == DMA_BYTE4 ? 4 : 16 ) ) ); //计算传输字节数
  assert(count < 0x8000);                                                                               //断言，最大只支持0x7FFF
  
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
    assert(0);
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
