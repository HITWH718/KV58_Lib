
#include "uart.h"
#include "port_cfg.h"
#include "stdio.h"
#include "clock.h"

static UART_Type * const UARTn[] = UART_BASE_PTRS;

void uart_init(UARTn_e uartn, uint32_t baud)
{
    uint16_t sbr,brfa;
    switch(uartn)
    {
    case uart0:
        /* Enable uart clock */
        SIM->SCGC4 |= (SIM_SCGC4_UART0_MASK);

        if(PTA1==UART0_RX_PIN)
        {
            port_init(UART0_RX_PIN,Alt2);
        }
        else if((PTA15==UART0_RX_PIN) || (PTB16==UART0_RX_PIN) || (PTD6==UART0_RX_PIN))
        {
            port_init(UART0_RX_PIN,Alt3);
        }
        else if(PTE21==UART0_RX_PIN)
        {
            port_init(UART0_RX_PIN,Alt4);
        }
        else if(PTB0==UART0_RX_PIN)
        {
            port_init(UART0_RX_PIN,Alt7);
        }
        else if(PTC6==UART0_RX_PIN)
        {
            port_init(UART0_RX_PIN,Alt5);
        }

        if(PTA2==UART0_TX_PIN)
        {
            port_init(UART0_TX_PIN,Alt2);
        }
        if((PTA14==UART0_TX_PIN) || (PTB17==UART0_TX_PIN) || (PTD7==UART0_TX_PIN))
        {
            port_init(UART0_TX_PIN,Alt3);
        }
        if(PTE20==UART0_TX_PIN)
        {
            port_init(UART0_TX_PIN,Alt4);
        }
        if(PTC7==UART0_TX_PIN)
        {
            port_init(UART0_TX_PIN,Alt5);
        }
        if(PTB1==UART0_TX_PIN)
        {
            port_init(UART0_TX_PIN,Alt7);
        }
        break;

    case uart1:
        /* Enable uart clock */
        SIM->SCGC4 |= (SIM_SCGC4_UART1_MASK);

        port_init(UART1_RX_PIN,Alt3);
        port_init(UART1_TX_PIN,Alt3);
        break;

    case uart2:
        /* Enable uart clock */
        SIM->SCGC4 |= (SIM_SCGC4_UART2_MASK);

        port_init(UART2_RX_PIN,Alt3);
        port_init(UART2_TX_PIN,Alt3);
        break;

    case uart3:
        /* Enable uart clock */
        SIM->SCGC4 |= (SIM_SCGC4_UART3_MASK);

        port_init(UART3_RX_PIN,Alt3);
        port_init(UART3_TX_PIN,Alt3);
        break;

    case uart4:
        /* Enable uart clock */
        SIM->SCGC1 |= (SIM_SCGC1_UART4_MASK);

        if(PTC14 == UART4_RX_PIN)
        {
            port_init(UART4_RX_PIN,Alt9);
            port_init(UART4_TX_PIN,Alt9);
        }
        else if(PTE25 == UART4_RX_PIN)
        {
            port_init(UART4_RX_PIN,Alt8);
            port_init(UART4_TX_PIN,Alt8);
        }

        break;

    case uart5:
        /* Enable uart clock */
        SIM->SCGC1 |= (SIM_SCGC1_UART5_MASK);

        port_init(UART5_RX_PIN,Alt3);
        port_init(UART5_TX_PIN,Alt3);
        break;

    default:
        break;
    }

    /* Disable UART TX RX before setting. */
    UARTn[uartn]->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);

    /* Calculate the baud rate modulo divisor, sbr*/
    sbr = (uint16_t)(fastperiphral_clk_khz * 1000 / (baud * 16));
    if(sbr > 0x1FFF)sbr = 0x1FFF;   //sbr[12:0]

    /* Calcute brfa */
    brfa = 2 * ((2 * fastperiphral_clk_khz * 1000 / baud) - (sbr * 16));
    if(brfa > 0x1F)brfa = 0x1F; //brfa[4:0]

    /* Write the sbr value to the BDH and BDL registers*/
    UARTn[uartn]->BDH = (UARTn[uartn]->BDH & ~UART_BDH_SBR_MASK) | (uint8_t)(sbr >> 8);
    UARTn[uartn]->BDL = (uint8_t)sbr;

    /* Write the brfa value to the register*/
    UARTn[uartn]->C4 = (UARTn[uartn]->C4 & ~UART_C4_BRFA_MASK) | brfa;

    /* Set bit count/parity mode/idle/stop bits type. */
    UARTn[uartn]->C1 &= ~(UART_C1_M_MASK | UART_C1_PE_MASK | UART_C1_PT_MASK);
    UARTn[uartn]->C1 |= UART_C1_M(0);   //8 data bits,or UART_C1_M(1),9 data bits
    UARTn[uartn]->C1 |= UART_C1_PE(0) | UART_C1_PT(0);   //parity function disabled
    UARTn[uartn]->BDH &= ~(UART_BDH_SBNS_MASK);
    UARTn[uartn]->BDH |= UART_BDH_SBNS(0);  //one stop bit,or two when UART_BDH_SBNS(1)

    /* Enable tx/rx FIFO */
    UARTn[uartn]->PFIFO |= (UART_PFIFO_TXFE_MASK | UART_PFIFO_RXFE_MASK);

    /* Flush FIFO */
    UARTn[uartn]->CFIFO |= (UART_CFIFO_TXFLUSH_MASK | UART_CFIFO_RXFLUSH_MASK);

    /* Enable TX/RX  */
    UARTn[uartn]->C2 |= (UART_C2_TE_MASK | UART_C2_RE_MASK);
}

