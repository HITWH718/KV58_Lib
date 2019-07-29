#ifndef _SPI_H_
#define _SPI_H_

#include "MKV58F24.h"
#include "port.h"
#include "stdlib.h"

/**********************************  SPI   ***************************************/
//PCS接口，不用的时候需要注释，就不会进行初始化对应的管脚
//      模块通道    端口          可选范围                  建议

#define SPI0_SCK_PIN PTD1  // PTA15、PTC5、PTD1 、PTE17       全部都是 ALT2
#define SPI0_SOUT_PIN PTD2 // PTA16、PTC6、PTD2 、PTE18       全部都是 ALT2
#define SPI0_SIN_PIN PTA17 // PTA17、PTC7、PTD3 、PTE19       全部都是 ALT2

#define SPI0_PCS0_PIN PTD0  // PTA14、PTC4、PTD0、PTE16      全部都是 ALT2
#define SPI0_PCS1_PIN PTC3  // PTC3、PTD4               全部都是 ALT2
#define SPI0_PCS2_PIN PTD5  // PTC2、PTD5               全部都是 ALT2
#define SPI0_PCS3_PIN PTC1  // PTC1、PTD6               全部都是 ALT2
#define SPI0_PCS4_PIN PTC0  // PTC0、                   全部都是 ALT2
#define SPI0_PCS5_PIN PTB23 // PTB23                    ALT3

#define SPI1_SCK_PIN PTB11  // PTE2、PTB11、            全部都是 ALT2
#define SPI1_SOUT_PIN PTB16 // PTE1、PTB16、            全部都是 ALT2
#define SPI1_SIN_PIN PTB17  // PTE3、PTB17、            全部都是 ALT2

#define SPI1_PCS0_PIN PTB10 // PTE4、PTB10、            全部都是 ALT2
#define SPI1_PCS1_PIN PTE0  // PTE0、PTB9、             全部都是 ALT2
#define SPI1_PCS2_PIN PTE5  // PTE5、                   全部都是 ALT2
#define SPI1_PCS3_PIN PTE6  // PTE6、                   全部都是 ALT2

#define SPI2_SCK_PIN PTB21  // PTB21、PTD12            全部都是 ALT2
#define SPI2_SOUT_PIN PTB22 // PTB22、PTD13             全部都是 ALT2
#define SPI2_SIN_PIN PTB23  // PTB23、PTD14             全部都是 ALT2
#define SPI2_PCS0_PIN PTB20 // PTB20、PTD11             全部都是 ALT2
#define SPI2_PCS1_PIN PTD15 // PTD15                    全部都是 ALT2

/**
 *  @brief 主从机模式
 */
typedef enum
{
    MASTER, //主机模式
    SLAVE   //从机模式
} SPI_CFG;

/**
 *  @brief SPI模块号
 */
typedef enum
{
    SPI_0 = 0,
    SPI_1 = 1,
    SPI_2 = 2
} SPIn_e;

/**
 *  @brief SPI模块片选号
 */
typedef enum
{
    SPI_PCS0 = 1 << 0,
    SPI_PCS1 = 1 << 1,
    SPI_PCS2 = 1 << 2,
    SPI_PCS3 = 1 << 3,
    SPI_PCS4 = 1 << 4,
    SPI_PCS5 = 1 << 5,
} SPI_PCSn_e;

extern uint32_t spi_init(SPIn_e, SPI_PCSn_e, SPI_CFG, uint32_t baud); //SPI初始化，选择片选信号，设置模式，波特率
uint32_t spi_set_baud(SPIn_e, uint32_t baud);

//主机接收发送函数
extern void spi_mosi(SPIn_e, SPI_PCSn_e pcs, uint8_t *modata, uint8_t *midata, uint32_t len);                                                //SPI发送接收函数,发送databuff数据，并把接收到的数据存放在databuff里(注意，会覆盖原来的databuff)
extern void spi_mosi_cmd(SPIn_e, SPI_PCSn_e pcs, uint8_t *mocmd, uint8_t *micmd, uint8_t *modata, uint8_t *midata, uint32_t cmdlen, uint32_t len); //SPI发送接收函数,与spi_mosi相比，多了先发送cmd 缓冲区的步骤，即分开两部分发送

#endif
