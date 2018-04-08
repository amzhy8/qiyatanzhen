#ifndef __BSP_CIRCLE_FIFO_H__
#define __BSP_CIRCLE_FIFO_H__

#include "stm32f4xx_hal.h"



#define CIRCLE_FIFO_SUCCESS 0
#define CIRCLE_FIFO_FALSE 1

#define CIRCLE_FIFO_BUSSY 1
#define CIRCLE_FIFO_NOBUSSY 0

#define CIRCLE_FIFO_MEMBER_MAXLEN 8*2
#define CIRCLE_FIFO_MAXDEEP 50



typedef struct{
	uint8_t bussy_flag;//������������
	uint8_t buf[CIRCLE_FIFO_MAXDEEP][CIRCLE_FIFO_MEMBER_MAXLEN];
	uint32_t bufmemberlen[CIRCLE_FIFO_MAXDEEP];//�洢ÿ�δ������ݵĳ���  ��bufһһ��Ӧ
	//fifo��������
	uint32_t fifo_deep;
	uint32_t fifo_member_len;
	uint32_t fifo_writeindex;
	uint32_t fifo_readindex;
	uint16_t buf_cnt;
} CircleFifo_Typedef;
extern CircleFifo_Typedef CircleFifo;

//��ʼ��  ���û��λ��������  ��Ա����
uint8_t CircleFifoInit(CircleFifo_Typedef *obj,uint32_t fifodeep,uint32_t memberlen);
//��������
uint8_t CircleFifoInput(CircleFifo_Typedef *obj,uint8_t *data,uint32_t len);
//��������
uint8_t CircleFifoOutput(CircleFifo_Typedef *obj,uint8_t *data,uint32_t *len);


#endif

