/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    quadspi.c
  * @brief   This file provides code for the configuration
  *          of the QUADSPI instances.
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
#include "quadspi.h"

/* USER CODE BEGIN 0 */
//等待状态标志
//flag:需要等待的标志位
//sta:需要等待的状态
//wtime:等待时间
//返回值:0,等待成功.
//	     1,等待失败.
uint8_t QSPI_Wait_Flag(uint32_t flag, uint8_t sta, uint32_t wtime)
{
	uint8_t flagsta=0;
	while(wtime) {
		flagsta=(QUADSPI->SR&flag)?1:0; 
		if(flagsta==sta)break;
		wtime--;
	}
	if(wtime)return 0;
	else return 1;
}
/* USER CODE END 0 */

QSPI_HandleTypeDef hqspi;

/* QUADSPI init function */
void MX_QUADSPI_Init(void)
{

  /* USER CODE BEGIN QUADSPI_Init 0 */

  /* USER CODE END QUADSPI_Init 0 */

  /* USER CODE BEGIN QUADSPI_Init 1 */

  /* USER CODE END QUADSPI_Init 1 */
  hqspi.Instance = QUADSPI;
  hqspi.Init.ClockPrescaler = 1;
  hqspi.Init.FifoThreshold = 4;
  hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_HALFCYCLE;
  hqspi.Init.FlashSize = POSITION_VAL(0x800000)-1;
  hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_5_CYCLE;
  hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
  hqspi.Init.FlashID = QSPI_FLASH_ID_1;
  hqspi.Init.DualFlash = QSPI_DUALFLASH_DISABLE;
  if (HAL_QSPI_Init(&hqspi) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN QUADSPI_Init 2 */

  /* USER CODE END QUADSPI_Init 2 */

}

void HAL_QSPI_MspInit(QSPI_HandleTypeDef* qspiHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
  if(qspiHandle->Instance==QUADSPI)
  {
  /* USER CODE BEGIN QUADSPI_MspInit 0 */

  /* USER CODE END QUADSPI_MspInit 0 */
  /** Initializes the peripherals clock
  */
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_QSPI;
    PeriphClkInitStruct.QspiClockSelection = RCC_QSPICLKSOURCE_D1HCLK;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    /* QUADSPI clock enable */
    __HAL_RCC_QSPI_CLK_ENABLE();

    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    /**QUADSPI GPIO Configuration
    PE2     ------> QUADSPI_BK1_IO2
    PB2     ------> QUADSPI_CLK
    PD11     ------> QUADSPI_BK1_IO0
    PD12     ------> QUADSPI_BK1_IO1
    PD13     ------> QUADSPI_BK1_IO3
    PB6     ------> QUADSPI_BK1_NCS
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN QUADSPI_MspInit 1 */

  /* USER CODE END QUADSPI_MspInit 1 */
  }
}

void HAL_QSPI_MspDeInit(QSPI_HandleTypeDef* qspiHandle)
{

  if(qspiHandle->Instance==QUADSPI)
  {
  /* USER CODE BEGIN QUADSPI_MspDeInit 0 */

  /* USER CODE END QUADSPI_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_QSPI_CLK_DISABLE();

    /**QUADSPI GPIO Configuration
    PE2     ------> QUADSPI_BK1_IO2
    PB2     ------> QUADSPI_CLK
    PD11     ------> QUADSPI_BK1_IO0
    PD12     ------> QUADSPI_BK1_IO1
    PD13     ------> QUADSPI_BK1_IO3
    PB6     ------> QUADSPI_BK1_NCS
    */
    HAL_GPIO_DeInit(GPIOE, GPIO_PIN_2);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_2|GPIO_PIN_6);

    HAL_GPIO_DeInit(GPIOD, GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13);

  /* USER CODE BEGIN QUADSPI_MspDeInit 1 */

  /* USER CODE END QUADSPI_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
//QSPI发送命令
//cmd:要发送的指令
//addr:发送到的目的地址
//mode:模式,详细位定义如下:
//	mode[1:0]:指令模式;00,无指令;01,单线传输指令;10,双线传输指令;11,四线传输指令.
//	mode[3:2]:地址模式;00,无地址;01,单线传输地址;10,双线传输地址;11,四线传输地址.
//	mode[5:4]:地址长度;00,8位地址;01,16位地址;10,24位地址;11,32位地址.
//	mode[7:6]:数据模式;00,无数据;01,单线传输数据;10,双线传输数据;11,四线传输数据.
//dmcycle:空指令周期数
void QSPI_Send_CMD(uint8_t cmd,uint32_t addr,uint8_t mode,uint8_t dmcycle)
{
	QSPI_CommandTypeDef Cmdhandler;
    
	Cmdhandler.Instruction=cmd;									//指令
	Cmdhandler.Address=addr;									//地址
	Cmdhandler.DummyCycles=dmcycle;								//设置空指令周期数
	
	if(((mode>>0)&0x03) == 0)
	Cmdhandler.InstructionMode=QSPI_INSTRUCTION_NONE;			//指令模式
	else if(((mode>>0)&0x03) == 1)
	Cmdhandler.InstructionMode=QSPI_INSTRUCTION_1_LINE;			//指令模式
	else if(((mode>>0)&0x03) == 2)
	Cmdhandler.InstructionMode=QSPI_INSTRUCTION_2_LINES;			//指令模式
	else if(((mode>>0)&0x03) == 3)
	Cmdhandler.InstructionMode=QSPI_INSTRUCTION_4_LINES;			//指令模式
	
	if(((mode>>2)&0x03) == 0)
	Cmdhandler.AddressMode=QSPI_ADDRESS_NONE;   					//地址模式
	else if(((mode>>2)&0x03) == 1)
	Cmdhandler.AddressMode=QSPI_ADDRESS_1_LINE;   					//地址模式
	else if(((mode>>2)&0x03) == 2)
	Cmdhandler.AddressMode=QSPI_ADDRESS_2_LINES;   					//地址模式
	else if(((mode>>2)&0x03) == 3)
	Cmdhandler.AddressMode=QSPI_ADDRESS_4_LINES;   					//地址模式
	
	if(((mode>>4)&0x03) == 0)
	Cmdhandler.AddressSize=QSPI_ADDRESS_8_BITS;   					//地址长度
	else if(((mode>>4)&0x03) == 1)
	Cmdhandler.AddressSize=QSPI_ADDRESS_16_BITS;   					//地址长度
	else if(((mode>>4)&0x03) == 2)
	Cmdhandler.AddressSize=QSPI_ADDRESS_24_BITS;   					//地址长度
	else if(((mode>>4)&0x03) == 3)
	Cmdhandler.AddressSize=QSPI_ADDRESS_32_BITS;   					//地址长度
	
	if(((mode>>6)&0x03) == 0)
	Cmdhandler.DataMode=QSPI_DATA_NONE;             			//数据模式
	else if(((mode>>6)&0x03) == 1)
	Cmdhandler.DataMode=QSPI_DATA_1_LINE;             			//数据模式
	else if(((mode>>6)&0x03) == 2)
	Cmdhandler.DataMode=QSPI_DATA_2_LINES;             			//数据模式
	else if(((mode>>6)&0x03) == 3)
	Cmdhandler.DataMode=QSPI_DATA_4_LINES;             			//数据模式
	
	Cmdhandler.SIOOMode=QSPI_SIOO_INST_EVERY_CMD;				//每次都发送指令
	Cmdhandler.AlternateByteMode=QSPI_ALTERNATE_BYTES_NONE;		//无交替字节
	Cmdhandler.DdrMode=QSPI_DDR_MODE_DISABLE;					//关闭DDR模式
	Cmdhandler.DdrHoldHalfCycle=QSPI_DDR_HHC_ANALOG_DELAY;

	HAL_QSPI_Command(&hqspi,&Cmdhandler,5000);
}

//QSPI接收指定长度的数据
//buf:接收数据缓冲区首地址
//datalen:要传输的数据长度
//返回值:0,正常
//    其他,错误代码
uint8_t QSPI_Receive(uint8_t* buf,uint32_t datalen)
{
    hqspi.Instance->DLR=datalen-1;                           //配置数据长度
    if(HAL_QSPI_Receive(&hqspi,buf,5000)==HAL_OK) return 0;  //接收数据
    else return 1;
}

//QSPI发送指定长度的数据
//buf:发送数据缓冲区首地址
//datalen:要传输的数据长度
//返回值:0,正常
//    其他,错误代码
uint8_t QSPI_Transmit(uint8_t* buf,uint32_t datalen)
{
    hqspi.Instance->DLR=datalen-1;                            //配置数据长度
    if(HAL_QSPI_Transmit(&hqspi,buf,5000)==HAL_OK) return 0;  //发送数据
    else return 1;
}
/* USER CODE END 1 */
