/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    rtc.c
  * @brief   This file provides code for the configuration
  *          of the RTC instances.
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
/* Includes ------------------------------------------------------------------*/
#include "rtc.h"

/* USER CODE BEGIN 0 */
#include "usart.h"

/* USER CODE END 0 */

RTC_HandleTypeDef hrtc;

/* RTC init function */
void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */
  /* Check if this is the first configuration */
  uint16_t bkpflag = 0;
  /* USER CODE END RTC_Init 0 */

  RTC_PrivilegeStateTypeDef privilegeState = {0};
  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  RTC_AlarmTypeDef sAlarm = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  hrtc.Init.OutPutPullUp = RTC_OUTPUT_PULLUP_NONE;
  hrtc.Init.BinMode = RTC_BINARY_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  privilegeState.rtcPrivilegeFull = RTC_PRIVILEGE_FULL_NO;
  privilegeState.backupRegisterPrivZone = RTC_PRIVILEGE_BKUP_ZONE_NONE;
  privilegeState.backupRegisterStartZone2 = RTC_BKP_DR0;
  privilegeState.backupRegisterStartZone3 = RTC_BKP_DR0;
  if (HAL_RTCEx_PrivilegeModeSet(&hrtc, &privilegeState) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */
  bkpflag = rtc_read_bkr(0);  	/* Read the value of BKP0 */
  if (bkpflag != 0X5050) /* Uninitialized before, reconfigured */
  {
  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x9;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_WEDNESDAY;
  sDate.Month = RTC_MONTH_FEBRUARY;
  sDate.Date = 0x24;
  sDate.Year = 0x24;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable the Alarm A
  */
  sAlarm.AlarmTime.Hours = 0x0;
  sAlarm.AlarmTime.Minutes = 0x0;
  sAlarm.AlarmTime.Seconds = 0x0;
  sAlarm.AlarmTime.SubSeconds = 0x0;
  sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 0x1;
  sAlarm.Alarm = RTC_ALARM_A;
  sAlarm.FlagAutoClr = ALARM_FLAG_AUTOCLR_ENABLE;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */
  rtc_set_time(6, 59, 56, RTC_HOURFORMAT12_AM);       /* Set time. */
  rtc_set_date(24, 2, 28, 1);
   }
  /* USER CODE END RTC_Init 2 */

}

void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{

  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspInit 0 */
	HAL_PWR_EnableBkUpAccess();    /* Disable backup region write protection */
  /* USER CODE END RTC_MspInit 0 */

  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* RTC clock enable */
    __HAL_RCC_RTC_ENABLE();
    __HAL_RCC_RTCAPB_CLK_ENABLE();

    /* RTC interrupt Init */
    HAL_NVIC_SetPriority(RTC_IRQn, 2, 2);
    HAL_NVIC_EnableIRQ(RTC_IRQn);
  /* USER CODE BEGIN RTC_MspInit 1 */
    rtc_write_bkr(0, 0x5050);
  /* USER CODE END RTC_MspInit 1 */
  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspDeInit 0 */

  /* USER CODE END RTC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();
    __HAL_RCC_RTCAPB_CLK_DISABLE();

    /* RTC interrupt Deinit */
    HAL_NVIC_DisableIRQ(RTC_IRQn);
  /* USER CODE BEGIN RTC_MspDeInit 1 */

  /* USER CODE END RTC_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/**
 * @brief   RTC writes to the fallback region SRAM
 * @param   bkrx : fallback register number, range: 0-41
 *                 Corresponding to RTC_BKP_DR1 to RTC_BKP_DR42
 * @param   data : 16 bits of data to be written
 * @retval  None
 */
void rtc_write_bkr(uint32_t bkrx, uint32_t data)
{
    HAL_PWR_EnableBkUpAccess(); /* Disable write protection for the backup area */
    HAL_RTCEx_BKUPWrite(&hrtc, bkrx, data);
}

/**
 * @brief   RTC reads the fallback region SRAM
 * @param   bkrx: fallback register number, range: 0-31
 *                Corresponding to RTC_BKP_DR1 to RTC_BKP_DR42
 * @retval  the value read
 */
uint16_t rtc_read_bkr(uint32_t bkrx)
{
    uint32_t temp = 0;
    temp = HAL_RTCEx_BKUPRead(&hrtc, bkrx);
    return temp; /* Returns the value read */
}

/**
 * @brief   RTC time setting
 * @param   hour,min,sec: hour, minute, second
 * @param   ampm : AM/PM, 0=AM/24H; 1=PM/12H
 * @retval  0, success
 *          Other, abnormal state
 */
HAL_StatusTypeDef rtc_set_time(uint8_t hour, uint8_t min, uint8_t sec, uint8_t ampm)
{
	RTC_TimeTypeDef sTime = {0};

	sTime.Hours = hour;
	sTime.Minutes = min;
	sTime.Seconds = sec;
	sTime.TimeFormat = ampm;
	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sTime.StoreOperation = RTC_STOREOPERATION_RESET;

	return HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
}

/**
 * @brief   RTC date setting
 * @param   year,month,date : Year (0-99), month (1-12), day (0-31)
 * @param   week : Week (1 to 7,0, illegal!)
 * @retval  0, success
 *          Other, abnormal state
 */
HAL_StatusTypeDef rtc_set_date(uint8_t year, uint8_t month, uint8_t date, uint8_t week)
{
	RTC_DateTypeDef sDate = {0};

	sDate.Date = date;
	sDate.Month = month;
	sDate.WeekDay = week;
	sDate.Year = year;

	return HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
}

/**
 * @brief  Sets an alarm to the year, month, day, hour, and second
 * @note   takes January 1, 1970 as the base date and adds time backward
 *         Legal years range from 1970 to 2105
 * @param  week : Day of the week (1-7)
 * @param  hour,min,sec : hour, minute, second
 * @retval 0: success; 1: fail;
 */
void rtc_set_alarm(uint8_t week, uint8_t hour, uint8_t min, uint8_t sec)
{
	RTC_AlarmTypeDef sAlarm;

	sAlarm.AlarmTime.Hours = hour;
	sAlarm.AlarmTime.Minutes = min;
	sAlarm.AlarmTime.Seconds = sec;
	sAlarm.AlarmTime.SubSeconds = 0;
	sAlarm.AlarmTime.TimeFormat = RTC_HOURFORMAT12_AM;

	sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
	sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_NONE;
	sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;
	sAlarm.AlarmDateWeekDay = week;
	sAlarm.Alarm = RTC_ALARM_A;
	HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN);

}

