#ifndef __KV58_FLASH_H__
#define __KV58_FLASH_H__

#include "MKV58F24.h"

//#define FMC_PFAPR             ((FMC)->PFAPR)
//#define FMC_PFB0CR            ((FMC)->PFB0CR)

#define FIFE_FSTAT            ((FTFE)->FSTAT)
#define FIFE_FCNFG            ((FTFE)->FCNFG)
#define FIFE_FSEC             ((FTFE)->FSEC)
#define FIFE_FOPT             ((FTFE)->FOPT)
//访问地址寄存器
#define FIFE_FCCOB3            ((FTFE)->FCCOB3)   //Flash address [7:0]
#define FIFE_FCCOB2            ((FTFE)->FCCOB2)   //Flash address [15:8]
#define FIFE_FCCOB1            ((FTFE)->FCCOB1)   //Flash address [23:16]
//命令寄存器
#define FIFE_FCCOB0            ((FTFE)->FCCOB0)   //FCMD (a code that defines the FTFE command)
//数据寄存器
#define FIFE_FCCOB7            ((FTFE)->FCCOB7)   //Data Byte 3
#define FIFE_FCCOB6            ((FTFE)->FCCOB6)   //Data Byte 2
#define FIFE_FCCOB5            ((FTFE)->FCCOB5)   //Data Byte 1
#define FIFE_FCCOB4            ((FTFE)->FCCOB4)   //Data Byte 0
#define FIFE_FCCOBB            ((FTFE)->FCCOBB)   //Data Byte 7
#define FIFE_FCCOBA            ((FTFE)->FCCOBA)   //Data Byte 6
#define FIFE_FCCOB9            ((FTFE)->FCCOB9)   //Data Byte 5
#define FIFE_FCCOB8            ((FTFE)->FCCOB8)   //Data Byte 4

//flash保护
#define FIFE_FPROT3            ((FTFE)->FPROT3)   //PROT[7:0]
#define FIFE_FPROT2            ((FTFE)->FPROT2)   //PROT[15:8]
#define FIFE_FPROT1            ((FTFE)->FPROT1)   //PROT[23:16]
#define FIFE_FPROT0            ((FTFE)->FPROT0)   //PROT[31:24]

#define CMD_ERASE_FLASH_SECTOR       0x09U              //擦除一个扇区
#define CMD_PROGRAM_PHRASE           0x07U               //写入8字节

#define FLASH_SECTOR_SIZE            (8*1024)              //扇区大小
#define FLASH_ALIGN_ADDR             8                  //地址对齐整数倍

#define FLASH_ADDR              0x10000000              //flash地址--到1007_FFFF

typedef uint64_t FlashWrite_Type;                         //写入最小8个字节

typedef union
{
    uint32_t  word;
    uint8_t   byte[4];
}Addr_Type;

extern void flash_init(void);                           //初始化Flash
extern uint8_t flash_erase(uint16_t sector_num);            //擦除指定flash扇区
extern uint8_t flash_write(uint16_t sector_num,uint16_t offset ,FlashWrite_Type data);//写入8字节数据到flash扇区
extern uint8_t flash_write_buff(uint16_t sector_num,uint16_t offset,uint8_t *buff,uint16_t count);//写入数据到flash扇区
extern void flash_read(uint16_t sector_num,uint16_t offset,uint8_t *buff,uint8_t count); ///从flash扇区读取数据

#endif
