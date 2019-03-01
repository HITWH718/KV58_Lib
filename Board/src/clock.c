
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
    SMC->PMCTRL |= SMC_PMCTRL_RUNM(3);
    while (SMC->PMSTAT != (1<<7))
    {
    }
    /* Set the system clock dividers in SIM to safe value. */
    SIM->CLKDIV1 = 0x01170000U;
    /* Initializes OSC0 */
    osc0_init();
    /* Set Clock Mode */
    mcg_init();
    /* Set the clock configuration in SIM module. */
    SIM->CLKDIV1 = 0x01390000U;
    SIM->SOPT2 = ((SIM->SOPT2 & ~SIM_SOPT2_PLLFLLSEL_MASK) | SIM_SOPT2_PLLFLLSEL(1));
    SIM->SOPT1 = ((SIM->SOPT1 & ~SIM_SOPT1_OSC32KSEL_MASK) | SIM_SOPT1_OSC32KSEL(3));
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

void osc0_init()
{
    OSC0->CR &= ~(OSC_CR_SC16P_MASK | OSC_CR_SC8P_MASK | OSC_CR_SC4P_MASK | OSC_CR_SC2P_MASK);
    OSC0->CR &= ~(OSC_CR_ERCLKEN_MASK | OSC_CR_EREFSTEN_MASK);
    OSC0->CR |= OSC_CR_ERCLKEN_MASK;
    
    MCG->C2 &= ~(MCG_C2_EREFS_MASK | MCG_C2_HGO_MASK | MCG_C2_RANGE_MASK);
    MCG->C2 |= (MCG_C2_EREFS(0) | MCG_C2_RANGE(2) | MCG_C2_HGO(0));
}

void mcg_init()
{
    /* Set the value of FRDIV */
    MCG->C1 &= ~MCG_C1_FRDIV_MASK;
    MCG->C1 |= MCG_C1_FRDIV(0);

    
    /* Set MCG to PEE mode. */
    /* Set MCG to PBE mode. */
    MCG->C2 &= ~MCG_C2_LP_MASK; /* Disable lowpower. */
    /* Change to use external clock first. */
    MCG->C1 = ((MCG->C1 & ~(MCG_C1_CLKS_MASK | MCG_C1_IREFS_MASK)) | MCG_C1_CLKS(2));
    /* Wait for CLKST clock status bits to show clock source is ext ref clk */
    while ((MCG->S & (MCG_S_IREFST_MASK | MCG_S_CLKST_MASK)) != (MCG_S_IREFST(0) | MCG_S_CLKST(2)))
    {
    }
    /* Disable PLL first, then configure PLL. */
    MCG->C6 &= ~MCG_C6_PLLS_MASK;
    while (MCG->S & MCG_S_PLLST_MASK)
    {
    }
    /* Configure the PLL. */
    MCG->C5 = MCG_C5_PRDIV0(3); /* Disable the PLL first. */
    MCG->C6 = (MCG->C6 & ~MCG_C6_VDIV0_MASK) | MCG_C6_VDIV0(0x16U);
    /* Set enable mode. */
    MCG->C5 |= ((uint32_t)MCG_C5_PLLCLKEN0_MASK | 0U);
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
    while (((MCG->S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 3U)
    {
    }
    
    /* Configure the Internal Reference clock (MCGIRCLK). */
    /* If need to update the FCRDIV. */
    if (0x0U != ((MCG->SC & MCG_SC_FCRDIV_MASK) >> MCG_SC_FCRDIV_SHIFT))
    {
        /* If fast IRC is in use currently, change to slow IRC. */
        if ((0x1U == ((MCG->S & MCG_S_IRCST_MASK) >> MCG_S_IRCST_SHIFT)) && ((((MCG->S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x1U) || (MCG->C1 & MCG_C1_IRCLKEN_MASK)))
        {
            MCG->C2 = ((MCG->C2 & ~MCG_C2_IRCS_MASK) | (MCG_C2_IRCS(0)));
            while (((MCG->S & MCG_S_IRCST_MASK) >> MCG_S_IRCST_SHIFT) != 0)
            {
            }
        }
        /* Update FCRDIV. */
        MCG->SC = (MCG->SC & ~(MCG_SC_FCRDIV_MASK | MCG_SC_ATMF_MASK | MCG_SC_LOCS0_MASK)) | MCG_SC_FCRDIV(0x0U);
    }
    /* Set internal reference clock selection. */
    MCG->C2 = (MCG->C2 & ~MCG_C2_IRCS_MASK) | (MCG_C2_IRCS(0));
    MCG->C1 = (MCG->C1 & ~(MCG_C1_IRCLKEN_MASK | MCG_C1_IREFSTEN_MASK)) | MCG_C1_IRCLKEN_MASK;
    /* If MCGIRCLK is used, need to wait for MCG_S_IRCST. */
    if (((MCG->S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x1U)
    {
        while (((MCG->S & MCG_S_IRCST_MASK) >> MCG_S_IRCST_SHIFT) != 0)
        {
        }
    }
}