/**
 * @brief  Gets the current time
 * @note   This function does not return the time directly, the time data is stored in the calendar struct
 * @param  *hour,*min,*sec : hour, minute, second
 * @param  *ampm : AM/PM,0=AM/24H,1=PM/12H.
 * @retval  None
 */
void rtc_get_time(uint8_t *hour, uint8_t *min, uint8_t *sec, uint8_t *ampm)
{
	RTC_TimeTypeDef sTime;

	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);

	*hour = sTime.Hours;
	*min = sTime.Minutes;
	*sec = sTime.Seconds;
    *ampm = sTime.TimeFormat;
}

/**
 * @brief    RTC date setting
 * @param    year,month,date: Year (0-99), month (1-12), day (0-31)
 * @param    week : Week (1 to 7,0, illegal!)
 * @retval   0, success; Other, abnormal state
 */
void rtc_get_date(uint8_t *year, uint8_t *month, uint8_t *date, uint8_t *week)
{
    RTC_DateTypeDef sDate;

    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);

    *year = sDate.Year;
    *month = sDate.Month;
    *date = sDate.Date;
    *week = sDate.WeekDay;
}

/* Monthly revised data sheet */
uint8_t const table_week[12] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};

/**
 * @brief   Converts the year, month, day, hour, minute, and second to a number of seconds
 * @note    Enter calendar date to get the day of the week (start time: March 1, 0 AD, enter any date after that to get the correct day of the week)
 *          Using the Kim-Larsson formula, see this post for an explanation:
 *          https://www.cnblogs.com/fengbohello/p/3264300.html
 * @param   syear : The year
 * @param   smon  : Month
 * @param   sday  : The date
 * @retval  0, Sunday; 1 ~ 6: Monday ~ Saturday
 */
uint8_t rtc_get_week(uint16_t year, uint8_t month, uint8_t day)
{
	uint16_t temp;
	uint8_t yearh, yearl;

	yearh = year / 100;
	yearl = year % 100;

	/*  For the 21st century, add 100 years */
	if (yearh > 19)
	{
	    yearl += 100;
	}

	/* The leap year counts only after 1900 */
	temp = yearl + yearl / 4;
	temp = temp % 7;
	temp = temp + day + table_week[month - 1];

	if (yearl % 4 == 0 && month < 3)
	{
	   temp--;
	}

	temp %= 7;

	if (temp == 0)
	{
	   temp = 7;
	}

	return temp;
}

/**
 * @breif   Periodic wake-up timer setting
 * @param   wksel :
 * @arg         RTC_WAKEUPCLOCK_RTCCLK_DIV16 ((uint32_t)0x00000000)
 * @arg         RTC_WAKEUPCLOCK_RTCCLK_DIV8 ((uint32_t)0x00000001)
 * @arg         RTC_WAKEUPCLOCK_RTCCLK_DIV4 ((uint32_t)0x00000002)
 * @arg         RTC_WAKEUPCLOCK_RTCCLK_DIV2 ((uint32_t)0x00000003)
 * @arg         RTC_WAKEUPCLOCK_CK_SPRE_16BITS ((uint32_t)0x00000004)
 * @arg         RTC_WAKEUPCLOCK_CK_SPRE_17BITS ((uint32_t)0x00000006)
 * @note    000,RTC/16;001,RTC/8;010,RTC/4;
 *          RTC is the clock frequency of RTC, that is, RTCCLK!
 * @param   cnt : Automatic reload value. Decrement to 0, an interrupt is generated.
 * @retval  None.
 */
void rtc_set_wakeup(uint8_t wksel, uint16_t cnt)
{
    __HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_WUTF);  /* Clear the RTC WAKE UP flag */

    HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, cnt, wksel, 0);       /* Set the reload value and clock */
}

/**
  * @brief  Alarm A secure callback.
  * @param  hrtc RTC handle
  * @retval None
  */
void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
    printf("ALARM A!\r\n");
}

/**
  * @brief  Wake Up Timer callback.
  * @param  hrtc RTC handle
  * @retval None
  */
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
    printf("WakeUp!\r\n");
}

/* USER CODE END 1 */
