#ifndef _GPIO_H_
#define _GPIO_H_

#include "fsl_common.h"
#include "port.h"

typedef enum _gpio_direction
{
    gpi = 0U,  /*!< Set current pin as digital input*/
    gpo = 1U, /*!< Set current pin as digital output*/
} gpio_direction_t;

extern void gpio_init(PTXn_e ptxn, gpio_direction_t direction, uint8_t outputLogic);
extern void gpio_set(PTXn_e ptxn, uint8_t output);

#endif
