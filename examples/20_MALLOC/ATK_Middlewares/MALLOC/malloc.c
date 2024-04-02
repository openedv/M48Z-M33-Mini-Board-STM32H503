/**
 ****************************************************************************************************
 * @file        malloc.c
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

#include "malloc.h"

/* Memory pool (64 byte alignment) */
static __ALIGNED(64) uint8_t mem1base[MEM1_MAX_SIZE];           /* Internal SRAM memory pool */
static __ALIGNED(64) uint8_t mem2base[MEM2_MAX_SIZE] __attribute__((section(".bss.ARM.__at_0x20004000")));  /* Internal SRAM2 memory pool */

/* Memory management table */
static MT_TYPE mem1mapbase[MEM1_ALLOC_TABLE_SIZE];                                                          /* Internal SRAM1 memory pool MAP */
static MT_TYPE mem2mapbase[MEM2_ALLOC_TABLE_SIZE] __attribute__((section(".bss.ARM.__at_0x20008000")));     /* Internal SRAM2 memory pool MAP */


/* Memory management parameters */
const uint32_t memtblsize[SRAMBANK] = {MEM1_ALLOC_TABLE_SIZE, MEM2_ALLOC_TABLE_SIZE};   /* Memory table size */
const uint32_t memblksize[SRAMBANK] = {MEM1_BLOCK_SIZE, MEM2_BLOCK_SIZE};               /* Memory chunk size */
const uint32_t memsize[SRAMBANK]    = {MEM1_MAX_SIZE, MEM2_MAX_SIZE};                   /* Total memory size */

/* Memory Management controller */
struct _m_mallco_dev mallco_dev =
{
    my_mem_init,                    /* Memory initialization */
    my_mem_perused,                 /* memory usage */
    mem1base, mem2base,             /* memory pool */
    mem1mapbase, mem2mapbase,       /* Memory management status table */
    0, 0,                           /* Memory management is not in place */
};

/**
 * @brief   Copies memory
 * @param  *des: destination address
 * @param  *src: the source address
 * @param   n: The amount of memory (in bytes) to copy from
 * @retval  None
 */
void my_mem_copy(void *des, void *src, uint32_t n)
{
    uint8_t *xdes = des;
    uint8_t *xsrc = src;

    while (n--)*xdes++ = *xsrc++;
}

/**
 * @brief  Sets the memory value
 * @param  *s    : Memory start address
 * @param   c    : The value to set
 * @param  count : The amount of memory (in bytes) to set
 * @retval None
 */
void my_mem_set(void *s, uint8_t c, uint32_t count)
{
    uint8_t *xs = s;

    while (count--)*xs++ = c;
}

/**
 * @brief   Memory management initialization
 * @param   memx : The memory block it belongs to
 * @retval  None
 */
void my_mem_init(uint8_t memx)
{
    uint8_t mttsize = sizeof(MT_TYPE);  /* Gets the type length of the memmap array (uint16t/uint32t) */
    my_mem_set(mallco_dev.memmap[memx], 0, memtblsize[memx]*mttsize); /* Memory state table data reset */
    mallco_dev.memrdy[memx] = 1;        /* Memory management initializes OK */
}

/**
 * @brief   Gets memory usage
 * @param   memx : The memory block it belongs to
 * @retval  usage (10x larger, 0-1000, 0.0%-100.0%)
 */
uint16_t my_mem_perused(uint8_t memx)
{
    uint32_t used = 0;
    uint32_t i;

    for (i = 0; i < memtblsize[memx]; i++)
    {
        if (mallco_dev.memmap[memx][i])used++;
    }

    return (used * 1000) / (memtblsize[memx]);
}

/**
 * @brief  Memory allocation (internal call)
 * @param  memx : The memory block it belongs to
 * @param  size : The size (in bytes) of memory to allocate
 * @retval memory offset address
 * @arg    0 to 0XFFFFFFFE : A valid memory offset
 * @arg    0XFFFFFFFF      : Invalid memory offset address
 */
