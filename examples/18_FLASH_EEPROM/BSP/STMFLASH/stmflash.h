/**
 ****************************************************************************************************
 * @file        stmflash.h
 * @author      ALIENTEK Team
 * @version     V1.0
 * @date        Feb 22, 2024
 * @brief       This file provides the driver for internal FLASH.
 * @license     Copyright (C) 2020-2032, ALIENTEK
 ****************************************************************************************************
 * @attention
 *
 * Experimental platform  : ALIENTEK STM32H503 development board
 * Online video           : https://www.yuanzige.com
 * Technical Forum        : http://www.openedv.com/forum.php
 * Company website        : https://www.alientek.com
 * Buy from               : https://www.aliexpress.com/store/1102909571
 *
 ****************************************************************************************************
 */

#ifndef BSP_STMFLASH_STMFLASH_H_
#define BSP_STMFLASH_STMFLASH_H_

#include "main.h"

/* FLASH start address */
#define STM32_FLASH_BASE        0x08000000                            /* STM32 FLASH start address */
#define STM32_FLASH_SIZE        0x20000                               /* Total STM32 FLASH size */
#define FLASH_WAITETIME         50000                                 /* Flash waits for a timeout */


/* function declaration */
uint32_t stmflash_read_word(uint32_t faddr);                          /* Read out the word */
void stmflash_write(uint32_t waddr, uint32_t *pbuf, uint32_t length); /* Writes data of the specified length starting at the specified address */
void stmflash_read(uint32_t raddr, uint32_t *pbuf, uint32_t length);  /* Read data of the specified length starting at the specified address */
void stmflash_test_write(uint32_t waddr, uint32_t wdata);             /* Test write */

#endif /* BSP_STMFLASH_STMFLASH_H_ */
