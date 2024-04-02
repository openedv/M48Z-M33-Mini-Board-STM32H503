/**
 ****************************************************************************************************
 * @file        exti.c
 * @author      ALIENTEK
 * @brief       EXTI code
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

#include "exti.h"
#include "../KEY/key.h"
#include "../LED/led.h"



/**
  * @brief  EXTI line rising detection callback.
  * @param  GPIO_Pin: Specifies the port pin connected to corresponding EXTI line.
  * @retval None
  */
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
    HAL_Delay(20);       	   /* delay 20ms to eliminate chattering */
    switch (GPIO_Pin)
    {
        case KEY0_Pin:
        	if (KEY0 == 1)
        	{
        		LED_TOGGLE();  /* LED state is flipped */
        	}
        	break;
        case WK_UP_Pin:
        	if (WK_UP == 1)
        	{
        		LED_TOGGLE();  /* LED state is flipped */
        	}
        	break;
    }
}
