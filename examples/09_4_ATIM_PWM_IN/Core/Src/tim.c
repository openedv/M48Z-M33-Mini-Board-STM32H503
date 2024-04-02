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

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;

/* TIM1 init function */
void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;
  sSlaveConfig.InputTrigger = TIM_TS_TI1FP1;
  sSlaveConfig.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sSlaveConfig.TriggerFilter = 0;
  if (HAL_TIM_SlaveConfigSynchro(&htim1, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  sConfigIC.ICSelection = TIM_ICSELECTION_INDIRECTTI;
  if (HAL_TIM_IC_ConfigChannel(&htim1, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */
    __HAL_TIM_ENABLE_IT(&htim1, TIM_IT_UPDATE);
    HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_1);
    HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_2);
  /* USER CODE END TIM1_Init 2 */

}
/* TIM2 init function */
void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 250-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 99;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 50;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);       /* Open the corresponding PWM channel */
  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(tim_baseHandle->Instance==TIM1)
  {
  /* USER CODE BEGIN TIM1_MspInit 0 */

  /* USER CODE END TIM1_MspInit 0 */
    /* TIM1 clock enable */
    __HAL_RCC_TIM1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM1 GPIO Configuration
    PA8     ------> TIM1_CH1
    */
    GPIO_InitStruct.Pin = LED_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
    HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

    /* TIM1 interrupt Init */
    HAL_NVIC_SetPriority(TIM1_UP_IRQn, 2, 1);
    HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);
    HAL_NVIC_SetPriority(TIM1_CC_IRQn, 2, 1);
    HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
  /* USER CODE BEGIN TIM1_MspInit 1 */

  /* USER CODE END TIM1_MspInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspInit 0 */

  /* USER CODE END TIM2_MspInit 0 */
    /* TIM2 clock enable */
    __HAL_RCC_TIM2_CLK_ENABLE();
  /* USER CODE BEGIN TIM2_MspInit 1 */

  /* USER CODE END TIM2_MspInit 1 */
  }
}
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(timHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspPostInit 0 */

  /* USER CODE END TIM2_MspPostInit 0 */

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM2 GPIO Configuration
    PA1     ------> TIM2_CH2
    */
    GPIO_InitStruct.Pin = TIM2_CH2_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
    HAL_GPIO_Init(TIM2_CH2_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM2_MspPostInit 1 */

  /* USER CODE END TIM2_MspPostInit 1 */
  }

}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM1)
  {
  /* USER CODE BEGIN TIM1_MspDeInit 0 */

  /* USER CODE END TIM1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM1_CLK_DISABLE();

    /**TIM1 GPIO Configuration
    PA8     ------> TIM1_CH1
    */
    HAL_GPIO_DeInit(LED_GPIO_Port, LED_Pin);

    /* TIM1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM1_UP_IRQn);
    HAL_NVIC_DisableIRQ(TIM1_CC_IRQn);
  /* USER CODE BEGIN TIM1_MspDeInit 1 */

  /* USER CODE END TIM1_MspDeInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspDeInit 0 */

  /* USER CODE END TIM2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM2_CLK_DISABLE();
  /* USER CODE BEGIN TIM2_MspDeInit 1 */

  /* USER CODE END TIM2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/* PWM input state (g_timxchy_cap_sta)
 * 0, no capture.
 * 1, the capture was successful
 */
uint8_t g_timxchy_pwmin_sta   = 0;  /* PWM input status. */
uint16_t g_timxchy_pwmin_psc  = 0;  /* PWM input frequency division coefficient. */
uint32_t g_timxchy_pwmin_hval = 0;  /* High level pulse width of PWM. */
uint32_t g_timxchy_pwmin_cval = 0;  /* PWM cycle width. */

/*
 * @brief  Timer TIMX PWM input mode restarts capture.
 * @param  None.
 * @retval None.
 */
void atim_timx_pwmin_chy_restart(void)
{
	__ASM volatile("cpsid i");                      /* Turn off interrupt */

    g_timxchy_pwmin_sta = 0;                      	/* Reset the state and start the detection again. */
    g_timxchy_pwmin_psc = 0;                       	/* The frequency division coefficient is cleared to zero. */

    __HAL_TIM_SET_PRESCALER(&htim1, 0);           	/* It is collected at the maximum counting frequency to get the best accuracy. */
    __HAL_TIM_SET_COUNTER(&htim1, 0);             	/* Clear counter */

    __HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC1);      	/* Enable channel 1 to capture the interrupt */
    __HAL_TIM_ENABLE_IT(&htim1, TIM_IT_UPDATE);  	/* Enable overflow interrupts */
    __HAL_TIM_ENABLE(&htim1);                     	/* Enable TIMX */

    TIM1->SR = 0;                          			/* Clear all interrupt flags. */

    __ASM volatile("cpsie i");                      /* Open interrupts */
}

