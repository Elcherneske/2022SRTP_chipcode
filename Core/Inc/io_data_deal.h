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
 
#ifndef _IO_DATA_DEAL_H_
#define _IO_DATA_DEAL_H_

#include "circular_buffer.h"

/* 接口数据缓冲区 */
extern BUF_RING_8 ring_buf_debug;
extern BUF_RING_8 ring_buf_modbus_usb;

/* 缓冲区初始化 */
void sys_buf_init(void);
/* 处理接口数据 */
void io_data_deal(void);
/* 传感器数据采集 */
void sensor_data_get(void);
/* 状态输出 */
void sensor_data_print(void);

/* 在指定地址开始读出指定个数的数据 */
void config_read(uint32_t ReadAddr, uint8_t *pBuffer, uint32_t NumToRead);
/* 在指定地址开始写入指定个数的数据 */
void config_write(uint32_t WriteAddr, uint8_t *pBuffer, uint32_t NumToWrite);
/* 检查接口数据状态，每隔一段时间，重置解析流程 */
void io_data_avail_check(void);
/* 推送ADC数据 */
void sensor_data_usb_send(void);
/* 比较保持寄存器数据是否变更 */
void modbus_hold_reg_data_compare(uint16_t *data_old, uint16_t *data_new);
/* 累计数据 */
void adc_data_total_get(void);
/* 存储高速ADC数据 */
void adc_data_store_quick(void);

#endif
