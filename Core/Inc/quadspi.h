/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    quadspi.h
  * @brief   This file contains all the function prototypes for
  *          the quadspi.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __QUADSPI_H__
#define __QUADSPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "common.h"
/* USER CODE END Includes */

extern QSPI_HandleTypeDef hqspi;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_QUADSPI_Init(void);

/* USER CODE BEGIN Prototypes */
void QSPI_Send_CMD(uint8_t cmd,uint32_t addr,uint8_t mode,uint8_t dmcycle);
uint8_t QSPI_Receive(uint8_t* buf,uint32_t datalen);
uint8_t QSPI_Transmit(uint8_t* buf,uint32_t datalen);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __QUADSPI_H__ */