void uart_deinit(UARTn_e uartn)
{
    UARTn[uartn]->C2 &= ~(UART_C2_TE_MASK | UART_C2_RE_MASK);
    switch(uartn)
    {
    case uart0:
        SIM->SCGC4 &= ~(SIM_SCGC4_UART0_MASK);
        break;
    case uart1:
        SIM->SCGC4 &= ~(SIM_SCGC4_UART1_MASK);
        break;
    case uart2:
        SIM->SCGC4 &= ~(SIM_SCGC4_UART2_MASK);
        break;
    case uart3:
        SIM->SCGC4 &= ~(SIM_SCGC4_UART3_MASK);
        break;
    case uart4:
        SIM->SCGC4 &= ~(SIM_SCGC1_UART4_MASK);
        break;
    case uart5:
        SIM->SCGC4 &= ~(SIM_SCGC1_UART5_MASK);
        break;
    }
}

void uart_getbuff(UARTn_e uartn, uint8_t *buff, uint32_t len)
{
    while(len--)
    {
        while(!(UARTn[uartn]->S1 & UART_S1_RDRF_MASK))
        {
        }
        *buff++ = UARTn[uartn]->D;
    }
}

void uart_putchar(UARTn_e uartn, uint8_t ch)
{
    while(!(UARTn[uartn]->S1 & UART_S1_TDRE_MASK))
    {
    }
    UARTn[uartn]->D = ch;
}

void uart_putbuff(UARTn_e uartn, uint8_t *buff, uint32_t len)
{
    while(len--)
    {
        while(!(UARTn[uartn]->S1 & UART_S1_TDRE_MASK))
        {
        }
        UARTn[uartn]->D = *buff++;
    }
}

void uart_putstr(UARTn_e uartn, const uint8_t *str)
{
    while(*str)
    {
        uart_putchar(uartn,*str++);
    }
}

void uart_rx_irq_en(UARTn_e uartn)
{
    UARTn[uartn]->C2 |= UART_C2_RIE_MASK;
    NVIC_EnableIRQ((IRQn_Type)((uartn << 1) + UART0_RX_TX_IRQn));  //Multiplied By 2
}

void uart_tx_irq_en(UARTn_e uartn)
{
    UARTn[uartn]->C2 |= UART_C2_TIE_MASK;
    NVIC_EnableIRQ((IRQn_Type)((uartn << 1) + UART0_RX_TX_IRQn));  //Multiplied By 2
}

void UART0_RX_TX_IRQHandler(void)
{
    if(UARTn[0]->S1 & UART_S1_RDRF_MASK)
    {
        uart_putchar(uart0,(uint8_t)(UARTn[0]->D));
    }
    if(UARTn[0]->S1 & UART_S1_TDRE_MASK)
    {

    }
}

int fputc(int ch,FILE*stream)
{
    uart_putchar(DEBUG_UART,(char)ch);
    return ch;
}

