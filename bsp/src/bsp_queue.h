#ifndef __BSP_QUEUE_H__
#define __BSP_QUEUE_H__
#include "stm32f4xx_hal.h"


#define QUEUE_SUCCESS 0
#define QUEUE_FAIL 1

//ʹ�ܴ��ڽ��ն���������
#define QUEUE_BUF_LEN 256
#define POINT_QUEUE_BUF_LEN 100

//���ݶ���
typedef struct{
	uint8_t *buf;
	uint16_t size;
	uint16_t readindex;
	uint16_t writeindex;
	uint16_t currentcount;
}Queue_Typdef;

//ָ�����
typedef struct{
	void *point;
	uint16_t size;
	uint16_t readindex;
	uint16_t writeindex;
	uint16_t currentcount;
}PointQueue_Typdef;

//��ʼ������
uint8_t Queue_Init(Queue_Typdef *queue);
//�ж϶����Ƿ����Ѿ�����
uint8_t Queue_Is_Full(Queue_Typdef *queue);
//�ж϶����Ƿ�Ϊ��
uint8_t Queue_Is_Empty(Queue_Typdef *queue);
//д���ݵ�����
uint8_t Queue_WriteByte(Queue_Typdef *queue,uint8_t data);
//�Ӷ��ж�����
uint8_t Queue_ReadByte(Queue_Typdef *queue,uint8_t *data);

#endif


