#include "bsp_queue.h"
#include <string.h>

//初始化队列
uint8_t Queue_Init(Queue_Typdef *queue){
	queue->size=QUEUE_BUF_LEN;//可更改设置
	memset(queue->buf,0,QUEUE_BUF_LEN);
	queue->currentcount=0;
	queue->readindex=0;
	queue->writeindex=0;
	return QUEUE_SUCCESS;
}

//判断队列是否已已经满了
uint8_t Queue_Is_Full(Queue_Typdef *queue){
	if(((queue->writeindex+1)%queue->size)==queue->readindex){
		return QUEUE_SUCCESS;
	}else{
		return QUEUE_FAIL;
	}
}

//判断队列是否为空
uint8_t Queue_Is_Empty(Queue_Typdef *queue){
	if(queue->writeindex==queue->readindex){
		return QUEUE_SUCCESS;
	}else{
		return QUEUE_FAIL;
	}
}

//写数据到队列
uint8_t Queue_WriteByte(Queue_Typdef *queue,uint8_t data){
	if(Queue_Is_Full(queue)==QUEUE_SUCCESS){
		return QUEUE_FAIL;
	}
	
	queue->buf[queue->writeindex]=data;
	queue->currentcount++;
	queue->writeindex=(queue->writeindex+1)%queue->size;
	return QUEUE_SUCCESS; 
}

//从队中读数据
uint8_t Queue_ReadByte(Queue_Typdef *queue,uint8_t *data){
	if(Queue_Is_Empty(queue)==QUEUE_SUCCESS){
		return QUEUE_FAIL;
	}
	*data=queue->buf[queue->readindex];
	queue->currentcount--;
	queue->readindex=(queue->readindex+1)%queue->size;
	return QUEUE_SUCCESS; 
}


//初始化指针队列
uint8_t PointQueue_Init(PointQueue_Typdef *queue,void *point){
	queue->size=POINT_QUEUE_BUF_LEN;//可更改设置
	//memset(queue->buf,NULL,QUEUE_BUF_LEN);
	queue->point=point;
	queue->currentcount=0;
	queue->readindex=0;
	queue->writeindex=0;
	return QUEUE_SUCCESS;
}
