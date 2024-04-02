/**
 ****************************************************************************************************
 * @file        delay.h
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

#ifndef BSP_DELAY_DELAY_H_
#define BSP_DELAY_DELAY_H_

#include "main.h"

void delay_init(uint16_t sysclk);
void delay_us(uint32_t nus);

#endif /* BSP_DELAY_DELAY_H_ */
