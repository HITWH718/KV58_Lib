#include "Systick.h"
#include "headfile.h"

extern uint32_t SystemCoreClock;
void Systick_delay(uint32_t time)
{
  if(time==0)
  {
    return;
  }
  assert((time<=SysTick_RVR_RELOAD_MASK));
  SysTick_BASE_PTR->CSR=0x00;
  SysTick_BASE_PTR->RVR=time;
  SysTick_BASE_PTR->CVR=0x00;
  SysTick_BASE_PTR->CSR=(0
                         | SysTick_CSR_ENABLE_MASK
                         // SysTick_CSR_TICKINT_MASK
                         | SysTick_CSR_CLKSOURCE_MASK
                        );
  while(!((SysTick_BASE_PTR->CSR)& SysTick_CSR_COUNTFLAG_MASK));
}

void Systick_delay_ms(uint32_t ms)
{
  while(ms--)
  {
    Systick_delay(SystemCoreClock/1000);
  }
}