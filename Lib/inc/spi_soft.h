#ifndef _SPI_SOFTWARE_H_
#define _SPI_SOFTWARE_H_

#include "gpio.h"

//SPI引脚宏定义
#define SPI_MOSI    PTD5    //主机输出从机输入
#define SPI_MISO    PTE4    //主机输入从机输出
#define SPI_SCK     PTD4    //时钟
#define SPI_NSS     PTD8    //片选

#define MISO()      gpio_get(SPI_MISO)

#define MOSI_SET(x) gpio_set(SPI_MOSI, x)
#define MISO_SET(x) gpio_set(SPI_MISO, x)
#define SCK_SET(x)  gpio_set(SPI_SCK, x)
#define NSS_SET(x)  gpio_set(SPI_NSS, x)

#define SPI_ENABLE      0U  //低使能
#define SPI_DISABLE     1U


extern inline void SPI_enable(uint8_t enable);
extern uint8_t SPI_read();
extern void SPI_write(uint8_t c);
extern void SPI_init();
extern void SPI_write_16bit(uint16_t c);
extern uint8_t SPI_read_reg(uint8_t dev_add, uint8_t reg);
extern void SPI_write_reg(uint8_t dev_add, uint8_t reg, uint8_t data);

#endif



