/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    rtc.h
  * @brief   This file contains all the function prototypes for
  *          the rtc.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RTC_H__
#define __RTC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern RTC_HandleTypeDef hrtc;

/* USER CODE BEGIN Private defines */


/* USER CODE END Private defines */

void MX_RTC_Init(void);

/* USER CODE BEGIN Prototypes */

/* interface function */
void hal_rtc_get_tim(void);							/* Official access to RTC time information */
uint16_t rtc_read_bkr(uint32_t bkrx);               /* Read the backing register */
void rtc_write_bkr(uint32_t bkrx, uint32_t data);   /* Write the backing register */
HAL_StatusTypeDef rtc_set_date(uint8_t year, uint8_t month, uint8_t date, uint8_t week);
void rtc_get_date(uint8_t *year, uint8_t *month, uint8_t *date, uint8_t *week); /* Get the RTC time information */
uint8_t rtc_get_week(uint16_t year, uint8_t month, uint8_t day);    /* Gets the day of the week based on the year, month and day */
HAL_StatusTypeDef rtc_set_time(uint8_t hour, uint8_t min, uint8_t sec, uint8_t ampm);   /* Set time */
void rtc_set_alarm(uint8_t week, uint8_t hour, uint8_t min, uint8_t sec);  /* Set an alarm time */
void rtc_get_time(uint8_t *hour, uint8_t *min, uint8_t *sec, uint8_t *ampm);
/* USER CODE END Private defines */

void MX_RTC_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __RTC_H__ */

