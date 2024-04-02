/**
 ****************************************************************************************************
 * @file        pwr.c
 * @author      ALIENTEK
 * @brief       PWR code
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

#include "pwr.h"
#include "usart.h"
#include "../LCD/lcd.h"


/**
 * @brief    Initialize the PVD voltage monitor
 * @param    pls : PWR_PVD_detection_level
 *   @arg    PWR_PVDLEVEL_0,2.2V;
 *   @arg    PWR_PVDLEVEL_1,2.3V;
 *   @arg    PWR_PVDLEVEL_2,2.4V;
 *   @arg    PWR_PVDLEVEL_3,2.5V;
 *   @arg    PWR_PVDLEVEL_4,2.6V;
 *   @arg    PWR_PVDLEVEL_5,2.7V;
 *   @arg    PWR_PVDLEVEL_6,2.8V;
 *   @arg    PWR_PVDLEVEL_7,2.9V;
 * @retval   None
 */
void pwr_pvd_init(uint32_t pls)
{
    PWR_PVDTypeDef pwr_pvd = {0};

    pwr_pvd.PVDLevel = pls;                          /* Detecting voltage level */
    pwr_pvd.Mode = PWR_PVD_MODE_IT_RISING_FALLING;   /* A double edge trigger using the rising
                                                      * and falling edge of the interrupt line is used */
    HAL_PWR_ConfigPVD(&pwr_pvd);

    HAL_NVIC_SetPriority(PVD_AVD_IRQn, 3 ,3);
    HAL_NVIC_EnableIRQ(PVD_AVD_IRQn);
    HAL_PWR_EnablePVD();                             /* Enable PVD detection */
}

/**
 * @brief    PVD interrupt service function
 * @param    None
 * @retval   None
 */
void PVD_IRQHandler(void)
{
    HAL_PWR_PVD_IRQHandler();
}

/**
 * @brief    PVD interrupt service callback function
 * @param    None
 * @retval   None
 */
void HAL_PWR_PVDCallback(void)
{
    if (__HAL_PWR_GET_FLAG(PWR_FLAG_PVDO)) /* The voltage is lower than the one selected by PLS */
    {
        lcd_show_string(30, 130, 200, 16, 16, "PVD Low Voltage!", RED); /* LCD display voltage is low */
        printf("PVD Low Voltage!\r\n");
    }
    else
    {
    	printf("PVD Voltage OK!\r\n");
        lcd_show_string(30, 130, 200, 16, 16, "PVD Voltage OK! ", BLUE); /* The LCD display voltage is normal */

    }
}

