/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "iwdg.h"
#include "opamp.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"
#include "tim.h"
#include "quadspi.h"
#include "sensor.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "delay.h"
#include "io_data_deal.h"
#include "norflash.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */

int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

/* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC1_Init();
  MX_ADC3_Init();
  //MX_DMA_Init();
  MX_USB_DEVICE_Init();
  MX_DAC1_Init();
  MX_USART1_UART_Init();
  MX_IWDG1_Init();
  MX_TIM4_Init();
  MX_QUADSPI_Init();
  /* USER CODE BEGIN 2 */
  /* 延时函数初始化 */
  delay_init(480);
  /* 初始化SPI FLASH */
  NORFLASH_Init();
  if (NORFLASH_ReadID()==W25Q64) {
      //uart_debug_str("NorFlash Check OK\r\n");
  }
  /* 缓冲区初始化 */
  sys_buf_init();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  //uart_debug_str("Program Start(20220403)\r\n");
  MX_OPAMP1_Init();
  MX_OPAMP2_Init();
  /* 输出当前ADC高速采样周期,单位us */
  uint32_t adc_interval = modbus_reg[MODBUS_HOLD_ADC_TIM_PSC] * modbus_reg[MODBUS_HOLD_ADC_TIM_ARR] / 240;
  char adc_interval_str[128] = {0};
  sprintf(adc_interval_str, "adc_interval=%d us\r\n", adc_interval);
  //uart_debug_str(adc_interval_str);
  
  DAC_CH1_Set_Vol(modbus_reg[MODBUS_HOLD_DAC1_VOL]);
  DAC_CH2_Set_Vol(modbus_reg[MODBUS_HOLD_DAC2_VOL]);
  TIM3_PWM_Init(modbus_reg[MODBUS_HOLD_PWM_ARR] - 1, modbus_reg[MODBUS_HOLD_PWM_PSC] - 1);		//240M/240=1M的计数频率，自动重装载为500，那么PWM频率为1M/500=2kHZ
  TIM_SetTIM3Compare4(modbus_reg[MODBUS_HOLD_PWM_COMPARE]);
  MX_TIM5_Init();
  
  /* 开启使用内部电压基准，基准电压2.5V */
//  HAL_SYSCFG_VREFBUF_VoltageScalingConfig(SYSCFG_VREFBUF_VOLTAGE_SCALE0);
//  HAL_SYSCFG_EnableVREFBUF();
//  HAL_SYSCFG_VREFBUF_HighImpedanceConfig(SYSCFG_VREFBUF_HIGH_IMPEDANCE_DISABLE);
  
//  /* 使用外部参考电压 */
//  HAL_SYSCFG_DisableVREFBUF();
//  HAL_SYSCFG_VREFBUF_HighImpedanceConfig(SYSCFG_VREFBUF_HIGH_IMPEDANCE_ENABLE);
  
  int32_t data_deal_timeout = 0;
  extern uint16_t modbus_reg[REG_NUM_HOLD];
  while (1)
  {
	if(modbus_reg[MODBUS_HOLD_BEGIN] == 1){
		if (tim5_timeout_index > tim5_timeout_index_prev ) { /* 每隔100微秒进入函数 */
		        tim5_timeout_index_prev = tim5_timeout_index;
		        /* 推送ADC数据 */
		        sensor_data_usb_send();
		        //uart1_send_data("cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc", 64);

		}

		if (tim4_timeout_index > tim4_timeout_index_prev) { /* 每隔500ms进入函数 */
		    tim4_timeout_index_prev = tim4_timeout_index;
		    //uart1_send_data("bbbbbb", 6);
		        /* 处理接口数据 */
		    io_data_deal();

		         /* 传感器数据采集 */
		    sensor_data_get();


		    data_deal_timeout++;
		    if (data_deal_timeout >= 10) {
		        data_deal_timeout = 0;
		            /* 喂独立看门狗 */
		        IWDG_Feed();
		            /* 检查接口数据状态，每隔一段时间，重置解析流程 */
		        io_data_avail_check();
		            /* 状态输出 */
		        //sensor_data_print();
		    }
		}
	}
	else{

		if (tim5_timeout_index > tim5_timeout_index_prev) { /* 每隔100微秒进入函数 */
			tim5_timeout_index_prev = tim5_timeout_index;
		}
		if (tim4_timeout_index > tim4_timeout_index_prev) { /* 每隔100ms进入函数 */
			tim4_timeout_index_prev = tim4_timeout_index;
					        /* 处理接口数据 */

			io_data_deal();
			data_deal_timeout++;
			if (data_deal_timeout >= 10) {
				data_deal_timeout = 0;
				//uart1_send_data("aaa", 3);
				            /* 喂独立看门狗 */
				IWDG_Feed();

			}
		}
	}




      
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** Macro to configure the PLL clock source
  */
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 20;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInitStruct.PLL2.PLL2M = 25;
  PeriphClkInitStruct.PLL2.PLL2N = 160;
  PeriphClkInitStruct.PLL2.PLL2P = 4;
  PeriphClkInitStruct.PLL2.PLL2Q = 2;
  PeriphClkInitStruct.PLL2.PLL2R = 2;
  PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_0;
  PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOMEDIUM;
  PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
  PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

