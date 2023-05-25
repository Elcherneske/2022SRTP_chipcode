#include "protocol_modbus_slave.h"
#include "sensor.h"
#include "io_data_deal.h"
#include "usart.h"

/* 帧数据处理 */
void simple_frame_deal(uint8_t *frame, uint8_t io_type);
/* 帧解析缓存 */
extern MODBUS_FRAME_ANALYSIS_DATA analysis_data_modbus_rtu_usb;

/* 标识接口是否有数据 */
extern int32_t io_data_avail_usb;
/* USB发送数据 */
uint8_t CDC_Transmit_FS(uint8_t epnum, uint8_t* Buf, uint16_t Len);

/* 发送数据 */
void modbus_data_response(uint8_t *data, int32_t len)
{
    //CDC_Transmit_FS(USB_CONFIG_MODBUS_EP_WRITE, data, len);
}

const uint8_t auchCRCHi[] = {
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
	0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
	0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

const uint8_t auchCRCLo[] = {
	0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
	0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
	0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
	0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
	0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
	0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
	0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
	0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
	0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
	0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
	0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
	0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
	0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
	0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
	0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
	0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
	0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
	0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
	0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
	0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
	0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
	0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
	0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
	0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
	0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
	0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

uint16_t modbus_crc16(uint8_t *puchMsg, uint16_t usDataLen)
{
	uint8_t uchCRCHi = 0xFF; /* 高CRC字节初始化 */
	uint8_t uchCRCLo = 0xFF; /* 低CRC 字节初始化 */
	uint16_t uIndex; /* CRC循环中的索引 */
	while(usDataLen--) {
		uIndex = uchCRCHi ^* puchMsg++;
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex];
		uchCRCLo = auchCRCLo[uIndex];
	}
	return (uchCRCHi<<8 | uchCRCLo);
}

/* 初始化帧解析信息 */
void modbus_frame_analysis_data_rst(MODBUS_FRAME_ANALYSIS_DATA *data)
{
	memset(data, 0, sizeof(MODBUS_FRAME_ANALYSIS_DATA));
	data->current_state = MODBUS_FRAME_STATE_DEV;
	io_data_avail_usb = 0;
}

/* 数据解析初始化 */
void protocol_modbus_rtu_analog_init(void)
{
	modbus_frame_analysis_data_rst(&analysis_data_modbus_rtu_usb);
}

/* 读保存寄存器处理 */
void hold_registers_read_deal(uint8_t *frame, uint16_t addr, uint16_t len, uint8_t io_type)
{
	int32_t i = 0;
	uint16_t crc = 0;
	uint16_t *data = modbus_reg;
	uint16_t index = 0;
	uint8_t buf[REG_NUM_HOLD * 2 + 16];
	memset(buf, 0, REG_NUM_HOLD * 2 + 16);
	
	/* 读取寄存器数量超过限制 */
	if ((addr + len) > REG_NUM_HOLD)
		return;

	buf[index++] = frame[0];
	buf[index++] = 0x03;
	buf[index++] = (len * 2) & 0xFF;
	for (i = 0; i < len; i++) {
		uint16_t temp = data[addr + i];
		buf[index + i * 2] = (temp >> 8) & 0xFF;
		buf[index + i * 2 + 1] = (temp >> 0) & 0xFF;
	}
	
	crc = modbus_crc16(buf, index + len * 2);
	buf[index + len * 2] = (crc >> 8) & 0xFF;
	buf[index + len * 2 + 1] = (crc >> 0) & 0xFF;
	modbus_data_response(buf, index + len * 2 + 2);
}

/* 单个写入保持寄存器 */
void hold_registers_write_single_deal(uint8_t *frame, uint16_t addr, uint16_t hold_single, uint8_t io_type)
{
    uint16_t modbus_reg_old[REG_NUM_HOLD];
    memcpy((uint8_t *)modbus_reg_old, (uint8_t *)modbus_reg, sizeof(uint16_t) * REG_NUM_HOLD);
    
	uint16_t *data = modbus_reg;
	
	/* 读取寄存器数量超过限制 */
	if (addr >= REG_NUM_HOLD)
		return;
	
	data[addr] = hold_single;
	config_write(CONFIG_BASE_ADDR + sizeof(uint16_t) * addr, (uint8_t *)(data + addr), sizeof(uint16_t));
	
	modbus_data_response(frame, 8);
    
    modbus_hold_reg_data_compare(modbus_reg_old, modbus_reg);
}

/* 多个写入保持寄存器 */
void hold_registers_write_multi_deal(uint8_t *frame, uint16_t addr, uint16_t len, uint8_t io_type)
{
    uint16_t modbus_reg_old[REG_NUM_HOLD];
    memcpy((uint8_t *)modbus_reg_old, (uint8_t *)modbus_reg, sizeof(uint16_t) * REG_NUM_HOLD);
    
	int32_t i = 0;
	uint16_t crc = 0;
	uint16_t index = 0;
	uint16_t *data = modbus_reg;
	uint8_t buf[REG_NUM_HOLD * 2 + 16];
	memset(buf, 0, REG_NUM_HOLD * 2 + 16);
	
	memcpy(buf, frame, 6);
	crc = modbus_crc16(buf, 6);
	buf[6] = (crc >> 8) & 0xFF;
	buf[7] = (crc >> 0) & 0xFF;
	
	/* 读取寄存器数量超过限制 */
	if ((addr + len) > REG_NUM_HOLD)
		return;
	
	for (i = 0; i < len; i++) {
		data[addr + i] = (frame[index + 7 + i * 2] << 8) | frame[index + 7 + i * 2 + 1];
	}
		
    config_write(CONFIG_BASE_ADDR + sizeof(uint16_t) * addr, (uint8_t *)(data + addr), sizeof(uint16_t) * len);
	modbus_data_response(buf, 8);
    modbus_hold_reg_data_compare(modbus_reg_old, modbus_reg);
}

/* 帧数据处理 */
void modbus_frame_deal(uint8_t *frame, uint8_t io_type)
{	
	uint8_t dev, cmd, addr_h, addr_l, len_h, len_l;
	uint16_t addr, len, hold_single, index = 0;
	
	dev = frame[index++];
	/* 设备地址不正确 */
	if (dev != MODBUS_DEV_ADDR)
		return;
		
	cmd = frame[index++];
	addr_h = frame[index++];
	addr_l = frame[index++];
	len_h = frame[index++];
	len_l = frame[index++];
	addr = (addr_h << 8) | addr_l;
	len = (len_h << 8) | len_l;
	
	if (cmd == 0x03) { /* 读取保持寄存器 */
		hold_registers_read_deal(frame, addr, len, io_type);
	} else if (cmd == 0x06) { /* 单个写入保持寄存器 */
		hold_single = len;
		hold_registers_write_single_deal(frame, addr, hold_single, io_type);
	} else if (cmd == 0x10) { /* 多个写入保持寄存器 */
		hold_registers_write_multi_deal(frame, addr, len, io_type);
	}
}

/* 协议解析处理 */
void modbus_data_deal(uint8_t *buf, uint16_t len, uint8_t io_type)
{	
	uint16_t i = 0;
	MODBUS_FRAME_ANALYSIS_DATA *data = &analysis_data_modbus_rtu_usb;
		
	for (i = 0; i < len; i++) {			
		data->data8 = buf[i];
		switch (data->current_state) {
			case MODBUS_FRAME_STATE_DEV:
				if (data->data8 == MODBUS_DEV_ADDR) {
					data->frame[(data->index)++] = data->data8;
					data->current_state = MODBUS_FRAME_STATE_CMD;
					//uart_debug_str("MODBUS_FRAME_STATE_CMD\n");

					/* 一帧解析开始，开始计时，超时重置 */
					io_data_avail_usb = IO_DATA_AVAIL_DELAY;
				} else {
					modbus_frame_analysis_data_rst(data);
					continue;
				}
				break;
			case MODBUS_FRAME_STATE_CMD:
				if (data->data8 == 0x03 || data->data8 == 0x06 || data->data8 == 0x10) {
					data->frame[(data->index)++] = data->data8;
					data->cmd = data->data8;
					data->current_state = MODBUS_FRAME_STATE_ADDR_H;
					//uart_debug_str("MODBUS_FRAME_STATE_ADDR_H\n");
				} else {
					modbus_frame_analysis_data_rst(data);
					continue;
				}
				break;
			case MODBUS_FRAME_STATE_ADDR_H:
				data->frame[(data->index)++] = data->data8;
				data->current_state = MODBUS_FRAME_STATE_ADDR_L;
				//uart_debug_str("MODBUS_FRAME_STATE_ADDR_L\n");
				break;
			case MODBUS_FRAME_STATE_ADDR_L:
				data->frame[(data->index)++] = data->data8;
				if (data->cmd == 0x03 || data->cmd == 0x10) {
					data->current_state = MODBUS_FRAME_STATE_LEN_H;
					//uart_debug_str("MODBUS_FRAME_STATE_LEN_H\n");
				} else if (data->cmd == 0x06) {
					data->current_state = MODBUS_FRAME_STATE_MSG_H;
					//uart_debug_str("MODBUS_FRAME_STATE_MSG\n");
				}
				break;		
			case MODBUS_FRAME_STATE_LEN_H:
				data->frame[(data->index)++] = data->data8;
				data->current_state = MODBUS_FRAME_STATE_LEN_L;
				//uart_debug_str("MODBUS_FRAME_STATE_LEN_L\n");
				break;
			case MODBUS_FRAME_STATE_LEN_L:
				data->frame[(data->index)++] = data->data8;
				if (data->cmd == 0x03) {
					data->current_state = MODBUS_FRAME_STATE_CHK_1;
					//uart_debug_str("MODBUS_FRAME_STATE_CHK_1\n");
					data->crc = modbus_crc16(data->frame, 6);
				} else if (data->cmd == 0x10) {
					data->current_state = MODBUS_FRAME_STATE_LEN;
					//uart_debug_str("MODBUS_FRAME_STATE_LEN\n");
				}
				break;
			case MODBUS_FRAME_STATE_MSG_H:
				data->frame[(data->index)++] = data->data8;
				data->current_state = MODBUS_FRAME_STATE_MSG_L;
				//uart_debug_str("MODBUS_FRAME_STATE_MSG_L\n");
				break;
			case MODBUS_FRAME_STATE_MSG_L:
				data->frame[(data->index)++] = data->data8;
				data->current_state = MODBUS_FRAME_STATE_CHK_1;
				//uart_debug_str("MODBUS_FRAME_STATE_CHK_1\n");
				data->crc = modbus_crc16(data->frame, 6);
				break;
			case MODBUS_FRAME_STATE_LEN:
				data->frame[(data->index)++] = data->data8;
				data->hold_mutli_num = data->data8;
				data->current_state = MODBUS_FRAME_STATE_MSG;
				//uart_debug_str("MODBUS_FRAME_STATE_MSG\n");
				break;
			case MODBUS_FRAME_STATE_MSG:
				data->frame[(data->index)++] = data->data8;
				(data->hold_mutli_seq)++;
				if (data->hold_mutli_seq >= data->hold_mutli_num) {			
					data->current_state = MODBUS_FRAME_STATE_CHK_1;
					//uart_debug_str("MODBUS_FRAME_STATE_CHK_1\n");
					data->crc = modbus_crc16(data->frame, 7 + data->hold_mutli_num);
				}
				break;
			case MODBUS_FRAME_STATE_CHK_1:
				data->frame[(data->index)++] = data->data8;
				data->data[0] = data->data8;
				data->current_state = MODBUS_FRAME_STATE_CHK_2;
				//uart_debug_str("MODBUS_FRAME_STATE_CHK_2\n");
				break;
			case MODBUS_FRAME_STATE_CHK_2:
				data->frame[(data->index)++] = data->data8;
				data->data[1] = data->data8;
				if (data->crc == ((data->data[0] << 8) | data->data[1])) {
					//uart_debug_str("OK\r\n");
					modbus_frame_deal(data->frame, io_type);
				} else {
					//uart_debug_str("ERR\r\n");
				}
				modbus_frame_analysis_data_rst(data);
				break;
		}
	}
}
