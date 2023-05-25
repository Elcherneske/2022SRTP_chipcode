/****************************************************
 * 目的: 基本定义
 * 文件: user_config.h
 * 版本:
 * 作者: 孙坚
 * 邮件: wgetfree@qq.com
 * 创建时间:
 * 修改时间:
 ****************************************************/

#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_tim.h"
#include "stm32h7xx_hal_qspi.h"
#include "usbd_cdc.h"

#include "circular_buffer.h"

#define ARRAYSIZE(a) 				(sizeof(a) /sizeof(a[0]))
#define APP_NVIC_OFFSET 			0x8000
#define COM_BUF_SZ				    1024

//#define UNUSED(x) 				(void)(x)

/* 函数返回值 */
#define RETURN_ERROR				-1	/* 错误 */
#define RETURN_FALSE				0	/* 假 */
#define RETURN_TRUE					1	/* 真 */
#define RETURN_UNKNOWN				2	/* 未知 */

/* 参数配置存储起始地址 */
#define CONFIG_BASE_ADDR            0
/* 保持寄存器数量 */
#define REG_NUM_HOLD                128
/* 等待数据超时 */
#define IO_DATA_AVAIL_DELAY			3

/* USB端点 */
#define USB_CONFIG_MODBUS_EP_READ   CDC_OUT_EP  /* 0x01 */
#define USB_CONFIG_MODBUS_EP_WRITE  CDC_IN_EP   /* 0x81 */
#define USB_ADC_DATA_EP_READ        CDC_OUT_EP2 /* 0x02 */
#define USB_ADC_DATA_EP_WRITE       CDC_IN_EP2  /* 0x82 */

/* ADC数据帧帧头 */
#define ADC_FRAME_HEAD_1            0xA0
#define ADC_FRAME_HEAD_2            0xA1
#define ADC_FRAME_HEAD_3            0xA2
#define ADC_FRAME_HEAD_4            0xA3

/* MODBUS设备地址 */
#define MODBUS_DEV_ADDR             1

/* ADC数据传输通道启停标识 */
#define USB_ADC_DATA_TRANSFER_START 1
#define USB_ADC_DATA_TRANSFER_STOP  2

/* 保持寄存器地址 */
#define MODBUS_HOLD_DAC1_VOL        0  /* PA4, 0~3300，单位mV */
#define MODBUS_HOLD_DAC2_VOL        1  /* PA5, 0~3300，单位mV */
#define MODBUS_HOLD_PWM_PSC         2  /* PC9, 分频值PSC */
#define MODBUS_HOLD_PWM_ARR         3  /* PC9, 重装值ARR, 频率计算：如果PSC=240，则240M/240=1M的计数频率，自动重装载ARR为500，那么PWM频率为1M/500=2kHZ */
#define MODBUS_HOLD_PWM_COMPARE     4  /* PC9, 设置占空比，如果设置为ARR/2，则高低电平时长一致 */
#define MODBUS_HOLD_PGA1_GAIN       5  /* PB0输入,PC4和ADC1第4通道输出,PGA1增益倍数，只能是2/4/8/16 */
#define MODBUS_HOLD_PGA2_GAIN       6  /* PE9输入,PE7输出,PGA2增益倍数，只能是2/4/8/16 */
#define MODBUS_HOLD_ADC_TIM_PSC     7  /* ADC数据采集定时器TIM5分频值PSC */
#define MODBUS_HOLD_ADC_TIM_ARR     8  /* ADC数据采集定时器TIM5分频值ARR, 时间间隔计算：如果PSC=24000/100，ARR=10000/100, 则时间间隔100us，可以调整PSC和ARR数值，比如PSC提高10倍，则延时为1ms
                                        Tout = (arr * psc) / Ft 其中Ft为主频240MHz*/
#define MODBUS_HOLD_BEGIN			9  /* 开始运行标识符*/

#endif /* _COMMON_H_ */
