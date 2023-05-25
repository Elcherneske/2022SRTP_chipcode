/****************************************************
 * 目的: 循环缓冲区
 * 用法:
 * 文件: circular_buffer.c
 * 版本: v1.3
 * 作者: 孙坚
 * 邮件: wgetfree@gmail.com
 * 创建时间: 2011-05-22 09:47:49
 * 修改时间: 2013-05-16 14:16:35
 ****************************************************/

#include "circular_buffer.h"

/********************* 1字节单元缓冲区 *******************/
/* 清空缓冲区 */
void ring_8_reset(BUF_RING_8 *RING)
{
	memset(RING, 0, sizeof(*RING));
}

/* 返回缓冲区中保存的元素个数 */
unsigned int ring_8_sz(BUF_RING_8 *RING)
{
	return (RING->start - RING->end + BUF_SZ) % BUF_SZ;
}

/* 返回缓冲区中剩余空间的元素个数 */
unsigned int ring_8_remain(BUF_RING_8 *RING)
{
	return (RING->end - RING->start - 1 + BUF_SZ) % BUF_SZ;
}

/* 判断缓冲区是否写满 */
int32_t ring_8_is_full(BUF_RING_8 *RING)
{
	if (((RING->start + 1) % BUF_SZ) == RING->end)
		return RETURN_TRUE;

	return RETURN_FALSE;
}

/* 判断缓冲区是否为空 */
int32_t ring_8_is_empty(BUF_RING_8 *RING)
{
	if (RING->end == RING->start)
		return RETURN_TRUE;

	return RETURN_FALSE;
}

/* 向缓冲区写入n个元素 */
int32_t ring_8_push_n(BUF_RING_8 *RING, unsigned char *buf, unsigned int n)
{

	uint32_t i = 0;


	if (ring_8_remain(RING) < n)
		return RETURN_ERROR;

	for (i = 0; i < n; i++) {
		RING->buf[RING->start] = buf[i];
		RING->start = (RING->start + 1) % BUF_SZ;
	}

	return n;
}

/* 从缓冲区提取n个元素 */
int32_t ring_8_pop_n(BUF_RING_8 *RING, unsigned char *buf, unsigned int n)
{

	uint32_t i = 0;


	if (ring_8_sz(RING) < n || buf == NULL || n <= 0)
		return RETURN_ERROR;

	for (i = 0; i < n; i++) {
		buf[i] = RING->buf[RING->end];
		RING->end = (RING->end + 1) % BUF_SZ;
	}

	return n;
}

/********************* 1字节单元缓冲区 *******************/
/* 清空缓冲区 */
void ring_8_reset_large(BUF_RING_8_LARGE *RING)
{
	memset(RING, 0, sizeof(*RING));
}

/* 返回缓冲区中保存的元素个数 */
unsigned int ring_8_sz_large(BUF_RING_8_LARGE *RING)
{
	return (RING->start - RING->end + BUF_SZ_LARGE) % BUF_SZ_LARGE;
}

/* 返回缓冲区中剩余空间的元素个数 */
unsigned int ring_8_remain_large(BUF_RING_8_LARGE *RING)
{
	return (RING->end - RING->start - 1 + BUF_SZ_LARGE) % BUF_SZ_LARGE;
}

/* 判断缓冲区是否写满 */
int32_t ring_8_is_full_large(BUF_RING_8_LARGE *RING)
{
	if (((RING->start + 1) % BUF_SZ_LARGE) == RING->end)
		return RETURN_TRUE;

	return RETURN_FALSE;
}

/* 判断缓冲区是否为空 */
int32_t ring_8_is_empty_large(BUF_RING_8_LARGE *RING)
{
	if (RING->end == RING->start)
		return RETURN_TRUE;

	return RETURN_FALSE;
}

/* 向缓冲区写入n个元素 */
int32_t ring_8_push_n_large(BUF_RING_8_LARGE *RING, unsigned char *buf, unsigned int n)
{

	uint32_t i = 0;


	if (ring_8_remain_large(RING) < n)
		return RETURN_ERROR;

	for (i = 0; i < n; i++) {
		RING->buf[RING->start] = buf[i];
		RING->start = (RING->start + 1) % BUF_SZ_LARGE;
	}

	return n;
}

/* 从缓冲区提取n个元素 */
int32_t ring_8_pop_n_large(BUF_RING_8_LARGE *RING, unsigned char *buf, unsigned int n)
{

	uint32_t i = 0;


	if (ring_8_sz_large(RING) < n || buf == NULL || n <= 0)
		return RETURN_ERROR;

	for (i = 0; i < n; i++) {
		buf[i] = RING->buf[RING->end];
		RING->end = (RING->end + 1) % BUF_SZ_LARGE;
	}

	return n;
}

