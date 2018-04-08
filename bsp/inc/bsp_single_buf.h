#ifndef _BSP_SINGLE_BUF_H__
#define _BSP_SINGLE_BUF_H__

#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>
#include "bsp_config.h"



#define SINGLE_BUF_DEEP  CONFIG_SINGLE_BUF_DEEP
#define SINGLE_BUF_MEMBER_LEN CONFIG_SINGLE_BUF_MEMBER_LEN


#define SIGNLE_BUF_EMPTY 0
#define SIGNLE_BUF_FULL  1

#define SIGNLE_BUF_IDEL  0
#define SIGNLE_BUF_BUSSY 1


#define SIGNLE_BUF_SUCCESS 0
#define SIGNLE_BUF_FALSE 1



typedef struct{
	int16_t buf[SINGLE_BUF_DEEP*SINGLE_BUF_MEMBER_LEN];
	uint8_t fullorempty;
	uint16_t buf_deep;
	uint16_t buf_member_len;
	uint16_t buf_index;
	uint16_t buf_cnt;
}SingleBuf_Typedef;

typedef struct{
	uint16_t buf_deep;
	uint16_t buf_member_len;
}SingleBufConfig_Typedef;

extern SingleBuf_Typedef SingleBuf;
uint8_t InitSingleBuf(SingleBuf_Typedef *singlebuf,uint16_t deep,uint16_t member_len);
//��ʼ��
uint8_t ReInitSingleBuf(SingleBuf_Typedef *singlebuf);
//���BUF�Ƿ��Ѿ�����
uint8_t SingleBufIsFull(SingleBuf_Typedef *singlebuf);
//���BUF�Ƿ�Ϊ��
uint8_t SingleBufIsEmpty(SingleBuf_Typedef *singlebuf);
//װ��һ������
uint8_t SingleBufLoadData(SingleBuf_Typedef *singlebuf, int16_t *data);
//����������
uint8_t SingleBufReadAll(SingleBuf_Typedef *singlebuf, int16_t *data);




#endif

