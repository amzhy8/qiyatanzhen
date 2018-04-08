#ifndef _BSP_EVENT_H__
#define _BSP_EVENT_H__
#include "stm32f4xx_hal.h"

#define EVENT_MAX_COUNT   50
#define EVENT_MEMBER_BUF_LEN 10


#define EVENT_SUCCESS 0
#define EVENT_FLASE 1

#define EVENT_MEMBER_BUSY  1
#define EVENT_MEMBER_FREE  0






//�¼�����
#define EVENT_NO_EVENT 0
#define ADC_SAMPLE_ORDER 1







//�¼����λ�������Ա
typedef struct{
	uint8_t BusyOrFree;
	uint8_t EvenID;
	uint8_t EventBuf[EVENT_MEMBER_BUF_LEN];
	uint32_t BufLen;
}EventMember_Typedef;

//�¼����λ�����
typedef struct{
	EventMember_Typedef member[EVENT_MAX_COUNT];
	uint16_t size;
	uint16_t readindex;
	uint16_t writeindex;
	uint16_t currentcount;
}EventQueue_Typdef;


//����
//�¼������Ƿ��Ѿ�����
uint8_t EventQueueIsFull(EventQueue_Typdef *event_obj);
//�¼������Ƿ�Ϊ��
uint8_t EventQueueIsEmpty(EventQueue_Typdef *event_obj);
//�����¼����¼�������λ��
uint8_t SentEventToQueue(EventQueue_Typdef *event_obj,uint8_t ID,uint8_t *eventbuf,uint32_t *buf_len);
//���¼�������λ����ȡ��һ���¼�
uint8_t Read_Event(EventQueue_Typdef *event_obj,EventMember_Typedef *event_member);
//���¼�������λ���м���¼�
uint8_t Check_Event(EventMember_Typedef *event_member);
uint8_t EventCheckCallback(EventQueue_Typdef *event_obj);


extern EventQueue_Typdef EventQueue;

#endif

