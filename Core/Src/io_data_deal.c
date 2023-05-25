/****************************************************
 * 目的: 接口数据处理
 * 用法:
 * 文件:
 * 版本:
 * 作者: 孙坚
 * 邮箱: wgetfree@qq.com
 * 创建时间:
 * 修改时间:
 ****************************************************/
 
#include "io_data_deal.h"
#include "usart.h"
#include "adc.h"
#include "dac.h"
#include "sensor.h"
#include "norflash.h"
#include "protocol_modbus_slave.h"
#include "tim.h"
#include "opamp.h"
#include "iwdg.h"
#include "delay.h"

/* 接口数据缓冲区 */
BUF_RING_8 ring_buf_debug;
BUF_RING_8 ring_buf_modbus_usb;
BUF_RING_8 ring_buf_adc_usb;
BUF_RING_8_LARGE ring_buf_adc_quick_store;
MODBUS_FRAME_ANALYSIS_DATA analysis_data_modbus_rtu_usb;
/* ADC缓冲区 */
extern uint16_t ADCxValues[16];
/* 参数配置数据 */
uint16_t modbus_reg[REG_NUM_HOLD];
/* USB发送数据 */
uint8_t CDC_Transmit_FS(uint8_t epnum, uint8_t* Buf, uint16_t Len);
/* 标识是否启动ADC数据传输 */
int8_t sig_usb_adc_data_transfer = RETURN_UNKNOWN;

/* 在指定地址开始读出指定个数的数据 */
void config_read(uint32_t ReadAddr, uint8_t *pBuffer, uint32_t NumToRead)
{
    NORFLASH_Read(pBuffer, ReadAddr, NumToRead);
}

/* 在指定地址开始写入指定个数的数据 */
void config_write(uint32_t WriteAddr, uint8_t *pBuffer, uint32_t NumToWrite)
{
    NORFLASH_Write(pBuffer, WriteAddr, NumToWrite);
}

/* 参数初始化 */
void config_init(void)
{
	int32_t ret = RETURN_UNKNOWN;
	memset(modbus_reg, 0, sizeof(uint16_t) * REG_NUM_HOLD);
	config_read(CONFIG_BASE_ADDR, (uint8_t *)modbus_reg, sizeof(uint16_t) * REG_NUM_HOLD);
    
    if (modbus_reg[MODBUS_HOLD_DAC1_VOL] > 3300) {
        modbus_reg[MODBUS_HOLD_DAC1_VOL] = 100;
        ret = RETURN_TRUE;
    }
    if (modbus_reg[MODBUS_HOLD_DAC2_VOL] > 3300) {
        modbus_reg[MODBUS_HOLD_DAC2_VOL] = 100;
        ret = RETURN_TRUE;
    } 
    
    if (modbus_reg[MODBUS_HOLD_PWM_PSC] == 0 || modbus_reg[MODBUS_HOLD_PWM_PSC] == 0xFFFF) {
        modbus_reg[MODBUS_HOLD_PWM_PSC] = 240;
        ret = RETURN_TRUE;
    }
    
    if (modbus_reg[MODBUS_HOLD_PWM_ARR] == 0 || modbus_reg[MODBUS_HOLD_PWM_ARR] == 0xFFFF) {
        modbus_reg[MODBUS_HOLD_PWM_ARR] = 500;
        ret = RETURN_TRUE;
    }
    
    if (modbus_reg[MODBUS_HOLD_PWM_COMPARE] == 0 || modbus_reg[MODBUS_HOLD_PWM_COMPARE] == 0xFFFF) {
        modbus_reg[MODBUS_HOLD_PWM_COMPARE] = modbus_reg[MODBUS_HOLD_PWM_ARR] / 2;
        ret = RETURN_TRUE;
    }
    
    if (modbus_reg[MODBUS_HOLD_PGA1_GAIN] != 2 && modbus_reg[MODBUS_HOLD_PGA1_GAIN] != 4
        && modbus_reg[MODBUS_HOLD_PGA1_GAIN] != 8  && modbus_reg[MODBUS_HOLD_PGA1_GAIN] != 16) {
        modbus_reg[MODBUS_HOLD_PGA1_GAIN] = 2;
        ret = RETURN_TRUE;
    }
    
    if (modbus_reg[MODBUS_HOLD_PGA2_GAIN] != 2 && modbus_reg[MODBUS_HOLD_PGA2_GAIN] != 4
        && modbus_reg[MODBUS_HOLD_PGA2_GAIN] != 8  && modbus_reg[MODBUS_HOLD_PGA2_GAIN] != 16) {
        modbus_reg[MODBUS_HOLD_PGA2_GAIN] = 2;
        ret = RETURN_TRUE;
    }
        
    if (modbus_reg[MODBUS_HOLD_ADC_TIM_PSC] == 0 || modbus_reg[MODBUS_HOLD_ADC_TIM_PSC] == 0xFFFF) {
        modbus_reg[MODBUS_HOLD_ADC_TIM_PSC] = 24000/100;
        ret = RETURN_TRUE;
    }
    
    if (modbus_reg[MODBUS_HOLD_ADC_TIM_ARR] == 0 || modbus_reg[MODBUS_HOLD_ADC_TIM_ARR] == 0xFFFF) {
        modbus_reg[MODBUS_HOLD_ADC_TIM_ARR] = 10000/100;
        ret = RETURN_TRUE;
    }
    
    if (modbus_reg[MODBUS_HOLD_BEGIN] != 2) {
            modbus_reg[MODBUS_HOLD_BEGIN] = 2;
            ret = RETURN_TRUE;
    }


	if (ret == RETURN_TRUE) {
		config_write(CONFIG_BASE_ADDR, (uint8_t *)modbus_reg, sizeof(uint16_t) * REG_NUM_HOLD);
        uart_debug_str("config_init\r\n");
	}
}

