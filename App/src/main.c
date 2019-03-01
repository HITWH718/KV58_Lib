
#include "headfile.h"

int main(void)
{
    pll_init();
    
    double a = 0;
    uint16_t b = 0;
    
    adc_init(ADC0_DP1);
    ftm_pwm_init(ftm0,ftm_ch0,15000);    
    ftm_pwm_duty(ftm0,ftm_ch0,500);
    
    while(1)
    {
        b = adc_once(ADC0_DP1,ADC_16bit);
        a = (uint16_t)(3.3*b/65536);
        printf("%lf",a);
        pit_delay_ms(pit0,200);
    }
}










