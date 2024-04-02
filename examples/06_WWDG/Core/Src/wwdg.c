/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    wwdg.c
  * @brief   This file provides code for the configuration
  *          of the WWDG instances.
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
#include "wwdg.h"

/* USER CODE BEGIN 0 */
#include "usart.h"
/* USER CODE END 0 */

WWDG_HandleTypeDef hwwdg;

/* WWDG init function */
void MX_WWDG_Init(void)
{

  /* USER CODE BEGIN WWDG_Init 0 */

  /* USER CODE END WWDG_Init 0 */

  /* USER CODE BEGIN WWDG_Init 1 */

  /* USER CODE END WWDG_Init 1 */
  hwwdg.Instance = WWDG;
  hwwdg.Init.Prescaler = WWDG_PRESCALER_8;
  hwwdg.Init.Window = 0x5F;
  hwwdg.Init.Counter = 0x7F;
  hwwdg.Init.EWIMode = WWDG_EWI_ENABLE;
  if (HAL_WWDG_Init(&hwwdg) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN WWDG_Init 2 */

  /* USER CODE END WWDG_Init 2 */

}

void HAL_WWDG_MspInit(WWDG_HandleTypeDef* wwdgHandle)
{

  if(wwdgHandle->Instance==WWDG)
  {
  /* USER CODE BEGIN WWDG_MspInit 0 */

  /* USER CODE END WWDG_MspInit 0 */
    /* WWDG clock enable */
    __HAL_RCC_WWDG_CLK_ENABLE();

    /* WWDG interrupt Init */
    HAL_NVIC_SetPriority(WWDG_IRQn, 2, 1);
    HAL_NVIC_EnableIRQ(WWDG_IRQn);
  /* USER CODE BEGIN WWDG_MspInit 1 */

  /* USER CODE END WWDG_MspInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/**
  * @brief  WWDG Early Wakeup callback.
  * @param  hwwdg  pointer to a WWDG_HandleTypeDef structure that contains
  *                the configuration information for the specified WWDG module.
  * @retval None
  */
void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef *hwwdg)
{
    HAL_WWDG_Refresh(hwwdg); 			/* Update the window watchdog value. */
    printf("wwdg success\r\n");         /* Feeding the dog successfully. */
}
/* USER CODE END 1 */
