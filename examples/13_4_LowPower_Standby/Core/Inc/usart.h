/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "stdio.h"	/* Serial port redirection is used in */
/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

/* USER CODE BEGIN Private defines */
#define USART_REC_LEN               200         /* The maximum number of bytes received is defined as 200 */
#define RXBUFFERSIZE   				1        	/* cache size */

extern uint8_t  g_usart_rx_buf[USART_REC_LEN];  /* Receive buffer, maximum USART_REC_LEN bytes. The last byte is a newline character */
extern uint16_t g_usart_rx_sta;                 /* Receiving status marking */
extern uint8_t g_rx_buffer[RXBUFFERSIZE];       /* The HAL library USART receives buffers */
/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

