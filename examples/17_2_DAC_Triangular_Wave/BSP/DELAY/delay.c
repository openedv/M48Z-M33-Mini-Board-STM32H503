/**
 ****************************************************************************************************
 * @file        delay.c
 * @author      ALIENTEK
 * @brief       Delay code
 * @license     Copyright (C) 2012-2024, ALIENTEK
 ****************************************************************************************************
 * @attention
 *
 * platform     : ALIENTEK M48-STM32H503 board
 * website      : www.alientek.com
 * forum        : www.openedv.com/forum.php
 *
 * change logs  ：
 * version      data         notes
 * V1.0         20240309     the first version
 *
 ****************************************************************************************************
 */


#include "delay.h"


static uint32_t g_fac_us = 0;       /* us time delay multiplier */

/**
 * @brief   Initializes the delay function
 * @param   sysclk : system clock frequency, that is, CPU frequency (rcc_c_ck), 250MHz
 * @retval  None
 */
void delay_init(uint16_t sysclk)
{
    g_fac_us = sysclk;
}

/**
 * @brief  Delay nus
 * @note   uses clock picking method to do us delay whether OS is used or not
 * @param  nus : The number of us to delay
 * @retval none
 */
void delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;    /* Value of LOAD */
    ticks = nus * g_fac_us;             /* The number of beats required */

    told = SysTick->VAL;                /* The value of the counter at the time of entry */
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;    /* It is sufficient to note here that SYSTICK is a decreasing counter */
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks)
            {
                break;                  /* If the time is more than/equal to the time to be delayed, exit */
            }
        }
    }
}

