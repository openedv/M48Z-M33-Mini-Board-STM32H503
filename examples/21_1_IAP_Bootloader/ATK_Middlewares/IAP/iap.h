/**
 ****************************************************************************************************
 * @file        iap.h
 * @author      ALIENTEK
 * @brief       IAP code
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

#ifndef __IAP_H
#define __IAP_H

#include "main.h"



typedef void (*iapfun)(void);                   /* Define an argument of type function */

#define FLASH_APP1_ADDR         0x0800F000      /* The first application start address (stored in internal FLASH)
                                                 * The space from 0X08000000 to 0X0800F000(60KB) is reserved for the Bootloader
                                                 */

void iap_load_app(uint32_t appxaddr);                                         /* Jump to APP program execution */
void iap_write_appbin(uint32_t appxaddr, uint8_t *appbuf, uint32_t applen);   /* At the specified address, write to bin */

#endif







































