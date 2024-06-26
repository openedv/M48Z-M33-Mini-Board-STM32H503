/**
 ****************************************************************************************************
 * @file        pwr.c
 * @author      ALIENTEK
 * @brief       PWR code
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

#include "pwr.h"
#include "usart.h"
#include "../LCD/lcd.h"


/**
 * @brief   Enter CPU stop mode
 * @param   None
 * @retval  None
 */
void pwr_enter_stop(void)
{
	HAL_PWR_ConfigAttributes(PWR_ALL, PWR_PRIV);
	HAL_SuspendTick();  /* Pause the tick clock to prevent wakeup from being interrupted by the tick clock */
	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI); /* Execute the WFI command and enter the stop mode */
}

/**
 * @brief    External interrupt callback function
 * @param    GPIO Pin : Interrupt pin
 * @note     This function is called by PWR WKUP INT IRQHandler()
 * @retval   None
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == WK_UP_Pin)
    {
        /* The HAL_GPIO_EXTI_IRQHandler() function has cleared the interrupt flag for us,
         *  so we can call the callback without doing anything */
    }
}
