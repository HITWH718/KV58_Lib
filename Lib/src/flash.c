#include "flash.h"
#include <assert.h>


/*!
*  @brief      ���� Flash����
*  @param      ��
*/
uint8_t flash_cmd(void)
{
    FIFE_FSTAT = (0
                  | FTFE_FSTAT_FPVIOL_MASK      //�Ƿ����ʱ�־����1д��FPVIOL�����FPVIOLλ��
                      | FTFE_FSTAT_ACCERR_MASK      //���ʴ����־����1д��FPVIOL�����ACCERRλ��
                          | FTFE_FSTAT_RDCOLERR_MASK    //��ȡ��ͻ��־����1д��FPVIOL�����RDCOLERRλ��
                              | FTFE_FSTAT_CCIF_MASK        //ָ����ɱ�־,1Ϊ���
                                  );


    while(!((FIFE_FSTAT & FTFE_FSTAT_CCIF_MASK)>>FTFE_FSTAT_CCIF_SHIFT));       //�ȴ�ָ�����
    if (FIFE_FSTAT & (FTFE_FSTAT_FPVIOL_MASK | FTFE_FSTAT_ACCERR_MASK | FTFE_FSTAT_RDCOLERR_MASK | FTFE_FSTAT_MGSTAT0_MASK))
    {
        return 0;   //ָ��ִ�г���
    }
    else
    {
        return 1;   //ָ��ִ�гɹ�
    }
}
/*!
*  @brief      ��ʼ��Flash
*  @param      ��
*/
void flash_init(void)
{
    FMC_PFB0CR |= FMC_PFB0CR_S_INV_MASK;  //���Ԥ������
    while(!((FIFE_FSTAT & FTFE_FSTAT_CCIF_MASK)>>FTFE_FSTAT_CCIF_SHIFT));       //�ȴ�ָ�����

    FIFE_FSTAT = (0
                  | FTFE_FSTAT_FPVIOL_MASK      //�Ƿ����ʱ�־����1д��FPVIOL�����FPVIOLλ��
                      | FTFE_FSTAT_ACCERR_MASK      //���ʴ����־����1д��FPVIOL�����ACCERRλ��
                          | FTFE_FSTAT_RDCOLERR_MASK    //��ȡ��ͻ��־����1д��FPVIOL�����RDCOLERRλ��
                              //                  | FTFE_FSTAT_CCIF_MASK        //ָ����ɱ�־,1Ϊ���
                              );
    while(!((FIFE_FSTAT & FTFE_FSTAT_CCIF_MASK)>>FTFE_FSTAT_CCIF_SHIFT));
}
/*!
*  @brief      ����ָ��flash����
*  @param      sector_num    ������(0-63)
*  @return     ִ�н��(1�ɹ���0ʧ��)
*  Sample usage:    flash_erase(60);        //��������60
*/
uint8_t flash_erase(uint16_t sector_num)
{
    uint32_t addr = sector_num*FLASH_SECTOR_SIZE;
    FIFE_FCCOB0 = CMD_ERASE_FLASH_SECTOR;         //д���������
    //д���ַ
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
*  @brief      ��flash��ȡ����
*  @param      sector_num    ������(0-63)
*  @param      offset          ��ȡ�������ڲ�ƫ�Ƶ�ַ
*  @param      buff             ��Ҫ��ȡ�����ݻ������׵�ַ
*  @param      count           ��ȡ���ֽ���8�ı���
*  Sample usage:    uint32_t data;       flash_read(60��2��&data,8);��ȡ60������ƫ��2���ֽڣ���ȡ8�ֽ�
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
*  @brief      д��8�ֽ����ݵ�flash����������д��⣬�Զ�����������
*  @param      sector_num    ������(0-63)
*  @param      offset          д�������ڲ�ƫ�Ƶ�ַ��0~8*1024�� 8�ı�����
*  @return     ִ�н��(1�ɹ���0ʧ��)
*  Sample usage:    flash_write(60,16,0x0FFF);        //д0x0FFF������60
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

    //д���ַ
    FIFE_FCCOB1 = ((Addr_Type *)&addr)->byte[2];
    FIFE_FCCOB2 = ((Addr_Type *)&addr)->byte[1];
    FIFE_FCCOB3 = ((Addr_Type *)&addr)->byte[0];

    tempdata = (uint32_t)data;

    FIFE_FCCOB4 = ((Addr_Type *)&tempdata)->byte[3];
    FIFE_FCCOB5 = ((Addr_Type *)&tempdata)->byte[2];
    FIFE_FCCOB6 = ((Addr_Type *)&tempdata)->byte[1];
    FIFE_FCCOB7 = ((Addr_Type *)&tempdata)->byte[0];

    FIFE_FCCOB0 = CMD_PROGRAM_PHRASE;         //д������

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
*  @brief      д���������ݵ�flash���������û�����Ӧ��û����=��=��
*  @param      sector_num    ������(0-63)
*  @param      offset          д�������ڲ�ƫ�Ƶ�ַ��0~8*1024�� 8�ı�����
*  @param      buff             ��Ҫд������ݻ������׵�ַ(���ݴ�СΪ8�ֽ�������)
*  @return     ִ�н��(1�ɹ���0ʧ��)
*  Sample usage:    flash_write_buff(60��8��buff,sizeof(buff));
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

    FIFE_FCCOB0 = CMD_PROGRAM_PHRASE;     //д������

    for(i=0; i < count; i += FLASH_ALIGN_ADDR)
    {
        //д���ַ
        FIFE_FCCOB1 = ((Addr_Type *)&addr)->byte[2];
        FIFE_FCCOB2 = ((Addr_Type *)&addr)->byte[1];
        FIFE_FCCOB3 = ((Addr_Type *)&addr)->byte[0];

        //д���32λ
        data = *((uint32_t*)buff);

        FIFE_FCCOB4 = ((Addr_Type *)&data)->byte[3];
        FIFE_FCCOB5 = ((Addr_Type *)&data)->byte[2];
        FIFE_FCCOB6 = ((Addr_Type *)&data)->byte[1];
        FIFE_FCCOB7 = ((Addr_Type *)&data)->byte[0];
        //д���32λ
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

