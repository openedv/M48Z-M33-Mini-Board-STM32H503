/**
 ****************************************************************************************************
 * @file        pwr.h
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

#ifndef BSP_PWR_PWR_H_
#define BSP_PWR_PWR_H_

#include "stdint.h"

void pwr_pvd_init(uint32_t pls);     /* PVD voltage detection initialization function */

#endif /* BSP_PWR_PWR_H_ */
