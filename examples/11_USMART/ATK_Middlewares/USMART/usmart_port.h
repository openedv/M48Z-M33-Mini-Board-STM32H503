/**
 ****************************************************************************************************
 * @file        usmart_port.h
 * @author      ALIENTEK
 * @brief       USMART code
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
 
#ifndef __USMART_PORT_H
#define __USMART_PORT_H

#include "usart.h"

/******************************************************************************************/
/* User configuration parameters */


#define MAX_FNAME_LEN           30      /* The maximum length of a function name should be set to no less than the length of the longest function name. */
#define MAX_PARM                10      /* The maximum number of parameters is 10. To modify this parameter, the usmart_exe must be modified to correspond to it. */
#define PARM_LEN                200     /* The length of the sum of all parameters should not exceed PARM_LEN bytes, and the serial port receiving part should correspond to it (not less than PARM_LEN). */


#define USMART_ENTIMX_SCAN      1       /* Use TIM's timing interrupt to SCAN the scan function. If it is set to 0, you need to
                                         * implement your own implementation to scan the scan function once in a while.
                                         * Note: To use the runtime statistics feature, you must set USMART_ENTIMX_SCAN to 1!!!!
                                         */

#define USMART_USE_HELP         1       /* With help, this value is set to 0, which saves nearly 700 bytes, but prevents the help information from being displayed */
#define USMART_USE_WRFUNS       1       /* Using read and write functions, enable here, can read the value of any address, also can write the value of the register. */

#define USMART_PRINTF           printf  /* Define the printf output */

/******************************************************************************************/

char *usmart_get_input_string(void);    /* Get the input data stream */
void usmart_timx_reset_time(void);      /* Reset run time */
uint32_t usmart_timx_get_time(void);    /* Getting the run time */
void usmart_timx_init(void);   			/* Initialize the timer */

#endif

