/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpdma.c
  * @brief   This file provides code for the configuration
  *          of the GPDMA instances.
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
#include "gpdma.h"

/* USER CODE BEGIN 0 */
#include "adc.h"

DMA_NodeTypeDef gpdma1_ch1_node;
DMA_QListTypeDef gpdma1_ch1_qlist;
extern ADC_HandleTypeDef hadc1;
/* USER CODE END 0 */

DMA_HandleTypeDef handle_GPDMA1_Channel1;

/* GPDMA1 init function */
void MX_GPDMA1_Init(void)
{

  /* USER CODE BEGIN GPDMA1_Init 0 */
  DMA_NodeConfTypeDef node_conf = {0};
  /* USER CODE END GPDMA1_Init 0 */

  /* Peripheral clock enable */
  __HAL_RCC_GPDMA1_CLK_ENABLE();

  /* GPDMA1 interrupt Init */
    HAL_NVIC_SetPriority(GPDMA1_Channel1_IRQn, 2, 1);
    HAL_NVIC_EnableIRQ(GPDMA1_Channel1_IRQn);

  /* USER CODE BEGIN GPDMA1_Init 1 */

  /* USER CODE END GPDMA1_Init 1 */
  handle_GPDMA1_Channel1.Instance = GPDMA1_Channel1;
  handle_GPDMA1_Channel1.InitLinkedList.Priority = DMA_LOW_PRIORITY_LOW_WEIGHT;
  handle_GPDMA1_Channel1.InitLinkedList.LinkStepMode = DMA_LSM_FULL_EXECUTION;
  handle_GPDMA1_Channel1.InitLinkedList.LinkAllocatedPort = DMA_LINK_ALLOCATED_PORT1;
  handle_GPDMA1_Channel1.InitLinkedList.TransferEventMode = DMA_TCEM_EACH_LL_ITEM_TRANSFER;
  handle_GPDMA1_Channel1.InitLinkedList.LinkedListMode = DMA_LINKEDLIST_CIRCULAR;
  if (HAL_DMAEx_List_Init(&handle_GPDMA1_Channel1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DMA_ConfigChannelAttributes(&handle_GPDMA1_Channel1, DMA_CHANNEL_NPRIV) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN GPDMA1_Init 2 */
  /* ADC1 DMA initialization */
  node_conf.NodeType = DMA_GPDMA_LINEAR_NODE;                                              /* GPDMA linear addressable node type */
  node_conf.Init.Request = GPDMA1_REQUEST_ADC1;                                            /* DMA channel request */
  node_conf.Init.BlkHWRequest = DMA_BREQ_SINGLE_BURST;                                     /* Single/burst level hardware request protocol */
  node_conf.Init.Direction = DMA_PERIPH_TO_MEMORY;                                         /* DMA channel transfer direction: peripheral to memory */
  node_conf.Init.SrcInc = DMA_SINC_FIXED;                                                  /* Source incremental mode: fixed mode single/burst */
  node_conf.Init.DestInc = DMA_DINC_INCREMENTED;                                           /* Target incremental pattern: Incremental pattern single/burst */
  node_conf.Init.SrcDataWidth = DMA_SRC_DATAWIDTH_HALFWORD;                                /* Width of source data: 16 bits */
  node_conf.Init.DestDataWidth = DMA_DEST_DATAWIDTH_HALFWORD;                              /* Target data width: 16 bits */
  node_conf.Init.SrcBurstLength = 1;                                                       /* Specifies the source burst length of the DMA, ranging from 1 to 64 */
  node_conf.Init.DestBurstLength = 1;                                                      /* Specified target burst length, range: 1 to 64 */
  node_conf.Init.TransferAllocatedPort = DMA_SRC_ALLOCATED_PORT1|DMA_DEST_ALLOCATED_PORT1; /* Specifies the port to which the transmission is assigned */
  node_conf.Init.TransferEventMode = DMA_TCEM_BLOCK_TRANSFER;                              /* Specifies the transfer event mode for the DMA channel */
  node_conf.Init.Mode = DMA_NORMAL;                                                        /* DMA mode: normal mode transmission */
  node_conf.TriggerConfig.TriggerPolarity = DMA_TRIG_POLARITY_MASKED;                      /* Set the DMA channel trigger polarity */
  node_conf.DataHandlingConfig.DataExchange = DMA_EXCHANGE_NONE;                           /* DMA channel data exchange mode: no data exchange */
  node_conf.DataHandlingConfig.DataAlignment = DMA_DATA_RIGHTALIGN_ZEROPADDED;             /* If the source data width < target data width => is right-aligned, fill 0 to the target data width */

  HAL_DMAEx_List_BuildNode(&node_conf, &gpdma1_ch1_node);
  HAL_DMAEx_List_InsertNode(&gpdma1_ch1_qlist, NULL, &gpdma1_ch1_node);                    /* Inserting new nodes */
  HAL_DMAEx_List_SetCircularMode(&gpdma1_ch1_qlist);                                       /* Sets the circular mode of the list queue */
  HAL_DMAEx_List_LinkQ(&handle_GPDMA1_Channel1, &gpdma1_ch1_qlist);                        /* Linked list queues to DMA channels */

  __HAL_LINKDMA(&hadc1, DMA_Handle, handle_GPDMA1_Channel1);                               /* Associate the DMA handle with the ADC handle */
  /* USER CODE END GPDMA1_Init 2 */

}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
