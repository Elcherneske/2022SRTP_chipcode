/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.c
  * @brief   This file provides code for the configuration
  *          of the TIM instances.
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
#include "tim.h"
#include "io_data_deal.h"
#include "sensor.h"

/* USER CODE BEGIN 0 */
/* 判断TIM4是否超时 */
uint32_t tim4_timeout_index = 0;
uint32_t tim4_timeout_index_prev = 0;
/* 判断TIM5是否超时 */
uint32_t tim5_timeout_index = 0;
uint32_t tim5_timeout_index_prev = 0;
/* USER CODE END 0 */

TIM_HandleTypeDef htim3;
TIM_OC_InitTypeDef TIM3_CH4Handler;     //定时器3通道4句柄
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim5;

//通用定时器3中断初始化,定时器3在APB1上，APB1的定时器时钟为200MHz
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!(定时器3挂在APB1上，时钟为HCLK/2)
void TIM3_Init(uint16_t arr, uint16_t psc)
{  
    htim3.Instance=TIM3;                          //通用定时器3
    htim3.Init.Prescaler=psc;                     //分频
    htim3.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    htim3.Init.Period=arr;                        //自动装载值
    htim3.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&htim3);
    
    HAL_TIM_Base_Start_IT(&htim3); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE    
}

//TIM3 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM3_PWM_Init(uint16_t arr, uint16_t psc)
{ 
    if (htim3.Instance==TIM3) {
        HAL_TIM_PWM_Stop(&htim3,TIM_CHANNEL_4);
        HAL_TIM_PWM_DeInit(&htim3);
    }
    
    htim3.Instance=TIM3;            //定时器3
    htim3.Init.Prescaler=psc;       //定时器分频
    htim3.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
    htim3.Init.Period=arr;          //自动重装载值
    htim3.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&htim3);       //初始化PWM
    
    TIM3_CH4Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM3_CH4Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，
                                            //默认比较值为自动重装载值的一半,即占空比为50%
    TIM3_CH4Handler.OCPolarity=TIM_OCPOLARITY_LOW; //输出比较极性为低 
    HAL_TIM_PWM_ConfigChannel(&htim3,&TIM3_CH4Handler,TIM_CHANNEL_4);//配置TIM3通道4
    HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_4);//开启PWM通道4
}

//定时器底层驱动，时钟使能，引脚配置
//此函数会被HAL_TIM_PWM_Init()调用
//htim:定时器句柄
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_TIM3_CLK_ENABLE();			//使能定时器3
    __HAL_RCC_GPIOC_CLK_ENABLE();			//开启GPIOC时钟
	
    GPIO_Initure.Pin=GPIO_PIN_9;           	//PC9
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推完输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;     //高速
	GPIO_Initure.Alternate=GPIO_AF2_TIM3;	//PC9复用为TIM3_CH4
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspInit 0 */

  /* USER CODE END TIM3_MspInit 0 */
    /* TIM3 clock enable */
    __HAL_RCC_TIM3_CLK_ENABLE();
  /* USER CODE BEGIN TIM3_MspInit 1 */
    HAL_NVIC_SetPriority(TIM3_IRQn,1,3);    //设置中断优先级，抢占优先级1，子优先级3
    HAL_NVIC_EnableIRQ(TIM3_IRQn);          //开启ITM3中断   
  /* USER CODE END TIM3_MspInit 1 */
  } else if(tim_baseHandle->Instance==TIM4)
  {
  /* USER CODE BEGIN TIM3_MspInit 0 */

  /* USER CODE END TIM3_MspInit 0 */
    /* TIM3 clock enable */
    __HAL_RCC_TIM4_CLK_ENABLE();
  /* USER CODE BEGIN TIM3_MspInit 1 */
    HAL_NVIC_SetPriority(TIM4_IRQn,1,3);    //设置中断优先级，抢占优先级1，子优先级3
    HAL_NVIC_EnableIRQ(TIM4_IRQn);          //开启ITM3中断   
  /* USER CODE END TIM3_MspInit 1 */
  } else if(tim_baseHandle->Instance==TIM5)
  {
  /* USER CODE BEGIN TIM3_MspInit 0 */

  /* USER CODE END TIM3_MspInit 0 */
    /* TIM3 clock enable */
    __HAL_RCC_TIM5_CLK_ENABLE();
  /* USER CODE BEGIN TIM3_MspInit 1 */
    HAL_NVIC_SetPriority(TIM5_IRQn,0,0);    //设置中断优先级，抢占优先级1，子优先级3
    HAL_NVIC_EnableIRQ(TIM5_IRQn);          //开启ITM3中断   
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
  /* USER CODE BEGIN TIM3_MspDeInit 1 */

  /* USER CODE END TIM3_MspDeInit 1 */
  } else if(tim_baseHandle->Instance==TIM4)
  {
  /* USER CODE BEGIN TIM3_MspDeInit 0 */

  /* USER CODE END TIM3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM4_CLK_DISABLE();
  /* USER CODE BEGIN TIM3_MspDeInit 1 */

  /* USER CODE END TIM3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim3);
}

//定时器3中断服务函数调用
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if(htim==(&htim3))
    {
    } else if(htim==(&htim4)) {
        tim4_timeout_index++;
    } else if(htim==(&htim5)) {
        tim5_timeout_index++;
        /* 存储高速ADC数据 */
        adc_data_store_quick();
    }
}

//设置TIM通道4的占空比
//compare:比较值
void TIM_SetTIM3Compare4(uint32_t compare)
{
	TIM3->CCR4=compare; 
}

/* TIM4 init function */
void MX_TIM4_Init(void)
{
    htim4.Instance=TIM4;                          //通用定时器4
    htim4.Init.Prescaler=24000-1;                 //分频
    htim4.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    htim4.Init.Period=10000/10-1;                 //自动装载值，延时100ms
    htim4.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&htim4);
    
    HAL_TIM_Base_Start_IT(&htim4); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE   
}

/* USER CODE BEGIN 1 */
//定时器4中断服务函数
void TIM4_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim4);
}

/* TIM5 init function */
void MX_TIM5_Init(void)
{
    if (htim5.Instance == TIM5) {
        HAL_TIM_Base_Stop_IT(&htim5);
    }
    
    htim5.Instance=TIM5;                          //通用定时器5
    htim5.Init.Prescaler= modbus_reg[MODBUS_HOLD_ADC_TIM_PSC] -1;             //分频
    //htim5.Init.Prescaler= 240 -1;
    htim5.Init.CounterMode=TIM_COUNTERMODE_UP;    //向上计数器
    htim5.Init.Period = modbus_reg[MODBUS_HOLD_ADC_TIM_ARR] - 1;                //自动装载值，延时1秒
   // htim5.Init.Period = 100 - 1;
    htim5.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;//时钟分频因子
    HAL_TIM_Base_Init(&htim5);
    
    HAL_TIM_Base_Start_IT(&htim5); //使能定时器3和定时器3更新中断：TIM_IT_UPDATE   
}

//定时器5中断服务函数
void TIM5_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&htim5);
}

/* USER CODE END 1 */
