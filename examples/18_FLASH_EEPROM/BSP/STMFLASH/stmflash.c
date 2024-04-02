/**
 ****************************************************************************************************
 * @file        stmflash.c
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

#include "stmflash.h"
#include "usart.h"

/**
 * @brief    Reads a word from the specified address (32-bit data)
 * @param    faddr : Read the address (this address must be a multiple of 4!!)
 * @retval   Read data (32 bits)
 */
uint32_t stmflash_read_word(uint32_t faddr)
{
    return *(volatile uint32_t *)faddr;
}

/**
 * @brief   reads the specified length of data starting at the specified address
 * @param   raddr  : the start address
 * @param   pbuf   : Data pointer
 * @param   length : The number of words (32-bit) to read, which is an integer multiple of 4 bytes
 * @retval  None
 */
void stmflash_read(uint32_t raddr, uint32_t *pbuf, uint32_t length)
{
    uint32_t  i;

    for (i = 0; i < length; i++)
    {
        pbuf[i] = stmflash_read_word(raddr);/* Read 4 bytes */
        raddr += 4;                         /* Offset by 4 bytes */
    }
}

/**
 * @brief   Gets the flash sector of an address
 * @param   addr : The flash address
 * @retval  specifies the flash sector where the address is located
 */
static uint32_t stmflash_get_flash_sector(uint32_t addr)
{
    uint32_t sector = 0;

    if ((addr >= FLASH_BASE) && (addr < FLASH_BASE + FLASH_BANK_SIZE))
    {
        sector = (addr & ~FLASH_BASE) / FLASH_SECTOR_SIZE;
    }
    else if ((addr >= FLASH_BASE + FLASH_BANK_SIZE) && (addr < FLASH_BASE + FLASH_SIZE))
    {
        sector = ((addr & ~FLASH_BASE) - FLASH_BANK_SIZE) / FLASH_SECTOR_SIZE;
    }
    else
    {
        sector = 0xFFFFFFFF;    /* address out of range */
    }

    return sector;
}

/**
 * @brief   Gets the flash bank of an address
 * @param   addr : The flash address
 * @retval  returns FLASH_BANK_1, FLASH_BANK_2, 0xFFFFFFFF (for address overflow)
 */
static uint32_t stmflash_get_flash_bank(uint32_t addr)
{
    uint32_t bank = 0;

    if((addr >= FLASH_BASE) && (addr < FLASH_BASE + FLASH_BANK_SIZE))
    {
        bank = FLASH_BANK_1;
    }
    else if ((addr >= FLASH_BASE + FLASH_BANK_SIZE) && (addr < FLASH_BASE + FLASH_SIZE))
    {
        bank = FLASH_BANK_2;
    }
    else
    {
        bank = 0xFFFFFFFF;      /* address out of range */
    }

    return bank;
}

/**
 * @brief  Writes a specified length of data to a specified address
 * @param  waddr  : Specifies the starting address for writing data
 * @param  pbuf   : Holds the starting address for writing data
 * @param  length : This specifies the length of the data to be written in words
 * @retval None
 */
void stmflash_write(uint32_t waddr, uint32_t *pbuf, uint32_t length)
{
    uint32_t addrx;
    uint32_t endaddr;
    uint32_t first_sector = 0;
    uint32_t num_sectors = 0;
    uint32_t bank_number = 0;
    uint32_t erase_addr;                                                /* Erase error; this value is the address of the sector where the error occurred */
    FLASH_EraseInitTypeDef flash_erase_init = {0};
    HAL_StatusTypeDef status = HAL_OK;

    if ((waddr < STM32_FLASH_BASE) ||                                   /* The specified address is smaller than the flash start address */
        (waddr > (STM32_FLASH_BASE + STM32_FLASH_SIZE)) ||              /* The specified address is greater than the final flash address */
         waddr % 4)                                                     /* The specified address is not aligned by 4 bytes */
    {
        return;                                                         /* illegal address */
    }

    HAL_ICACHE_Disable();                                               /* Disable instruction caching */
    HAL_FLASH_Unlock();                                                 /* FLASH unlock */

    addrx = waddr;                                                      /* The starting address to which data are written */
    endaddr = waddr + length * 4;                                       /* The end address at which data should be written */

    first_sector = stmflash_get_flash_sector(addrx);                    /* Gets the first sector to erase */
    num_sectors = stmflash_get_flash_sector(endaddr) - first_sector + 1;/* Gets the number of sectors to be erased */
    bank_number = stmflash_get_flash_bank(addrx);                       /* Get the bank for the given address */

    if (addrx < 0x081FFFFF)                                             /* Only the main storage area needs to be erased */
    {
        while (addrx < endaddr)                                         /* Clear all obstacles (for non-FFFFFFFF, erase first) */
        {
            if ((uint32_t)stmflash_read_word(addrx) != 0xFFFFFFFF)      /* There exists a non-0xFFFFFFFF */
            {
                flash_erase_init.TypeErase = FLASH_TYPEERASE_SECTORS;   /* In the form of sector erasure */
                flash_erase_init.Banks = bank_number;                   /* Erase the bank */
                flash_erase_init.Sector = first_sector;                 /* The first sector to erase */
                flash_erase_init.NbSectors = num_sectors;               /* The number of sectors to erase */
                status = HAL_FLASHEx_Erase( &flash_erase_init, &erase_addr);

                if (status == HAL_OK)                                   /* Erase successfully */
                {
                    break;
                }
            }
            else                                                        /* No need to erase */
            {
                addrx += 4;
            }
        }
    }

    if (status == HAL_OK)                                               /* There is no error in erasing a sector */
    {
        for(int i = 0; i < length; i++)
        {
            HAL_FLASH_Program(FLASH_TYPEPROGRAM_QUADWORD, waddr, (uint32_t)&pbuf[i]);
            waddr += 4;                                                 /* Point to the next half word */
        }
    }

    HAL_FLASH_Lock();                                                   /* Lock */
    HAL_ICACHE_Enable();                                                /* Enable instruction caching */
}

/***************************************************************************************************************************************/
/* Test code */

/**
 * @brief   test write data (write 1 word)
 * @param   waddr : Start address
 * @param   wdata : The data to write
 * @retval  the data read
 */
void stmflash_test_write(uint32_t waddr, uint32_t wdata)
{
    stmflash_write(waddr, &wdata, 1);        /* Write a word */
}





