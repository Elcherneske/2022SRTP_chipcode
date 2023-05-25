/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.h
  * @brief   This file contains all the function prototypes for
  *          the tim.c file
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
#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "common.h"
/* USER CODE END Includes */

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void TIM3_PWM_Init(uint16_t arr, uint16_t psc);
void TIM3_Init(uint16_t arr, uint16_t psc);
void MX_TIM4_Init(void);
void MX_TIM5_Init(void);

/* USER CODE BEGIN Prototypes */
/* 判断TIM3是否超时 */
extern uint32_t tim4_timeout_index;
extern uint32_t tim4_timeout_index_prev;
extern uint32_t tim5_timeout_index;
extern uint32_t tim5_timeout_index_prev;
void TIM_SetTIM3Compare4(uint32_t compare);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

