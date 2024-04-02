/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.c
  * @brief   This file provides code for the configuration
  *          of the TIM instances.
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
#include "tim.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

TIM_HandleTypeDef htim3;

/* TIM3 init function */
void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 250-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim3, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */
  __HAL_TIM_ENABLE_IT(&htim3, TIM_IT_UPDATE);     	/* Enable update interrupts */
  HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);    	/* Start capturing channel 1 of TIM3 */
  /* USER CODE END TIM3_Init 2 */

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(tim_baseHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspInit 0 */

  /* USER CODE END TIM3_MspInit 0 */
    /* TIM3 clock enable */
    __HAL_RCC_TIM3_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM3 GPIO Configuration
    PA0     ------> TIM3_CH1
    */
    GPIO_InitStruct.Pin = WK_UP_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
    HAL_GPIO_Init(WK_UP_GPIO_Port, &GPIO_InitStruct);

    /* TIM3 interrupt Init */
    HAL_NVIC_SetPriority(TIM3_IRQn, 2, 1);
    HAL_NVIC_EnableIRQ(TIM3_IRQn);
  /* USER CODE BEGIN TIM3_MspInit 1 */

  /* USER CODE END TIM3_MspInit 1 */
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspDeInit 0 */

  /* USER CODE END TIM3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM3_CLK_DISABLE();

    /**TIM3 GPIO Configuration
    PA0     ------> TIM3_CH1
    */
    HAL_GPIO_DeInit(WK_UP_GPIO_Port, WK_UP_Pin);

    /* TIM3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM3_IRQn);
  /* USER CODE BEGIN TIM3_MspDeInit 1 */

  /* USER CODE END TIM3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* Input captured state (g_timxchy_cap_sta)
 * [7] :0, no capture was successful; 1, one successful capture.
 * [6] :0, the high level has not been captured yet; 1, the high level has been captured.
 * [5:0]: The number of overflows after capturing high levels. The maximum number of overflows is 63, so the maximum captured value is = 63*65536 + 65535 = 4194303
 * Note: For generality, we default ARR and CCRy to 16-bit registers, and only use 16-bit for 32-bit timers like TIM5
 * The maximum overflow time at 1us counting frequency is :4194303 us, about 4.19 seconds
 *
 * (Note: For normal 32-bit timers,1us counter incrementing by 1, overflow time :4294 seconds)
 */
uint8_t g_timxchy_cap_sta = 0;    /* Input capture state. */
uint16_t g_timxchy_cap_val = 0;   /* Input capture value. */

/**
  * @brief  Input Capture callback in non-blocking mode
  * @param  htim TIM IC handle
  * @retval None
  */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM3)
    {
        if ((g_timxchy_cap_sta & 0X80) == 0)                	/* No capture yet. */
        {
            if (g_timxchy_cap_sta & 0X40)                   	/* A falling edge is captured. */
            {
                g_timxchy_cap_sta |= 0X80;                  	/* The marker successfully captured one high level pulse width. */
                g_timxchy_cap_val = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);  /* Gets the current capture value. */
                TIM_RESET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1); /* Be sure to clear the original Settings first */
                __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);/* Configure TIM3 channel 1 rising edge capture. */
            }
            else /* Not yet started, first capture rising edge */
            {
                g_timxchy_cap_sta = 0;                      	/* clear */
                g_timxchy_cap_val = 0;
                g_timxchy_cap_sta |= 0X40;                     	/* The marker has captured the rising edge. */
                __HAL_TIM_DISABLE(htim);          				/* Turn off timer 3 */
                __HAL_TIM_SET_COUNTER(htim, 0);   				/* Timer 3 The counter is cleared */
                TIM_RESET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1); /* Be sure to clear the original Settings first */
                __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);/* Timer 3 Channel 1 is set to falling edge capture */
                __HAL_TIM_ENABLE(htim);           				/* Enable timer 3 */
            }
        }
    }
}

/**
  * @brief  Period elapsed callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM3)
    {
        if ((g_timxchy_cap_sta & 0X80) == 0)            /* No capture yet */
        {
            if (g_timxchy_cap_sta & 0X40)               /* High levels have been captured */
            {
                if ((g_timxchy_cap_sta & 0X3F) == 0X3F) /* The high level is too long */
                {
                	TIM_RESET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1); /* Be sure to clear the original Settings first */
                	__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);/* Configure TIM3 channel 1 rising edge capture */
                    g_timxchy_cap_sta |= 0X80;          /* The tag was successfully captured once */
                    g_timxchy_cap_val = 0XFFFF;
                }
                else      /* Accumulate the number of timer overflows */
                {
                    g_timxchy_cap_sta++;
                }
            }
        }
    }
}
/* USER CODE END 1 */
