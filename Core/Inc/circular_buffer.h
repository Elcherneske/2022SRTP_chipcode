/****************************************************
 * 目的: 循环缓冲区 (头文件)
 * 用法:
 * 文件: circular_buffer.h
 * 版本: v1.3
 * 作者: 孙坚
 * 邮件: wgetfree@gmail.com
 * 创建时间: 2011-05-22 09:47:49
 * 修改时间: 2012-11-24 12:36:28
 ****************************************************/

#ifndef _CIRCULAR_BUFFER_H_
#define _CIRCULAR_BUFFER_H_

#include "common.h"

/* 缓冲区大小 */
#define BUF_SZ (1024)
#define BUF_SZ_LARGE (50 * 8000)

/********************* 1字节单元缓冲区 *******************/
/* 缓冲区结构 */
typedef struct
{
	unsigned char buf[BUF_SZ];
	unsigned int start;
	unsigned int end;
} BUF_RING_8;

typedef struct
{
	unsigned char buf[BUF_SZ_LARGE];
	unsigned int start;
	unsigned int end;
} BUF_RING_8_LARGE;

/* 清空缓冲区 */
void ring_8_reset(BUF_RING_8 *RING);
/* 返回缓冲区中保存的元素个数 */
unsigned int ring_8_sz(BUF_RING_8 *RING);
/* 返回缓冲区中剩余空间的元素个数 */
unsigned int ring_8_remain(BUF_RING_8 *RING);
/* 判断缓冲区是否写满 */
int32_t ring_8_is_full(BUF_RING_8 *RING);
/* 判断缓冲区是否为空 */
int32_t ring_8_is_empty(BUF_RING_8 *RING);
/* 向缓冲区写入n个元素 */
int32_t ring_8_push_n(BUF_RING_8 *RING, unsigned char *buf, unsigned int n);
/* 从缓冲区提取n个元素 */
int32_t ring_8_pop_n(BUF_RING_8 *RING, unsigned char *buf, unsigned int n);

/* 清空缓冲区 */
void ring_8_reset_large(BUF_RING_8_LARGE *RING);
/* 返回缓冲区中保存的元素个数 */
unsigned int ring_8_sz_large(BUF_RING_8_LARGE *RING);
/* 返回缓冲区中剩余空间的元素个数 */
unsigned int ring_8_remain_large(BUF_RING_8_LARGE *RING);
/* 判断缓冲区是否写满 */
int32_t ring_8_is_full_large(BUF_RING_8_LARGE *RING);
/* 判断缓冲区是否为空 */
int32_t ring_8_is_empty_large(BUF_RING_8_LARGE *RING);
/* 向缓冲区写入n个元素 */
int32_t ring_8_push_n_large(BUF_RING_8_LARGE *RING, unsigned char *buf, unsigned int n);
/* 从缓冲区提取n个元素 */
int32_t ring_8_pop_n_large(BUF_RING_8_LARGE *RING, unsigned char *buf, unsigned int n);

#endif /* _CIRCULAR_BUFFER_H_ */
