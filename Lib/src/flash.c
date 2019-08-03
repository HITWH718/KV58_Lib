#include "flash.h"

#include "fsl_ftfx_cache.h"
#include "fsl_ftfx_flash.h"
#if FSL_FEATURE_FLASH_HAS_FLEX_NVM
#include "fsl_ftfx_flexnvm.h"
#endif

#include "fsl_common.h"
#include "fsl_ftfx_controller.h"
#include "fsl_ftfx_utilities.h"
#include "MKV58F24_features.h"

#include "assert.h"

static flash_config_t s_flashDriver;
/*! @brief Buffer for readback */

static uint32_t pflashBlockBase = 0;
static uint32_t pflashTotalSize = 0;
static uint32_t pflashSectorSize = 0;

void flash_property_get()
{
	
    FLASH_GetProperty(&s_flashDriver, kFLASH_PropertyPflash0BlockBaseAddr, &pflashBlockBase);
    FLASH_GetProperty(&s_flashDriver, kFLASH_PropertyPflash0TotalSize, &pflashTotalSize);
    FLASH_GetProperty(&s_flashDriver, kFLASH_PropertyPflash0SectorSize, &pflashSectorSize);
}

status_t  Flash_init()
{
	status_t result;
        ftfx_security_state_t securityStatus = kFTFx_SecurityStateNotSecure;
	memset(&s_flashDriver,0,sizeof(flash_config_t));
	result = FLASH_Init(&s_flashDriver);
	if(result!=kStatus_FLASH_Success)
		return result;
	flash_property_get();
        result = FLASH_GetSecurityState(&s_flashDriver, &securityStatus);
        if (kStatus_FLASH_Success != result)
        {
          return result;
        }
	return result;
}
/*!
 *  @brief      从flash读取数据
 *  @param      index    扇区号(0-127)
 *  @param      size           读取的字节数
 *  @param      data             需要读取的数据缓冲区首地址
 *  Sample usage:    uint32 data;       flash_read(1，8，&data);          读取(128-1)扇区，读取8字节
 */

/*********                      注意：1为127扇区 ！！！                   *****/

void flash_read(uint8_t index,size_t size,uint8_t* data)
{
    uint32_t destAdrss = pflashBlockBase + (pflashTotalSize - (index * pflashSectorSize));
    SCB_CleanInvalidateDCache();
    for (uint32_t i = 0; i < size; i++)
    {
       data[i] = *(uint8_t*)(destAdrss + i);
    }

}
/*!
 *  @brief      从flash写数据
 *  @param      index    扇区号(0-127)
 *  @param      size           写的字节数8的倍数
 *  @param      point             需要写入的数据缓冲区首地址
 *  Sample usage:    uint32 data;       flash_write(1，8，&data);          写入(128-1)扇区，写入8字节
 */

/*********                      注意：1为127扇区 ！！！                   *****/

status_t flash_write(uint8_t index,size_t size,uint8_t* point)
{
	status_t result;
        uint32_t  failAddr;
        uint32_t  failDat;
	uint32_t destAdrss = pflashBlockBase + (pflashTotalSize - (index * pflashSectorSize));

      assert(size%8==0);
//    for(uint32_t i=0;i<size;i++)
//    {
//      flash_read(index,1,offset+i,&temp);
//      if(temp!=0xFF)
//      {
//        result = FLASH_Erase(&s_flashDriver, destAdrss, pflashSectorSize, kFLASH_ApiEraseKey);
//        if (kStatus_FLASH_Success != result)
//        {
//                return result;
//        }
//        break;
//      }
//    }
    result = FLASH_Erase(&s_flashDriver, destAdrss, pflashSectorSize, kFLASH_ApiEraseKey);
        if (kStatus_FLASH_Success != result)
        {
                return result;
        }
    result = FLASH_VerifyErase(&s_flashDriver, destAdrss, pflashSectorSize, kFTFx_MarginValueUser);
   if (kStatus_FLASH_Success != result)
	{
		return result;
	}

   	
    result = FLASH_Program(&s_flashDriver, destAdrss, point,size);
    if (kStatus_FLASH_Success != result)
	{
		return result;
	}
    
      result = FLASH_VerifyProgram(&s_flashDriver, destAdrss, size, point, kFTFx_MarginValueUser,
                                     &failAddr, &failDat);
        if (kStatus_FLASH_Success != result)
        {
            return result;
        }

	return result;	
}