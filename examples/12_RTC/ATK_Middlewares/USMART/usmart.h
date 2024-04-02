/**
 ****************************************************************************************************
 * @file        usmart.h
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

#ifndef __USMART_H
#define __USMART_H

#include "usmart_port.h"


#define USMART_OK               0       /* No error */
#define USMART_FUNCERR          1       /* Function errors */
#define USMART_PARMERR          2       /* Parameter error */
#define USMART_PARMOVER         3       /* Parameter overflow */
#define USMART_NOFUNCFIND       4       /* No matching function found */

#define SP_TYPE_DEC             0       /* Decimal argument display */
#define SP_TYPE_HEX             1       /* Hexadecimal parameter display */


/* List of function names */
struct _m_usmart_nametab
{
    void *func;             /* function pointer */
    const char *name;       /* Function name (lookup string) */
};

/* The usmart control manager */
struct _m_usmart_dev
{
    struct _m_usmart_nametab *funs;     /* Function name pointer */

    void (*init)(uint16_t tclk);        /* Initialize */
    uint8_t (*cmd_rec)(char *str);   	/* Identify the function name and parameters */
    void (*exe)(void);                  /* Execute */
    void (*scan)(void);                 /* Scan */
    uint8_t fnum;                       /* Number of functions */
    uint8_t pnum;                       /* arity */
    uint8_t id;                         /* Function ID */
    uint8_t sptype;                     /* Argument display type (non-string argument):0:decimal; 1:hexadecimal; */
    uint16_t parmtype;                  /* Types of parameters */
    uint8_t  plentbl[MAX_PARM];         /* The length of each parameter is temporarily stored in the table */
    uint8_t  parm[PARM_LEN];            /* Function arguments */
    uint8_t runtimeflag;                /* 0, no function execution time. 1. Count the execution time of the function.
                                           Note: this feature must be enabled by USMART_ENTIMX_SCAN to be useful */
    uint32_t runtime;                   /* Running time, unit :0.1ms, the maximum delay time is 2 times of the CNT value of the timer *0.1ms */
};

extern struct _m_usmart_nametab usmart_nametab[];   /* Defined in usmart config.c */
extern struct _m_usmart_dev usmart_dev;             /* Defined in usmart config.c */


void usmart_init(uint16_t tclk);     	/* Initialize */
uint8_t usmart_cmd_rec(char*str);    	/* recognition */
void usmart_exe(void);                  /* Execute */
void usmart_scan(void);                 /* Scan */
uint32_t read_addr(uint32_t addr);      /* Identifies the value that reads the specified address */
void write_addr(uint32_t addr,uint32_t val);/* Writes the specified value at the specified address */

#endif




