#ifndef __PROTOCOL_MODBUS_SLAVE_H
#define __PROTOCOL_MODBUS_SLAVE_H

#include "common.h"

/* 帧状态机 */
typedef enum {
	MODBUS_FRAME_STATE_DEV,			/* 设备地址 */
	MODBUS_FRAME_STATE_CMD,			/* 功能码 */
	MODBUS_FRAME_STATE_ADDR_H,		/* 地址 */
	MODBUS_FRAME_STATE_ADDR_L,		/* 地址 */
	MODBUS_FRAME_STATE_LEN_H,		/* 数据长度 */
	MODBUS_FRAME_STATE_LEN_L,		/* 数据长度 */
	MODBUS_FRAME_STATE_MSG_H,		/* 0x06指令对应消息体高字节 */
	MODBUS_FRAME_STATE_MSG_L,		/* 0x06指令对应消息体低字节 */
	MODBUS_FRAME_STATE_LEN,			/* 0x10指令对应消息体字节长度 */
	MODBUS_FRAME_STATE_MSG,			/* 0x10指令对应消息体 */
	MODBUS_FRAME_STATE_CHK_1,		/* 帧校验第一个字节 */
	MODBUS_FRAME_STATE_CHK_2		/* 帧校验第二个字节 */
} MODBUS_FRAME_STATE;

/* 帧解析信息暂存 */
typedef struct
{
	MODBUS_FRAME_STATE current_state;

	uint8_t data8;				/* 保存读取到的字节 */
	uint8_t data[2];			/* 保存读取到的字节数组 */
	uint16_t len;
	uint16_t crc;
	uint16_t addr;
	uint8_t cmd;
	uint16_t hold_single;			/* 保存0x06指令写入数据 */
	uint16_t hold_mutli_num; 		/* 保存0x10指令写入数据的字节长度 */
	uint16_t hold_mutli_seq; 		/* 记录读取到的0x10指令写入数据的字节序号 */
	
	uint16_t index;
	uint8_t frame[32];
} MODBUS_FRAME_ANALYSIS_DATA;

/* CRC校验 */
uint16_t modbus_crc16(uint8_t *puchMsg, uint16_t usDataLen);
/* 初始化帧解析信息 */
void modbus_frame_analysis_data_rst(MODBUS_FRAME_ANALYSIS_DATA *data);
/* 数据解析初始化 */
void protocol_modbus_rtu_analog_init(void);
/* 协议解析处理 */
void modbus_data_deal(uint8_t *buf, uint16_t len, uint8_t io_type);

#endif
