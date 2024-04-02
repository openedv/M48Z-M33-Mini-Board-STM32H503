/**
 ****************************************************************************************************
 * @file        iap.c
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


#include "iap.h"
#include "usart.h"
#include "../../BSP/STMFLASH/stmflash.h"


iapfun jump2app;
uint32_t g_iapbuf[512];       /* bytes cache */

/**
 * @brief   IAP writes to APP BIN
 * @param   appxaddr : The start address of the application
 * @param   appbuf   : the application CODE
 * @param   appsize  : Application size in bytes
 * @retval  None
 */
void iap_write_appbin(uint32_t appxaddr, uint8_t *appbuf, uint32_t appsize)
{
    uint32_t t;
    uint16_t i = 0;
    uint32_t temp;
    uint32_t fwaddr = appxaddr; /* The address currently being written to */
    uint8_t *dfu = appbuf;

    for (t = 0; t < appsize; t += 4)
    {
        temp = (uint32_t)dfu[3] << 24;
        temp |= (uint32_t)dfu[2] << 16;
        temp |= (uint32_t)dfu[1] << 8;
        temp |= (uint32_t)dfu[0];
        dfu += 4;               /* Offset by 4 bytes */
        g_iapbuf[i++] = temp;

        if (i == 512)
        {
            i = 0;
            stmflash_write(fwaddr, g_iapbuf, 512);
            fwaddr += 2048;     /* The offset is 2048, 16 is 2 times 8, so we have to multiply by 2 */
        }
    }

    if (i)
    {
        stmflash_write(fwaddr, g_iapbuf, i);  /* Write in the last few bytes of content */
    }
}

/**
 * @brief   Go to the APP section (execute APP)
 * @param   appxaddr : The start address of the application
 * @retval  None
 */
void iap_load_app(uint32_t appxaddr)
{
    if (((*(volatile  uint32_t *)appxaddr) & 0x2FFE0000) == 0x20000000)  /* Check that the top address of the stack is valid */
    {
        /* The second word in the user code area is the program start address (reset address). */
        jump2app = (iapfun) * (volatile uint32_t *)(appxaddr + 4);
        
        /* Initialize the APP stack pointer (the first word in the user code area is used to store the top address of the stack) */
        __set_MSP(appxaddr);
        
        /* Go to the APP */
        jump2app();
    }
}










