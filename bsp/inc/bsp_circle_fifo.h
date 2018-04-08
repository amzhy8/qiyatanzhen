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
	uint8_t bussy_flag;//缓冲器互斥量
	uint8_t buf[CIRCLE_FIFO_MAXDEEP][CIRCLE_FIFO_MEMBER_MAXLEN];
	uint32_t bufmemberlen[CIRCLE_FIFO_MAXDEEP];//存储每次存入数据的长度  与buf一一对应
	//fifo自身属性
	uint32_t fifo_deep;
	uint32_t fifo_member_len;
	uint32_t fifo_writeindex;
	uint32_t fifo_readindex;
	uint16_t buf_cnt;
} CircleFifo_Typedef;
extern CircleFifo_Typedef CircleFifo;

//初始化  配置环形缓冲器深度  成员长度
uint8_t CircleFifoInit(CircleFifo_Typedef *obj,uint32_t fifodeep,uint32_t memberlen);
//传入数据
uint8_t CircleFifoInput(CircleFifo_Typedef *obj,uint8_t *data,uint32_t len);
//读出数据
uint8_t CircleFifoOutput(CircleFifo_Typedef *obj,uint8_t *data,uint32_t *len);


#endif