/* 标识接口是否有数据 */
int32_t io_data_avail_usb = 0;
/* 检查接口数据状态，每隔一段时间，重置解析流程 */
void io_data_avail_check(void)
{
	io_data_avail_usb--;
	if (io_data_avail_usb == 0) {
		modbus_frame_analysis_data_rst(&analysis_data_modbus_rtu_usb);
	}
}

void m_io_check(void)
{
	modbus_frame_analysis_data_rst(&analysis_data_modbus_rtu_usb);
}


/* 缓冲区初始化 */
void sys_buf_init(void)
{
	/* 循环缓冲区初始化 */
	ring_8_reset(&ring_buf_debug);
    ring_8_reset(&ring_buf_modbus_usb);
    ring_8_reset(&ring_buf_adc_usb);
    ring_8_reset_large(&ring_buf_adc_quick_store);
    
    bsp_GetAdcValues();

    /* 参数初始化 */
    config_init();
}

/* 处理接口数据 */
void io_data_deal(void)
{
	int32_t i = 0;
	uint32_t data_len = 0;
	uint8_t buf[COM_BUF_SZ];
    
	/* 处理USB Modbus数据 */
	data_len = ring_8_sz(&ring_buf_modbus_usb);
	if (data_len > 0) {
		unsigned char buf[COM_BUF_SZ];
		if (data_len > COM_BUF_SZ)
			data_len = COM_BUF_SZ;
		memset(buf, 0, COM_BUF_SZ);
		ring_8_pop_n(&ring_buf_modbus_usb, buf, data_len);
		modbus_data_deal(buf, data_len, 0);
	}
    
}



/* 状态输出 */
void sensor_data_print(void)
{
    char output[1024] = {0};
    
    sprintf(output, "ADC=%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\r\n",
        ADCxValues[0], ADCxValues[1], ADCxValues[2], ADCxValues[3],
        ADCxValues[4], ADCxValues[5], ADCxValues[6], ADCxValues[7],
        ADCxValues[8], ADCxValues[9], ADCxValues[10], ADCxValues[11],
        ADCxValues[12], ADCxValues[13], ADCxValues[14], ADCxValues[15]);
    uart_debug_str(output);
}

