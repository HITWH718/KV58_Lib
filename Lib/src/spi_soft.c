#include "spi_soft.h"
#include "port.h"

/*!
 *  @brief      模拟SPI延时
 *  @return     void
 *  Sample usage:       SPI_delay()    如果SPI通讯失败可以尝试增加n的值
 */
static void SPI_delay()
{
    uint8_t n = 2;

    while(n--)
    {
        asm("nop");asm("nop");asm("nop");asm("nop");
    }
}

/*!
 *  @brief      SPI使能
 *  @return     void
 *  Sample usage:       SPI_enable(SPI_ENABLE)
 */
inline void SPI_enable(uint8_t enable)
{
    NSS_SET(enable);
}

/*!
 *  @brief      SPI读取
 *  @return     uint8_t SPI读取的数据
 *  Sample usage:       uint8_t data = SPI_read()
 */
uint8_t SPI_read()
{
    uint8_t c=0;
    uint8_t i =0;

    while(i--)
    {
        c <<= 1;

        SCK_SET(0);
        SPI_delay();
        SCK_SET(1);      // CPHA=1，在时钟的第一个跳变沿采样
        if(MISO())   //逻辑1
        {
            c += 1;
        }
        SPI_delay();
    }
    return c;
}

/*!
 *  @brief      SPI写入
 *  @return     void
 *  Sample usage:       SPI_write(c)
 */
void SPI_write(uint8_t c)
{
    uint8_t i=8;

    while(i--)
    {
        SCK_SET(0);
        if(c & 0x80)
        {
            MOSI_SET(1);    //输出逻辑1
        }
        else
        {
            MOSI_SET(0);    //输出逻辑0
        }
        SPI_delay();
        SCK_SET(1);      // CPHA=1，在时钟的第一个跳变沿采样
        SPI_delay();
        c <<= 1;
    }
}

/*!
 *  @brief      SPI写入16位
 *  @return     void
 *  Sample usage:       SPI_write_16bit(c)
 */
void SPI_write_16bit(uint16_t c)
{
    uint8_t i=16;

    while(i--)
    {
        SCK_SET(0);
        if(c & 0x8000)
        {
            MOSI_SET(1);    //输出逻辑1
        }
        else
        {
            MOSI_SET(0);    //输出逻辑0
        }
        SPI_delay();
        SCK_SET(1);      // CPHA=1，在时钟的第一个跳变沿采样
        SPI_delay();
        c <<= 1;
    }
}

uint8_t SPI_read_reg(uint8_t dev_add, uint8_t reg)
{
    uint8_t data;

	SPI_enable(SPI_ENABLE);
    SPI_write((dev_add<<1) | 0x00);  //发送器件地址加写位
	SPI_write(reg);   				  //发送从机寄存器地址

	SPI_enable(SPI_ENABLE);                       //IIC重启（由发送变为接收需要重启IIC）
	SPI_write((dev_add<<1) | 0x01);    //发送器件地址加读位
	data = SPI_read();   				//发送需要写入的数据
	SPI_enable(0);

	return data;
}

void SPI_write_reg(uint8_t dev_add, uint8_t reg, uint8_t data)
{
    SPI_enable(SPI_ENABLE);
    SPI_write((dev_add<<1) | 0x00);  //发送器件地址加写位
	SPI_write(reg);   				 //发送从机寄存器地址
	SPI_write(data);   				 //发送需要写入的数据
	SPI_enable(0);
}

/*!
 *  @brief      SPI初始化
 *  @return     void
 *  Sample usage:       SPI_init()
 */
void SPI_init()
{
    gpio_init(SPI_MOSI, GPO, 0);
    gpio_init(SPI_MISO, GPI, 0);
    gpio_init(SPI_SCK, GPO, 0);
    gpio_init(SPI_NSS, GPO, 0);

//    port_init_NoALT(SPI_MOSI, OD | PULLUP);
//    port_init_NoALT(SPI_MISO, OD | PULLUP);
//    port_init_NoALT(SPI_SCK, OD | PULLUP);
//    port_init_NoALT(SPI_NSS, OD | PULLUP);
}
