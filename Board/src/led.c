
#include "led.h"

void led_init(led_t led)
{
    if(led1==led)
    {
        gpio_init(PTA17,gpo,1);
    }
    else if(led2==led)
    {
        gpio_init(PTC0,gpo,1);
    }
    else if(led3==led)
    {
        gpio_init(PTD15,gpo,1);
    }
    else if(led4==led)
    {
        gpio_init(PTE29,gpo,1);
    }
    else if(led_all==led)
    {
        gpio_init(PTA17,gpo,1);
        gpio_init(PTC0,gpo,1);
        gpio_init(PTD15,gpo,1);
        gpio_init(PTE29,gpo,1);
    }
}

void led_set(led_t led, uint8_t output)
{
    if(led1 == led)
    {
        gpio_set(PTA17,output);
    }
    else if(led2 == led)
    {
        gpio_set(PTC0,output);
    }
    else if(led3 == led)
    {
        gpio_set(PTD15,output);
    }
    else if(led4 == led)
    {
        gpio_set(PTE29,output);
    }
    else if(led_all==led)
    {
        gpio_set(PTA17,output);
        gpio_set(PTC0,output);
        gpio_set(PTD15,output);
        gpio_set(PTE29,output);
    }
}