static uint32_t my_mem_malloc(uint8_t memx, uint32_t size)
{
    signed long offset = 0;
    uint32_t nmemb;     /* The number of memory blocks required */
    uint32_t cmemb = 0; /* The number of contiguous empty memory blocks */
    uint32_t i;

    if (!mallco_dev.memrdy[memx])
    {
        mallco_dev.init(memx);          /* Uninitialized, initialized first */
    }
    
    if (size == 0) return 0XFFFFFFFF;   /* No allocation required */

    nmemb = size / memblksize[memx];    /* Gets the number of contiguous memory blocks to allocate */

    if (size % memblksize[memx]) nmemb++;

    for (offset = memtblsize[memx] - 1; offset >= 0; offset--)  /* Search the entire memory control area */
    {
        if (!mallco_dev.memmap[memx][offset])
        {
            cmemb++;            /* The number of contiguous empty memory blocks is increased */
        }
        else 
        {
            cmemb = 0;          /* Contiguous memory blocks are reset */
        }
        
        if (cmemb == nmemb)     /* nmemb consecutive empty memory blocks are found */
        {
            for (i = 0; i < nmemb; i++) /* Note that the memory block is not empty */
            {
                mallco_dev.memmap[memx][offset + i] = nmemb;
            }

            return (offset * memblksize[memx]); /* Returns the offset address */
        }
    }

    return 0XFFFFFFFF;  /* No block suitable for allocation was found */
}

/**
 * @brief   Frees memory (internal call)
 * @param   memx   : The memory block it belongs to
 * @param   offset : Memory address offset
 * @retval  releases the result
 * @arg     0, release was successful;
 * @arg     1, release failed;
 * @arg     2, hyperregion (fail);
 */
static uint8_t my_mem_free(uint8_t memx, uint32_t offset)
{
    int i;

    if (!mallco_dev.memrdy[memx])                   /* Uninitialized, initialized first */
    {
        mallco_dev.init(memx);
        return 1;                                   /* uninitialized */
    }

    if (offset < memsize[memx])                     /* The offset is in the memory pool. */
    {
        int index = offset / memblksize[memx];      /* The memory block number in which the offset is located */
        int nmemb = mallco_dev.memmap[memx][index]; /* Number of memory blocks */

        for (i = 0; i < nmemb; i++)                 /* Memory blocks are reset */
        {
            mallco_dev.memmap[memx][index + i] = 0;
        }

        return 0;
    }
    else
    {
        return 2;                                   /* The offset is out of range. */
    }
}

/**
 * @brief   Frees memory (external call)
 * @param   memx : The memory block it belongs to
 * @param   ptr  : Memory head address
 * @retval  None
 */
void myfree(uint8_t memx, void *ptr)
{
    uint32_t offset;

    if (ptr == NULL)return;     /* address incomplete */

    offset = (uint32_t)ptr - (uint32_t)mallco_dev.membase[memx];
    my_mem_free(memx, offset);  /* free the memory */
}

/**
 * @brief   Allocate memory (external call)
 * @param   memx : The memory block it belongs to
 * @param   size : The size (in bytes) of memory to allocate
 * @retval  at the head of the allocated memory.
 */
void *mymalloc(uint8_t memx, uint32_t size)
{
    uint32_t offset;
    offset = my_mem_malloc(memx, size);

    if (offset == 0XFFFFFFFF)   /* Error in application */
    {
        return NULL;            /* line of return */
    }
    else                        /* No problem with the application. Return to the first address */
    {
        return (void *)((uint32_t)mallco_dev.membase[memx] + offset);
    }
}

/**
 * @brief   Reallocate memory (external call)
 * @param   memx : The memory block it belongs to
 * @param  *ptr  : old memory head address
 * @param   size : The size (in bytes) of memory to allocate
 * @retval  The first address of the newly allocated memory.
 */
void *myrealloc(uint8_t memx, void *ptr, uint32_t size)
{
    uint32_t offset;
    offset = my_mem_malloc(memx, size);

    if (offset == 0XFFFFFFFF)   /* Error in application */
    {
        return NULL;            /* line of return */
    }
    else                        /* No problem with the application. Return to the first address */
    {
        my_mem_copy((void *)((uint32_t)mallco_dev.membase[memx] + offset), ptr, size); /* Copy old memory contents to new memory. */
        myfree(memx, ptr);      /* Freeing old memory */
        return (void *)((uint32_t)mallco_dev.membase[memx] + offset);   /* Returns the new memory head address */
    }
}


