#include "Headfile.h"


static uint32_t fac_us = 0;

void InitDelay(uint8_t SYSCLK)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    fac_us = SYSCLK;
}

void DelayUs(uint32_t nus)
{
    uint32_t ticks = nus * fac_us;
    uint32_t told = DWT->CYCCNT;
    uint32_t tnow;
    while(1)
    {
        tnow = DWT->CYCCNT;
        if((tnow - told) >= ticks)
            break;
    }
}

void DelayMs(uint16_t nms)
{
    uint32_t i;
    for(i = 0; i < nms; i++)
    {
        DelayUs(1000);
    }
}
