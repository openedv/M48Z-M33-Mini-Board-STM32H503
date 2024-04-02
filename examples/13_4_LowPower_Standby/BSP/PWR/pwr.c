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

/**
 * @brief   Enter CPU standby mode
 * @param   None
 * @retval  None
 */
void pwr_enter_standby(void)
{
	HAL_PWR_ConfigAttributes(PWR_ALL, PWR_PRIV);

	if (__HAL_PWR_GET_FLAG(PWR_FLAG_SBF) != 0)
	{
		/* Clear the standby mode flag */
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_SBF);
		/* The PWR WAKEUP FLAG1 bit is detected and cleared */
		if (__HAL_PWR_GET_FLAG(PWR_WAKEUP_FLAG1) != 0)
		{
		   __HAL_PWR_CLEAR_FLAG(PWR_WAKEUP_FLAG1);
		}
	}

	HAL_PWR_EnableWakeUpPin(PWR_WUCR_WUPEN1);  /* Enable the wake-up function of the KEY UP pin */
	__HAL_PWR_GET_FLAG(PWR_WAKEUP_FLAG1);      /* You need to clear this flag or you will remain awake */
	HAL_PWR_EnterSTANDBYMode();                /* Enter standby mode */
}