/* 输出正弦函数 */
uint64_t sin_data_index = 0;
#define PI 3.14159265
#define ADC_frame_size (2 + 2 * 2 + 2)
/* 存储高速ADC数据 */
void adc_data_store_quick(void)
{
    int32_t len = ADC_frame_size;
    uint8_t frame[ADC_frame_size] = {0};
    int32_t index = 0;
    frame[index++] = ADC_FRAME_HEAD_1;
    frame[index++] = ADC_FRAME_HEAD_2;

    /* ADCxValues[8]为PGA1输出数值 ADCxValues[2]为PGA2输出数值*/

    *(uint16_t *)(frame + index) = ADCxValues[2]; index += 2;
    *(uint16_t *)(frame + index) = ADCxValues[8]; index += 2;
    *(uint16_t *)(frame + index) = 0; index += 2;


    //*(double *)(frame + index) = sin(PI * sin_data_index / 360); index += 8; sin_data_index++;
    //*(uint64_t *)(frame + index) = sin_data_index; index += 8; sin_data_index++;

    
    /* 将串口数据存入缓冲区 */
    if (ring_8_remain_large(&ring_buf_adc_quick_store) < len) {
        uint8_t tmp[ADC_frame_size];
        ring_8_pop_n_large(&ring_buf_adc_quick_store, tmp, len);
    }  
    ring_8_push_n_large(&ring_buf_adc_quick_store, frame, len);
}


/*这个是原本使用的adc_data_store_quick*/
void old_adc_data_store_quick(void)
{
    int32_t len = 4 + 2 * 16;
    uint8_t frame[4 + 2 * 16] = {0};
    int32_t index = 0;
    frame[index++] = ADC_FRAME_HEAD_1;
    frame[index++] = ADC_FRAME_HEAD_2;
    frame[index++] = ADC_FRAME_HEAD_3;
    frame[index++] = ADC_FRAME_HEAD_4;
    int32_t i = 0;
    /* ADCxValues[9]为PGA1输出数值 */
    for (i = 0; i < 12; i++) {
        *(uint16_t *)(frame + index) = ADCxValues[i]; index += 2;
    }
    //*(double *)(frame + index) = sin(PI * sin_data_index / 360); index += 8; sin_data_index++;
    *(uint64_t *)(frame + index) = sin_data_index; index += 8; sin_data_index++;


    /* 将串口数据存入缓冲区 */
    if (ring_8_remain_large(&ring_buf_adc_quick_store) < len) {
        uint8_t tmp[4 + 2 * 16];
        ring_8_pop_n_large(&ring_buf_adc_quick_store, tmp, len);
    }
    ring_8_push_n_large(&ring_buf_adc_quick_store, frame, len);
}

/* 判断是否已经发送完成 */
int8_t sig_sensor_data_usb_send = RETURN_UNKNOWN;
/* 推送ADC数据 */
/* 如果需要将某些ADC通道，组成低速帧传输，可以设置一个计数器，当高速帧累计N次后，
   自动组一个低速帧，紧接着高速帧后发出去，缓存区中始终保存有所有通道的高速数据 */
uint8_t frame_send[10 * (ADC_frame_size)];
void sensor_data_usb_send(void)
{
	int32_t timeout = 0;
	int32_t len = 10 * (ADC_frame_size);
	if (sig_usb_adc_data_transfer != RETURN_TRUE)
	    return;

	while (1) {
	    memset(frame_send, 0, len);
	    if (sig_usb_adc_data_transfer != RETURN_TRUE)
	        break;
	    if (ring_8_sz_large(&ring_buf_adc_quick_store) >= len) {
	        ring_8_pop_n_large(&ring_buf_adc_quick_store, frame_send, len);
	        sig_sensor_data_usb_send = RETURN_TRUE;
	        uart1_send_data(frame_send, len);
	    } else {
	        break;
	    }
	    sig_sensor_data_usb_send = RETURN_UNKNOWN;
	    IWDG_Feed();
	    timeout++;
	    if (timeout >= 1) {
	        timeout = 0;
	        io_data_deal();

	    }
	}
}



