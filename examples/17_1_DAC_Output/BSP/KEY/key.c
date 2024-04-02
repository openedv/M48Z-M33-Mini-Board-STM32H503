/**
 ****************************************************************************************************
 * @file        key.c
 * @author      ALIENTEK
 * @brief       KEY input code
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

#include "key.h"

/**
 * @brief       key scan function
 * @note        This function has response priority(multiple keys pressed at once):WK_UP > KEY0!!
 * @param       mode: 0/1, which means:
 * @arg         0, does not support sequential presses(when a key is held down,only the first call returns the key,
 *                 it must be released before another key is returned)
 * @arg         1, supports continuous pressing(when the key is held down,each call to this function returns the key)
 * @retval      key,defined as follows:
 *              KEY0_PRES, 1,KEY0 pressed
 *              WKUP_PRES, 2,WKUP is pressed
 */
uint8_t key_scan(uint8_t mode)
{
    static uint8_t key_up = 1;                /* Press the button to release the flag */
    uint8_t keyval = 0;

    if (mode == 1)
    {
    	key_up = 1;                           /* support for linking */
    }

    if (key_up && (KEY0 == 1 || WK_UP == 1))  /* The key release flag is 1, and any key is pressed */
    {
    	HAL_Delay(10);                        /* delay 10ms */
        key_up = 0;

        if (KEY0 == 1)
        {
        	keyval = KEY0_PRES;
        }

        if (WK_UP == 1)
        {
        	keyval = WKUP_PRES;
        }
    }
    else if (KEY0 == 0 && WK_UP == 0)         /* Without any key pressed, the marker key is released */
    {
        key_up = 1;
    }

    return keyval;                            /* return key value */
}




