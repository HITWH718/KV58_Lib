
#include "ftm.h"
#include "port_cfg.h"

static FTM_Type * const FTMn[] = FTM_BASE_PTRS;
static uint16_t period[4];

void ftm_pwm_init(FTMn_e ftmn, FTM_CHn_e ch, uint32_t freq)
{
    uint16_t mod;
    uint32_t freq_ftm;
    switch(ftmn)
    {
    case ftm0:
        /* Enable uart clock */
        SIM->SCGC6 |= SIM_SCGC6_FTM0_MASK;
        switch(ch)
        {
        case ftm_ch0:
            if(PTA3==FTM0_CH0_PIN || PTE24==FTM0_CH0_PIN)
            {
                port_init(FTM0_CH0_PIN, Alt3);
            }
            else if(PTC1==FTM0_CH0_PIN)
            {
                port_init(FTM0_CH0_PIN,Alt4);
            }
            else if(PTD0==FTM0_CH0_PIN)
            {
                port_init(FTM0_CH0_PIN,Alt5);
            }
            break;
            
        case ftm_ch1:
            if(PTA4==FTM0_CH1_PIN || PTE25==FTM0_CH1_PIN)
            {
                port_init(FTM0_CH1_PIN,Alt3);
            }
            else if(PTC2==FTM0_CH1_PIN)
            {
                port_init(FTM0_CH1_PIN,Alt4);
            }
            else if(PTD1==FTM0_CH1_PIN)
            {
                port_init(FTM0_CH1_PIN,Alt5);
            }
            break;
            
        case ftm_ch2:
            if(PTA5==FTM0_CH2_PIN || PTE29==FTM0_CH2_PIN)
            {
                port_init(FTM0_CH2_PIN,Alt3);
            }
            else if(PTC3==FTM0_CH2_PIN)
            {
                port_init(FTM0_CH2_PIN,Alt4);
            }
            else if(PTC5==FTM0_CH2_PIN || PTD2==FTM0_CH2_PIN)
            {
                port_init(FTM0_CH2_PIN,Alt5);
            }
            break;
            
        case ftm_ch3:
            if(PTA6==FTM0_CH3_PIN || PTE30==FTM0_CH3_PIN)
            {
                port_init(FTM0_CH3_PIN,Alt3);
            }
            else if(PTC4==FTM0_CH3_PIN)
            {
                port_init(FTM0_CH3_PIN,Alt4);
            }
            else if(PTD3==FTM0_CH3_PIN)
            {
                port_init(FTM0_CH3_PIN,Alt5);
            }
            break;
            
        case ftm_ch4:
            if(PTA7==FTM0_CH4_PIN || PTE26==FTM0_CH4_PIN)
            {
                port_init(FTM0_CH4_PIN,Alt3);
            }
            else if(PTD4==FTM0_CH4_PIN)
            {
                port_init(FTM0_CH4_PIN,Alt4);
            }
            break;
            
        case ftm_ch5:
            if(PTA0==FTM0_CH5_PIN)
            {
                port_init(FTM0_CH5_PIN,Alt3);
            }
            else if(PTD5==FTM0_CH5_PIN)
            {
                port_init(FTM0_CH5_PIN,Alt4);
            }
            break;

        case ftm_ch6:
            if(PTA1==FTM0_CH6_PIN)
            {
                port_init(FTM0_CH6_PIN,Alt3);
            }
            else if(PTD6==FTM0_CH6_PIN)
            {
                port_init(FTM0_CH6_PIN,Alt4);
            }
            break;
            
        case ftm_ch7:
            if(PTA2==FTM0_CH7_PIN)
            {
                port_init(FTM0_CH7_PIN,Alt3);
            }
            else if(PTD7==FTM0_CH7_PIN)
            {
                port_init(FTM0_CH7_PIN,Alt4);
            }
            break;
        }
        break;
        
    case ftm1:
        /* Enable uart clock */
        SIM->SCGC6 |= SIM_SCGC6_FTM1_MASK;
        switch(ch)
        {
        case ftm_ch0:
            if(PTA8==FTM1_CH0_PIN || PTA12==FTM1_CH0_PIN || PTB0==FTM1_CH0_PIN || PTE20==FTM1_CH0_PIN)
            {
                port_init(FTM1_CH0_PIN,Alt3);
            }
            else if(PTD6==FTM1_CH0_PIN)
            {
                port_init(FTM1_CH0_PIN,Alt5);
            }
            else if(PTA2==FTM1_CH0_PIN)
            {
                port_init(FTM1_CH0_PIN,Alt6);
            }
            break;
            
        case ftm_ch1:
            if(PTA9==FTM1_CH1_PIN || PTA13==FTM1_CH1_PIN || PTB1==FTM1_CH1_PIN || PTE21==FTM1_CH1_PIN)
            {
                port_init(FTM1_CH1_PIN,Alt3);
            }
            else if(PTD6==FTM1_CH1_PIN)
            {
                port_init(FTM1_CH1_PIN,Alt5);
            }
            else if(PTA1==FTM1_CH1_PIN)
            {
                port_init(FTM1_CH1_PIN,Alt6);
            }
            break;
        }
        break;
        
    case ftm2:
        /* Enable uart clock */
        SIM->SCGC6 |= SIM_SCGC6_FTM2_MASK;
        switch(ch)
        {
        case ftm_ch0:
            port_init(FTM2_CH0_PIN,Alt3);
            break;
            
        case ftm_ch1:
            port_init(FTM2_CH1_PIN,Alt3);
            break;
        
        default:
            break;
        }
        break;
    
    default:
        break;
    }
    
    /* Setup the channel output behaviour when a match occurs with the compare value */
    FTMn[ftmn]->CONTROLS[ch].CnSC &= FTM_CnSC_ELSA_MASK;
    FTMn[ftmn]->CONTROLS[ch].CnSC = FTM_CnSC_MSB_MASK | FTM_CnSC_ELSB_MASK;
    /* Set the clock prescale factor */
    FTMn[ftmn]->SC |= FTM_SC_PS(7);
    freq_ftm = 237500000/2/2^7;
    /* Select the clock source */
    FTMn[ftmn]->SC |= FTM_SC_CLKS(1);
    /* Calculate the MOD */
    mod = freq_ftm/freq;
    FTMn[ftmn]->MOD = mod;
    period[ftmn] = mod;
    /* Set the counter */
    FTMn[ftmn]->CNTIN = 0;
    FTMn[ftmn]->CONTROLS[ch].CnV = 0;
    FTMn[ftmn]->CNT = 0;

}

