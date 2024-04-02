/**
 ****************************************************************************************************
 * @file        usmart_str.h
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

#ifndef __USMART_STR_H
#define __USMART_STR_H

#include "usmart_port.h"


uint8_t usmart_get_parmpos(uint8_t num);          /* Get the starting position of a given parameter in the parameter column */
uint8_t usmart_strcmp(char *str1, char *str2);    /* Tests two strings for equality */
uint32_t usmart_pow(uint8_t m, uint8_t n);        /* M^N */
uint8_t usmart_str2num(char *str, uint32_t *res); /* Converting strings to numbers */
uint8_t usmart_get_cmdname(char *str, char *cmdname, uint8_t *nlen, uint8_t maxlen); /* Get the instruction name from str and return the instruction length */
uint8_t usmart_get_fname(char *str, char *fname, uint8_t *pnum, uint8_t *rval);/* Get the function name from str */
uint8_t usmart_get_aparm(char *str, char *fparm, uint8_t *ptype);   /* Get a function argument from str */
uint8_t usmart_get_fparam(char *str, uint8_t *parn);  /* Get all the function arguments in str. */

#endif