/* 传感器数据采集 */
void sensor_data_get(void)
{
    //bsp_GetAdcValues();
}



/* 比较保持寄存器数据是否变更 */
void modbus_hold_reg_data_compare(uint16_t *data_old, uint16_t *data_new)
{
    if (data_old[MODBUS_HOLD_DAC1_VOL] != data_new[MODBUS_HOLD_DAC1_VOL]) {
        DAC_CH1_Set_Vol(data_new[MODBUS_HOLD_DAC1_VOL]);
        uart_debug_str("DAC_CH1_Set\r\n");
    }
    
    if (data_old[MODBUS_HOLD_DAC2_VOL] != data_new[MODBUS_HOLD_DAC2_VOL]) {
        DAC_CH2_Set_Vol(data_new[MODBUS_HOLD_DAC2_VOL]);
        uart_debug_str("DAC_CH2_Set\r\n");
    }
    
    if (data_old[MODBUS_HOLD_PWM_PSC] != data_new[MODBUS_HOLD_PWM_PSC]
         || data_old[MODBUS_HOLD_PWM_ARR] != data_new[MODBUS_HOLD_PWM_ARR]) {
        TIM3_PWM_Init(data_new[MODBUS_HOLD_PWM_ARR] - 1, data_new[MODBUS_HOLD_PWM_PSC] - 1);
        uart_debug_str("PWM_Set\r\n");
    }  

    if (data_old[MODBUS_HOLD_PWM_COMPARE] != data_new[MODBUS_HOLD_PWM_COMPARE]) {
        TIM_SetTIM3Compare4(data_new[MODBUS_HOLD_PWM_COMPARE]);
        uart_debug_str("PWM_Compare_Set\r\n");
    }   

    if (data_old[MODBUS_HOLD_PGA1_GAIN] != data_new[MODBUS_HOLD_PGA1_GAIN]) {
        if (data_new[MODBUS_HOLD_PGA2_GAIN] != 2 && data_new[MODBUS_HOLD_PGA2_GAIN] != 4
            && data_new[MODBUS_HOLD_PGA2_GAIN] != 8 && data_new[MODBUS_HOLD_PGA2_GAIN] != 16) {
            data_new[MODBUS_HOLD_PGA2_GAIN] = data_old[MODBUS_HOLD_PGA1_GAIN];
        }
        MX_OPAMP1_Init();
        uart_debug_str("MX_OPAMP1_Set\r\n");
    } 

    if (data_old[MODBUS_HOLD_PGA2_GAIN] != data_new[MODBUS_HOLD_PGA2_GAIN]) {
        if (data_new[MODBUS_HOLD_PGA2_GAIN] != 2 && data_new[MODBUS_HOLD_PGA2_GAIN] != 4
            && data_new[MODBUS_HOLD_PGA2_GAIN] != 8 && data_new[MODBUS_HOLD_PGA2_GAIN] != 16) {
            data_new[MODBUS_HOLD_PGA2_GAIN] = data_old[MODBUS_HOLD_PGA2_GAIN];
        }
        MX_OPAMP2_Init();
        uart_debug_str("MX_OPAMP2_Set\r\n");
    }    

    if (data_old[MODBUS_HOLD_ADC_TIM_PSC] != data_new[MODBUS_HOLD_ADC_TIM_PSC]
         || data_old[MODBUS_HOLD_ADC_TIM_ARR] != data_new[MODBUS_HOLD_ADC_TIM_ARR]) {
        MX_TIM5_Init();
        uart_debug_str("ADC_CAPTURE_INTERVAL_Set\r\n");
    }      

    if (data_new[MODBUS_HOLD_BEGIN] == 1) {
    	sig_usb_adc_data_transfer = RETURN_TRUE;
    	ring_8_reset_large(&ring_buf_adc_quick_store);
        }
    if(data_new[MODBUS_HOLD_BEGIN] != 1){
    	sig_usb_adc_data_transfer = RETURN_UNKNOWN;
    }


}
