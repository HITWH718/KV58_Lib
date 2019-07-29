#ifndef __KV58_FLASH_H__
#define __KV58_FLASH_H__

#include "MKV58F24.h"

//#define FMC_PFAPR             ((FMC)->PFAPR)
//#define FMC_PFB0CR            ((FMC)->PFB0CR)

#define FIFE_FSTAT            ((FTFE)->FSTAT)
#define FIFE_FCNFG            ((FTFE)->FCNFG)
#define FIFE_FSEC             ((FTFE)->FSEC)
#define FIFE_FOPT             ((FTFE)->FOPT)
//���ʵ�ַ�Ĵ���
#define FIFE_FCCOB3            ((FTFE)->FCCOB3)   //Flash address [7:0]
#define FIFE_FCCOB2            ((FTFE)->FCCOB2)   //Flash address [15:8]
#define FIFE_FCCOB1            ((FTFE)->FCCOB1)   //Flash address [23:16]
//����Ĵ���
#define FIFE_FCCOB0            ((FTFE)->FCCOB0)   //FCMD (a code that defines the FTFE command)
//���ݼĴ���
#define FIFE_FCCOB7            ((FTFE)->FCCOB7)   //Data Byte 3
#define FIFE_FCCOB6            ((FTFE)->FCCOB6)   //Data Byte 2
#define FIFE_FCCOB5            ((FTFE)->FCCOB5)   //Data Byte 1
#define FIFE_FCCOB4            ((FTFE)->FCCOB4)   //Data Byte 0
#define FIFE_FCCOBB            ((FTFE)->FCCOBB)   //Data Byte 7
#define FIFE_FCCOBA            ((FTFE)->FCCOBA)   //Data Byte 6
#define FIFE_FCCOB9            ((FTFE)->FCCOB9)   //Data Byte 5
#define FIFE_FCCOB8            ((FTFE)->FCCOB8)   //Data Byte 4

//flash����
#define FIFE_FPROT3            ((FTFE)->FPROT3)   //PROT[7:0]
#define FIFE_FPROT2            ((FTFE)->FPROT2)   //PROT[15:8]
#define FIFE_FPROT1            ((FTFE)->FPROT1)   //PROT[23:16]
#define FIFE_FPROT0            ((FTFE)->FPROT0)   //PROT[31:24]

#define CMD_ERASE_FLASH_SECTOR       0x09U              //����һ������
#define CMD_PROGRAM_PHRASE           0x07U               //д��8�ֽ�

#define FLASH_SECTOR_SIZE            (8*1024)              //������С
#define FLASH_ALIGN_ADDR             8                  //��ַ����������

#define FLASH_ADDR              0x10000000              //flash��ַ--��1007_FFFF

typedef uint64_t FlashWrite_Type;                         //д����С8���ֽ�

typedef union
{
    uint32_t  word;
    uint8_t   byte[4];
}Addr_Type;

extern void flash_init(void);                           //��ʼ��Flash
extern uint8_t flash_erase(uint16_t sector_num);            //����ָ��flash����
extern uint8_t flash_write(uint16_t sector_num,uint16_t offset ,FlashWrite_Type data);//д��8�ֽ����ݵ�flash����
extern uint8_t flash_write_buff(uint16_t sector_num,uint16_t offset,uint8_t *buff,uint16_t count);//д�����ݵ�flash����
extern void flash_read(uint16_t sector_num,uint16_t offset,uint8_t *buff,uint8_t count); ///��flash������ȡ����

#endif
