/**
 ****************************************************************************************************
 * @file        led.h
 * @author      ALIENTEK
 * @brief       LED code
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

#ifndef BSP_LED_LED_H_
#define BSP_LED_LED_H_

#include "main.h"

/******************************************************************************************/
/* LED port definition */
#define LED(x)   do{ x ? \
                      HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET); \
                  }while(0)      /* turn over LED */

/* LED port defined in reverse */
#define LED_TOGGLE()   do{ HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin); }while(0) /* turn over LED */

#endif /* BSP_LED_LED_H_ */
