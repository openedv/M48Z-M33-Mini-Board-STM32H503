/**
 ****************************************************************************************************
 * @file        malloc.h
 * @author      ALIENTEK
 * @brief       MALLOC code
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

#ifndef __MALLOC_H
#define __MALLOC_H

#include "main.h"

/* Define two memory pools */
#define SRAMIN1                 0         /* Internal memory pool */
#define SRAMIN2                 1         /* SRAM2 memory pool */
#define SRAMBANK                2         /* Defines the number of SRAM blocks supported */

#define MT_TYPE     uint16_t
 
/* mem1 Memory parameter setting.mem1 is completely inside the internal SRAM1 */
#define MEM1_BLOCK_SIZE         32                              /* The memory block size is 32 bytes */
#define MEM1_MAX_SIZE           12 * 1024                       /* The maximum management memory is 12K */
#define MEM1_ALLOC_TABLE_SIZE   MEM1_MAX_SIZE/MEM1_BLOCK_SIZE   /* Memory table size */

/* mem2 memory parameter setting.mem2 is in the internal SRAM2, used to manage the internal SRAM2 */
#define MEM2_BLOCK_SIZE         32                              /* The memory block size is 32 bytes */
#define MEM2_MAX_SIZE           14 * 1024                       /* The maximum management memory is 14K */
#define MEM2_ALLOC_TABLE_SIZE   MEM2_MAX_SIZE/MEM2_BLOCK_SIZE   /* Memory table size */

/* If NULL is not defined, define NULL */
#ifndef NULL
#define NULL 0
#endif


/* Memory Management controller */
struct _m_mallco_dev
{
    void (*init)(uint8_t);              /* initialize */
    uint16_t (*perused)(uint8_t);       /* memory usage */
    uint8_t *membase[SRAMBANK];         /* The memory pool manages the memory of the SRAMBANK regions */
    MT_TYPE *memmap[SRAMBANK];          /* Memory management status table */
    uint8_t  memrdy[SRAMBANK];          /* Memory management in place */
};

extern struct _m_mallco_dev mallco_dev;


/* User-called function */
void my_mem_init(uint8_t memx);                     /* Memory management initialization functions (external/internal calls) */
uint16_t my_mem_perused(uint8_t memx) ;             /* Get memory usage (external/internal calls) */
void my_mem_set(void *s, uint8_t c, uint32_t count);/* Memory setup functions */
void my_mem_copy(void *des, void *src, uint32_t n); /* Memory copy function */

void myfree(uint8_t memx, void *ptr);               /* Memory deallocation (external call) */
void *mymalloc(uint8_t memx, uint32_t size);        /* Memory allocation (external call) */
void *myrealloc(uint8_t memx, void *ptr, uint32_t size);    /* Reallocation (external call) */

#endif













