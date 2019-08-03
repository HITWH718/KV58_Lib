#include "headfile.h"

uint8_t buff[8]={0};
uint64_t data=0x666;
int main(void)
{
    BOARD_BootClockRUN();
    gpio_init(PTA26,GPO,0);
    Flash_init();
    
    flash_write(1,8,&data);
    
    flash_read(1,8,&buff);
    if(buff[0]==44)
    {
      gpio_init(PTE9,GPO,0);
    }
    while(1){}
}