void ftm_pwm_duty(FTMn_e ftmn, FTM_CHn_e ch, uint16_t duty)
{
    uint32_t cnv;
    uint16_t cntin,mod;
    mod = period[ftmn];
    cntin = FTMn[ftmn]->CNTIN;
    cnv = duty * (mod - cntin);
    switch(ftmn)
    {
    case ftm0:
        cnv /= FTM0_PRECISON;
        break;
    case ftm1:
        cnv /= FTM1_PRECISON;
        break;
    case ftm2:
        cnv /= FTM2_PRECISON;
        break;
    case ftm3:
        cnv /= FTM3_PRECISON;
        break;
    default:
        break;
    }
    /* Set output on match to the requested level */
    FTMn[ftmn]->CONTROLS[ch].CnV = cnv;
}

void ftm_quad_init(FTMn_e ftmn)
{
    switch(ftmn)
    {
    case ftm1:
        SIM->SCGC6 |= SIM_SCGC6_FTM1_MASK;
        if(PTE20==FTM1_QDPHA)
        {
            port_init(FTM1_QDPHA,Alt5);
        }
        else if(PTB0==FTM1_QDPHA)
        {
            port_init(FTM1_QDPHA,Alt6);
        }
        else if(PTA12==FTM1_QDPHA)
        {
            port_init(FTM1_QDPHA,Alt7);
        }
        
        if(PTE21==FTM1_QDPHB)
        {
            port_init(FTM1_QDPHB,Alt5);
        }
        else if(PTB1==FTM1_QDPHB)
        {
            port_init(FTM1_QDPHB,Alt6);
        }
        else if(PTA13==FTM1_QDPHB)
        {
            port_init(FTM1_QDPHB,Alt7);
        }
        break;
    case ftm2:
        SIM->SCGC6 |= SIM_SCGC6_FTM2_MASK;
        if(PTE22==FTM2_QDPHA || PTA1==FTM2_QDPHA)
        {
            port_init(FTM2_QDPHA,Alt5);
        }
        else if(PTA10==FTM2_QDPHA || PTA18==FTM2_QDPHA)
        {
            port_init(FTM2_QDPHA,Alt6);
        }

        if(PTE23==FTM2_QDPHA || PTA2==FTM2_QDPHA)
        {
            port_init(FTM2_QDPHA,Alt5);
        }
        else if(PTA11==FTM2_QDPHA || PTB19==FTM2_QDPHA)
        {
            port_init(FTM2_QDPHA,Alt6);
        }
        break;
    default:
        break;
    }
    /* Write Protection Disable */
    FTMn[ftmn]->MODE &= FTM_MODE_WPDIS_MASK;
    /* Clear To Zero*/
    FTMn[ftmn]->QDCTRL &= FTM_QDCTRL_QUADMODE_MASK;
    /* Init */
    FTMn[ftmn]->CNTIN = 0;
    FTMn[ftmn]->MOD = FTM_MOD_MOD_MASK;
    /* Select QD Mode */
    FTMn[ftmn]->QDCTRL |= FTM_QDCTRL_QUADMODE_MASK;
    /* Enable */
    FTMn[ftmn]->QDCTRL |= FTM_QDCTRL_QUADEN_MASK;
    /* Init The Counter*/
    FTMn[ftmn]->CNT = 0;
}


uint16_t ftm_quad_get(FTMn_e ftmn)
{
    return FTMn[ftmn]->CNT;
}

void ftm_quad_clean(FTMn_e ftmn)
{
    FTMn[ftmn]->CNT = 0;
}

