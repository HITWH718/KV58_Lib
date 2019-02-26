
#include "clock.h"
#include "uart.h"

uint32_t mcgout_clk_khz;
uint32_t system_clk_khz;
uint32_t fastperiphral_clk_khz;
uint32_t flexbus_clk_khz;
uint32_t bus_clk_khz;


void pll_init()
{
    /* Set HSRUN power mode */
    SMC->PMPROT = SMC_PMPROT_AVLLS_MASK | SMC_PMPROT_AVLP_MASK | SMC_PMPROT_AHSRUN_MASK;
    SMC->PMCTRL |= SMC_PMCTRL_RUNM(11);
    while (SMC->PMSTAT != (1<<7))
    {
    }
    /* Initializes OSC0 */
    osc_init();
    /* Set Clock Mode */
    mcg_init();
    /* Set the clock configuration in SIM module. */
    SIM->CLKDIV1 = 0x01390000;
    /* Set SystemCoreClock variable. */
    SystemCoreClock = 237500000U;
    
    mcgout_clk_khz = 237500U;
    system_clk_khz = 237500U;
    fastperiphral_clk_khz = 237500U/2;
    flexbus_clk_khz = 237500U/4;
    bus_clk_khz = 237500U/10;
    
    /* Debug Init */
    uart_init(DEBUG_UART,DEBUG_BAUD);
}

void osc_init()
{
    OSC0->CR &= ~(OSC_CR_SC16P_MASK | OSC_CR_SC8P_MASK | OSC_CR_SC4P_MASK | OSC_CR_SC2P_MASK);
    MCG->C2 |= (MCG_C2_EREFS(0) | MCG_C2_RANGE(2) | MCG_C2_HGO(1));
}

void mcg_init()
{
    /* Change to use external clock first. */
    MCG->C1 = MCG_C1_CLKS(2);
    /* Wait for CLKST clock status bits to show clock source is ext ref clk */
    while ((MCG->S & (MCG_S_CLKST_MASK)) != (MCG_S_CLKST(2)))
    {
    }
    /* Disable PLL first, then configure PLL. */
    MCG->C6 &= ~MCG_C6_PLLS_MASK;
    while (MCG->S & MCG_S_PLLST_MASK)
    {
    }
    /* Configure the PLL. */
    MCG->C5 |= MCG_C5_PRDIV0(0x3U);
    MCG->C6 |= MCG_C6_VDIV0(0x16U);
    /* Set enable mode. */
    MCG->C5 |= MCG_C5_PLLCLKEN0_MASK;
    /* Wait for PLL lock. */
    while (!(MCG->S & MCG_S_LOCK0_MASK))
    {
    }
    /* Change to PLL mode. */
    MCG->C6 |= MCG_C6_PLLS_MASK;
    /* Wait for PLL mode changed. */
    while (!(MCG->S & MCG_S_PLLST_MASK))
    {
    }
    /* Change to use PLL output clock. */
    MCG->C1 = (MCG->C1 & ~MCG_C1_CLKS_MASK) | MCG_C1_CLKS(0);
    while (((MCG->S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 3)
    {
    }
}

