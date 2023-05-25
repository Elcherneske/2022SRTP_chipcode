/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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
#include "usart.h"
#include "common.h"

/* USER CODE BEGIN 0 */
#define RXBUFFERSIZE 2048
uint8_t aRxBuffer[RXBUFFERSIZE];//HAL库使用的串口接收缓冲
/* USER CODE END 0 */

UART_HandleTypeDef huart1;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{
	//UART 初始化设置
	huart1.Instance=USART1;					    //USART1
	huart1.Init.BaudRate=9600;				    //波特率
	huart1.Init.WordLength=UART_WORDLENGTH_8B;   //字长为8位数据格式
	huart1.Init.StopBits=UART_STOPBITS_1;	    //一个停止位
	huart1.Init.Parity=UART_PARITY_NONE;		    //无奇偶校验位
	huart1.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //无硬件流控
	huart1.Init.Mode=UART_MODE_TX_RX;		    //收发模式
	HAL_UART_Init(&huart1);					    //HAL_UART_Init()会使能UART1
	
	HAL_UART_Receive_IT(&huart1, (uint8_t *)aRxBuffer, RXBUFFERSIZE);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{
    //GPIO端口设置
	GPIO_InitTypeDef GPIO_Initure;
	
	if(uartHandle->Instance==USART1)//如果是串口1，进行串口1 MSP初始化
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			//使能GPIOA时钟
		__HAL_RCC_USART1_CLK_ENABLE();			//使能USART1时钟
	
		GPIO_Initure.Pin=GPIO_PIN_9;			//PA9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;			//上拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
		GPIO_Initure.Alternate=GPIO_AF7_USART1;	//复用为USART1
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA9

		GPIO_Initure.Pin=GPIO_PIN_10;			//PA10
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//初始化PA10
		
		HAL_NVIC_EnableIRQ(USART1_IRQn);				//使能USART1中断通道
		HAL_NVIC_SetPriority(USART1_IRQn,3,3);			//抢占优先级3，子优先级3
	}
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();

    /**USART1 GPIO Configuration
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/* 串口发送 */
void uart1_send_data(uint8_t *data, uint32_t len)
{
	HAL_UART_Transmit(&huart1, data, len, 5000);
}
void uart_debug_str(char *data)
{
    //uart1_send_data((uint8_t *)data, strlen(data));
}
/* USER CODE END 1 */
