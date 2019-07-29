#include "flash.h"
#include <assert.h>


/*!
*  @brief      启动 Flash命令
*  @param      无
*/
uint8_t flash_cmd(void)
{
    FIFE_FSTAT = (0
                  | FTFE_FSTAT_FPVIOL_MASK      //非法访问标志，将1写入FPVIOL来清除FPVIOL位。
                      | FTFE_FSTAT_ACCERR_MASK      //访问错误标志，将1写入FPVIOL来清除ACCERR位。
                          | FTFE_FSTAT_RDCOLERR_MASK    //读取冲突标志，将1写入FPVIOL来清除RDCOLERR位。
                              | FTFE_FSTAT_CCIF_MASK        //指令完成标志,1为完成
                                  );


    while(!((FIFE_FSTAT & FTFE_FSTAT_CCIF_MASK)>>FTFE_FSTAT_CCIF_SHIFT));       //等待指令完成
    if (FIFE_FSTAT & (FTFE_FSTAT_FPVIOL_MASK | FTFE_FSTAT_ACCERR_MASK | FTFE_FSTAT_RDCOLERR_MASK | FTFE_FSTAT_MGSTAT0_MASK))
    {
        return 0;   //指令执行出错
    }
    else
    {
        return 1;   //指令执行成功
    }
}
/*!
*  @brief      初始化Flash
*  @param      无
*/
void flash_init(void)
{
    FMC_PFB0CR |= FMC_PFB0CR_S_INV_MASK;  //清除预读缓存
    while(!((FIFE_FSTAT & FTFE_FSTAT_CCIF_MASK)>>FTFE_FSTAT_CCIF_SHIFT));       //等待指令完成

    FIFE_FSTAT = (0
                  | FTFE_FSTAT_FPVIOL_MASK      //非法访问标志，将1写入FPVIOL来清除FPVIOL位。
                      | FTFE_FSTAT_ACCERR_MASK      //访问错误标志，将1写入FPVIOL来清除ACCERR位。
                          | FTFE_FSTAT_RDCOLERR_MASK    //读取冲突标志，将1写入FPVIOL来清除RDCOLERR位。
                              //                  | FTFE_FSTAT_CCIF_MASK        //指令完成标志,1为完成
                              );
    while(!((FIFE_FSTAT & FTFE_FSTAT_CCIF_MASK)>>FTFE_FSTAT_CCIF_SHIFT));
}
/*!
*  @brief      擦除指定flash扇区
*  @param      sector_num    扇区号(0-63)
*  @return     执行结果(1成功，0失败)
*  Sample usage:    flash_erase(60);        //擦除扇区60
*/
uint8_t flash_erase(uint16_t sector_num)
{
    uint32_t addr = sector_num*FLASH_SECTOR_SIZE;
    FIFE_FCCOB0 = CMD_ERASE_FLASH_SECTOR;         //写入擦除命令
    //写入地址
    FIFE_FCCOB1 = ((Addr_Type *)&addr)->byte[2];
    FIFE_FCCOB2 = ((Addr_Type *)&addr)->byte[1];
    FIFE_FCCOB3 = ((Addr_Type *)&addr)->byte[0];

    if(flash_cmd()==0)
    {
        return 0;
    }
    ///////
    return 1;
}
/*!
*  @brief      从flash读取数据
*  @param      sector_num    扇区号(0-63)
*  @param      offset          读取的扇区内部偏移地址
*  @param      buff             需要读取的数据缓冲区首地址
*  @param      count           读取的字节数8的倍数
*  Sample usage:    uint32_t data;       flash_read(60，2，&data,8);读取60扇区，偏移2个字节，读取8字节
*/
void flash_read(uint16_t sector_num,uint16_t offset,uint8_t *buff,uint8_t count)
{
    uint32_t addr = sector_num*FLASH_SECTOR_SIZE+offset+FLASH_ADDR;
    assert(offset % FLASH_ALIGN_ADDR == 0);
    assert(offset <= FLASH_SECTOR_SIZE);
    while(count)
    {
        *(uint64_t *)buff = *(uint64_t *)addr;
        buff += 8;
        addr += 8;
        count -= 8;
    }
}
/*!
*  @brief      写入8字节数据到flash扇区（内置写检测，自动擦出扇区）
*  @param      sector_num    扇区号(0-63)
*  @param      offset          写入扇区内部偏移地址（0~8*1024中 8的倍数）
*  @return     执行结果(1成功，0失败)
*  Sample usage:    flash_write(60,16,0x0FFF);        //写0x0FFF到扇区60
*/
uint8_t flash_write(uint16_t sector_num,uint16_t offset ,FlashWrite_Type data)
{
    uint32_t addr = sector_num*FLASH_SECTOR_SIZE+offset;
    uint32_t tempaddr = FLASH_ADDR+sector_num*FLASH_SECTOR_SIZE+offset;
    uint32_t tempdata;

    assert(offset % FLASH_ALIGN_ADDR == 0);
    assert(offset <= FLASH_SECTOR_SIZE);


    if(!(*(uint64_t *)tempaddr==0xFFFFFFFFFFFFFFFF))
    {
        flash_erase(sector_num);
    }

    //写入地址
    FIFE_FCCOB1 = ((Addr_Type *)&addr)->byte[2];
    FIFE_FCCOB2 = ((Addr_Type *)&addr)->byte[1];
    FIFE_FCCOB3 = ((Addr_Type *)&addr)->byte[0];

    tempdata = (uint32_t)data;

    FIFE_FCCOB4 = ((Addr_Type *)&tempdata)->byte[3];
    FIFE_FCCOB5 = ((Addr_Type *)&tempdata)->byte[2];
    FIFE_FCCOB6 = ((Addr_Type *)&tempdata)->byte[1];
    FIFE_FCCOB7 = ((Addr_Type *)&tempdata)->byte[0];

    FIFE_FCCOB0 = CMD_PROGRAM_PHRASE;         //写入命令

    tempdata = (uint32_t)(data>>32);

    FIFE_FCCOB8 = ((Addr_Type *)&tempdata)->byte[3];
    FIFE_FCCOB9 = ((Addr_Type *)&tempdata)->byte[2];
    FIFE_FCCOBA = ((Addr_Type *)&tempdata)->byte[1];
    FIFE_FCCOBB = ((Addr_Type *)&tempdata)->byte[0];

    if(flash_cmd()==0)
    {
        return 0;
    }

    return 1;
}
/*!
*  @brief      写入数组数据到flash扇区（这个没测过，应该没问题=，=）
*  @param      sector_num    扇区号(0-63)
*  @param      offset          写入扇区内部偏移地址（0~8*1024中 8的倍数）
*  @param      buff             需要写入的数据缓冲区首地址(数据大小为8字节整数倍)
*  @return     执行结果(1成功，0失败)
*  Sample usage:    flash_write_buff(60，8，buff,sizeof(buff));
*/
uint8_t flash_write_buff(uint16_t sector_num,uint16_t offset,uint8_t *buff,uint16_t count)
{
    uint32_t addr = sector_num*FLASH_SECTOR_SIZE+offset;
    uint32_t tempaddr = FLASH_ADDR+sector_num*FLASH_SECTOR_SIZE+offset;
    uint16_t tempcount = count;
    uint32_t data;
    uint32_t i;
    assert(offset % FLASH_ALIGN_ADDR == 0);
    assert(offset <= FLASH_SECTOR_SIZE);

    while(tempcount)
    {
        if(!(*(uint64_t *)tempaddr==0xFFFFFFFFFFFFFFFF))
        {
            flash_erase(sector_num);
            break;
        }
        tempcount=tempcount-8;
    }

    FIFE_FCCOB0 = CMD_PROGRAM_PHRASE;     //写入命令

    for(i=0; i < count; i += FLASH_ALIGN_ADDR)
    {
        //写入地址
        FIFE_FCCOB1 = ((Addr_Type *)&addr)->byte[2];
        FIFE_FCCOB2 = ((Addr_Type *)&addr)->byte[1];
        FIFE_FCCOB3 = ((Addr_Type *)&addr)->byte[0];

        //写入低32位
        data = *((uint32_t*)buff);

        FIFE_FCCOB4 = ((Addr_Type *)&data)->byte[3];
        FIFE_FCCOB5 = ((Addr_Type *)&data)->byte[2];
        FIFE_FCCOB6 = ((Addr_Type *)&data)->byte[1];
        FIFE_FCCOB7 = ((Addr_Type *)&data)->byte[0];
        //写入高32位
        data = *((uint32_t*)(buff+4));

        FIFE_FCCOB8 = ((Addr_Type *)&data)->byte[3];
        FIFE_FCCOB9 = ((Addr_Type *)&data)->byte[2];
        FIFE_FCCOBA = ((Addr_Type *)&data)->byte[1];
        FIFE_FCCOBB = ((Addr_Type *)&data)->byte[0];

        if(flash_cmd()==0)
        {
            return 0;
        }
        addr += FLASH_ALIGN_ADDR;
        buff += FLASH_ALIGN_ADDR;
    }
    return 1;
}

