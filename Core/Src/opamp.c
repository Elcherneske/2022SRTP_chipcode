/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    opamp.c
  * @brief   This file provides code for the configuration
  *          of the OPAMP instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "opamp.h"
#include "sensor.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

OPAMP_HandleTypeDef hopamp1;
OPAMP_HandleTypeDef hopamp2;

/* OPAMP1 init function */
void MX_OPAMP1_Init(void)
{

  /* USER CODE BEGIN OPAMP1_Init 0 */
  if (hopamp1.Instance == OPAMP1) {
      HAL_OPAMP_Stop(&hopamp1);
  }
  /* USER CODE END OPAMP1_Init 0 */

  /* USER CODE BEGIN OPAMP1_Init 1 */

  /* USER CODE END OPAMP1_Init 1 */
  hopamp1.Instance = OPAMP1;
  hopamp1.Init.Mode = OPAMP_PGA_MODE;
  hopamp1.Init.NonInvertingInput = OPAMP_NONINVERTINGINPUT_IO0;
  hopamp1.Init.PowerMode = OPAMP_POWERMODE_NORMAL;
  hopamp1.Init.PgaConnect = OPAMP_PGA_CONNECT_INVERTINGINPUT_NO;
  
  if (modbus_reg[MODBUS_HOLD_PGA1_GAIN] == 2)
    hopamp1.Init.PgaGain = OPAMP_PGA_GAIN_2_OR_MINUS_1;
  else if (modbus_reg[MODBUS_HOLD_PGA1_GAIN] == 4)
    hopamp1.Init.PgaGain = OPAMP_PGA_GAIN_4_OR_MINUS_3;
  else if (modbus_reg[MODBUS_HOLD_PGA1_GAIN] == 8)
    hopamp1.Init.PgaGain = OPAMP_PGA_GAIN_8_OR_MINUS_7; 
  else if (modbus_reg[MODBUS_HOLD_PGA1_GAIN] == 16)
    hopamp1.Init.PgaGain = OPAMP_PGA_GAIN_16_OR_MINUS_15;   
  
  hopamp1.Init.UserTrimming = OPAMP_TRIMMING_FACTORY;
  if (HAL_OPAMP_Init(&hopamp1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN OPAMP1_Init 2 */
    HAL_OPAMP_Start(&hopamp1);
  /* USER CODE END OPAMP1_Init 2 */

}
/* OPAMP2 init function */
void MX_OPAMP2_Init(void)
{

  /* USER CODE BEGIN OPAMP2_Init 0 */
  if (hopamp2.Instance == OPAMP2) {
      HAL_OPAMP_Stop(&hopamp2);
  }
  /* USER CODE END OPAMP2_Init 0 */

  /* USER CODE BEGIN OPAMP2_Init 1 */

  /* USER CODE END OPAMP2_Init 1 */
  hopamp2.Instance = OPAMP2;
  hopamp2.Init.Mode = OPAMP_PGA_MODE;
  hopamp2.Init.NonInvertingInput = OPAMP_NONINVERTINGINPUT_IO0;
  hopamp2.Init.PowerMode = OPAMP_POWERMODE_NORMAL;
  hopamp2.Init.PgaConnect = OPAMP_PGA_CONNECT_INVERTINGINPUT_NO;
  
  if (modbus_reg[MODBUS_HOLD_PGA2_GAIN] == 2)
    hopamp2.Init.PgaGain = OPAMP_PGA_GAIN_2_OR_MINUS_1;
  else if (modbus_reg[MODBUS_HOLD_PGA2_GAIN] == 4)
    hopamp2.Init.PgaGain = OPAMP_PGA_GAIN_4_OR_MINUS_3;
  else if (modbus_reg[MODBUS_HOLD_PGA2_GAIN] == 8)
    hopamp2.Init.PgaGain = OPAMP_PGA_GAIN_8_OR_MINUS_7; 
  else if (modbus_reg[MODBUS_HOLD_PGA2_GAIN] == 16)
    hopamp2.Init.PgaGain = OPAMP_PGA_GAIN_16_OR_MINUS_15;  
  
  hopamp2.Init.UserTrimming = OPAMP_TRIMMING_FACTORY;
  if (HAL_OPAMP_Init(&hopamp2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN OPAMP2_Init 2 */
    HAL_OPAMP_Start(&hopamp2);
  /* USER CODE END OPAMP2_Init 2 */

}

static uint32_t HAL_RCC_OPAMP_CLK_ENABLED=0;

void HAL_OPAMP_MspInit(OPAMP_HandleTypeDef* opampHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(opampHandle->Instance==OPAMP1)
  {
  /* USER CODE BEGIN OPAMP1_MspInit 0 */

  /* USER CODE END OPAMP1_MspInit 0 */
    /* OPAMP1 clock enable */
    HAL_RCC_OPAMP_CLK_ENABLED++;
    if(HAL_RCC_OPAMP_CLK_ENABLED==1){
      __HAL_RCC_OPAMP_CLK_ENABLE();
    }

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**OPAMP1 GPIO Configuration
    PC4     ------> OPAMP1_VOUT
    PC5     ------> OPAMP1_VINM
    PB0     ------> OPAMP1_VINP
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN OPAMP1_MspInit 1 */

  /* USER CODE END OPAMP1_MspInit 1 */
  }
  else if(opampHandle->Instance==OPAMP2)
  {
  /* USER CODE BEGIN OPAMP2_MspInit 0 */

  /* USER CODE END OPAMP2_MspInit 0 */
    /* OPAMP2 clock enable */
    HAL_RCC_OPAMP_CLK_ENABLED++;
    if(HAL_RCC_OPAMP_CLK_ENABLED==1){
      __HAL_RCC_OPAMP_CLK_ENABLE();
    }

    __HAL_RCC_GPIOE_CLK_ENABLE();
    /**OPAMP2 GPIO Configuration
    PE7     ------> OPAMP2_VOUT
    PE8     ------> OPAMP2_VINM
    PE9     ------> OPAMP2_VINP
    */
    GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* USER CODE BEGIN OPAMP2_MspInit 1 */

  /* USER CODE END OPAMP2_MspInit 1 */
  }
}

void HAL_OPAMP_MspDeInit(OPAMP_HandleTypeDef* opampHandle)
{

  if(opampHandle->Instance==OPAMP1)
  {
  /* USER CODE BEGIN OPAMP1_MspDeInit 0 */

  /* USER CODE END OPAMP1_MspDeInit 0 */
    /* Peripheral clock disable */
    HAL_RCC_OPAMP_CLK_ENABLED--;
    if(HAL_RCC_OPAMP_CLK_ENABLED==0){
      __HAL_RCC_OPAMP_CLK_DISABLE();
    }

    /**OPAMP1 GPIO Configuration
    PC4     ------> OPAMP1_VOUT
    PC5     ------> OPAMP1_VINM
    PB0     ------> OPAMP1_VINP
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_4|GPIO_PIN_5);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_0);

  /* USER CODE BEGIN OPAMP1_MspDeInit 1 */

  /* USER CODE END OPAMP1_MspDeInit 1 */
  }
  else if(opampHandle->Instance==OPAMP2)
  {
  /* USER CODE BEGIN OPAMP2_MspDeInit 0 */

  /* USER CODE END OPAMP2_MspDeInit 0 */
    /* Peripheral clock disable */
    HAL_RCC_OPAMP_CLK_ENABLED--;
    if(HAL_RCC_OPAMP_CLK_ENABLED==0){
      __HAL_RCC_OPAMP_CLK_DISABLE();
    }

    /**OPAMP2 GPIO Configuration
    PE7     ------> OPAMP2_VOUT
    PE8     ------> OPAMP2_VINM
    PE9     ------> OPAMP2_VINP
    */
    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9);

  /* USER CODE BEGIN OPAMP2_MspDeInit 1 */

  /* USER CODE END OPAMP2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
