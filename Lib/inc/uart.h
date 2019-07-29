#ifndef _UART_H_
#define _UART_H_

#include "MKV58F24.h"

typedef enum _UARTn
{
    uart0,
    uart1,
    uart2,
    uart3,
    uart4,
    uart5,
}UARTn_e;

#define DEBUG_UART  uart1
#define DEBUG_BAUD  115200

extern void uart_init(UARTn_e uartn,uint32_t boud);
extern void uart_deinit(UARTn_e uartn);
extern uint8_t uart_getchar(UARTn_e uartn);
extern void uart_getbuff(UARTn_e uartn, uint8_t *data, uint32_t len);
extern void uart_putchar(UARTn_e uartn, uint8_t ch);
extern void uart_putbuff(UARTn_e uartn, uint8_t *buff, uint32_t len);
extern void uart_putstr(UARTn_e uartn, const uint8_t *str);
extern void uart_rx_irq_en(UARTn_e uartn);
extern void uart_tx_irq_en(UARTn_e uartn);
extern void uart_rx_irq_dis(UARTn_e uartn);
extern void uart_tx_irq_dis(UARTn_e uartn);

#endif
