#ifndef __BSP_QUEUE_H__
#define __BSP_QUEUE_H__
#include "stm32f4xx_hal.h"


#define QUEUE_SUCCESS 0
#define QUEUE_FAIL 1

//使能串口接收二级缓冲器
#define QUEUE_BUF_LEN 256
#define POINT_QUEUE_BUF_LEN 100

//数据队列
typedef struct{
	uint8_t *buf;
	uint16_t size;
	uint16_t readindex;
	uint16_t writeindex;
	uint16_t currentcount;
}Queue_Typdef;

//指针队列
typedef struct{
	void *point;
	uint16_t size;
	uint16_t readindex;
	uint16_t writeindex;
	uint16_t currentcount;
}PointQueue_Typdef;

//初始化队列
uint8_t Queue_Init(Queue_Typdef *queue);
//判断队列是否已已经满了
uint8_t Queue_Is_Full(Queue_Typdef *queue);
//判断队列是否为空
uint8_t Queue_Is_Empty(Queue_Typdef *queue);
//写数据到队列
uint8_t Queue_WriteByte(Queue_Typdef *queue,uint8_t data);
//从队中读数据
uint8_t Queue_ReadByte(Queue_Typdef *queue,uint8_t *data);

#endif