/*
 * @brief  Timer TIMX channel Y PWM input mode interrupt handler function
 * @param  None.
 * @retval None.
 */
void atim_timx_pwmin_chy_process(void)
{
    static uint8_t sflag = 0;               				 /* Start PWMIN input detection flag. */

    if (g_timxchy_pwmin_sta)
    {
        g_timxchy_pwmin_psc = 0;
        TIM1->SR = 0;                                        /* Clear all interrupt flags. */
        __HAL_TIM_SET_COUNTER(&htim1, 0);             		 /* Clear counter */
        return ;
    }

    if (__HAL_TIM_GET_FLAG(&htim1, TIM_FLAG_UPDATE))         /* An overflow interrupt has occurred. */
    {
        __HAL_TIM_CLEAR_FLAG(&htim1, TIM_FLAG_UPDATE);       /* Clear the overflow interrupt mark. */

        if (__HAL_TIM_GET_FLAG(&htim1, TIM_FLAG_CC1) == 0)   /* No cycle capture interruption occurred and the capture was not completed */
        {
            sflag = 0;
            if (g_timxchy_pwmin_psc == 0)
            {
                g_timxchy_pwmin_psc ++;
            }
            else
            {
                if (g_timxchy_pwmin_psc == 65535)                 /* It's already Max. It's probably a no-input state */
                {
                    g_timxchy_pwmin_psc = 0;                      /* Restore the frequency */
                }
                else if (g_timxchy_pwmin_psc > 32767)             /* It's at its maximum. */
                {
                    g_timxchy_pwmin_psc = 65535;                  /* It is directly equal to the maximum frequency division coefficient. */
                }
                else
                {
                    g_timxchy_pwmin_psc += g_timxchy_pwmin_psc;   /* double */
                }
            }

            __HAL_TIM_SET_PRESCALER(&htim1, g_timxchy_pwmin_psc); /* Set the timer predivision coefficient */
            __HAL_TIM_SET_COUNTER(&htim1, 0);                     /* Clear counter */
            TIM1->SR = 0;                                   	  /* Clear all interrupt flags */
            return ;
        }
    }

    if (sflag == 0)                 /* First acquisition to capture interrupt */
    {
        if (__HAL_TIM_GET_FLAG(&htim1, TIM_FLAG_CC1))             /* The first cycle capture interruption was detected */
        {
            sflag = 1;              /* Mark that the first cycle has been captured and the second cycle capture can begin */
        }

        TIM1->SR = 0;               /* Clear all interrupt flags */
        return ;                    /* Complete the operation */
    }

    if (g_timxchy_pwmin_sta == 0)   /* No capture yet */
    {
        if (__HAL_TIM_GET_FLAG(&htim1, TIM_FLAG_CC1))         	/* A cycle capture interrupt has been detected */
        {
            g_timxchy_pwmin_hval = HAL_TIM_ReadCapturedValue(&htim1, TIM_CHANNEL_2) + 1; /* High level pulse width capture value */
            g_timxchy_pwmin_cval = HAL_TIM_ReadCapturedValue(&htim1, TIM_CHANNEL_1) + 1; /* Periodic capture value */

            if (g_timxchy_pwmin_hval < g_timxchy_pwmin_cval)    /* The high level pulse width must be smaller than the cycle length */
            {
                g_timxchy_pwmin_sta = 1;                        /* Mark capture successful */

                g_timxchy_pwmin_psc = TIM1->PSC;     		    /* Obtain the PWM input frequency division coefficients */

                if (g_timxchy_pwmin_psc == 0)                   /* When the divider coefficient is 0, correct the read data */
                {
                    g_timxchy_pwmin_hval++;                     /* The correction factor is 1, plus 1 */
                    g_timxchy_pwmin_cval++;                     /* The correction factor is 1, plus 1 */
                }

                sflag = 0;
                /* After each capture PWM input is successful,
                 * the capture is stopped to avoid frequent interrupts
                 * affecting the normal code operation of the system */
                TIM1->CR1  &= ~(1 << 0);                    	/* Turn off TIMX */
                __HAL_TIM_DISABLE_IT(&htim1, TIM_IT_CC1);     	/* Turn off channel 1 to capture the interrupt */
                __HAL_TIM_DISABLE_IT(&htim1, TIM_IT_CC2);     	/* Turn off channel 2 to capture the interrupt */
                __HAL_TIM_DISABLE_IT(&htim1, TIM_IT_UPDATE);  	/* Turn off overflow interrupts. */

                TIM1->SR = 0;                                  	/* Clear all interrupt flags. */
            }
            else
            {
                atim_timx_pwmin_chy_restart();
            }
        }
    }

    TIM1->SR = 0;                                                /* Clear all interrupt flags. */
}
/* USER CODE END 1 */
