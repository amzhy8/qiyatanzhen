#include "bsp_event.h"
#include "string.h"
#include "bsp_Ad7606.h"
#include "bsp_double_buf.h"

//ʵ����ȫ�ֶ���
EventQueue_Typdef EventQueue;

//�¼������Ƿ��Ѿ�����
uint8_t EventQueueIsFull(EventQueue_Typdef *event_obj){
	if(event_obj->readindex==(event_obj->writeindex+1)%event_obj->size){
		return EVENT_SUCCESS;
	}
	return EVENT_FLASE;
}

//�¼������Ƿ�Ϊ��
uint8_t EventQueueIsEmpty(EventQueue_Typdef *event_obj){
	if(event_obj->readindex==event_obj->writeindex){
		return EVENT_SUCCESS;
	}
	return EVENT_FLASE;
}

//�����¼����¼�������λ��
uint8_t SentEventToQueue(EventQueue_Typdef *event_obj,uint8_t ID,uint8_t *eventbuf,uint32_t *buf_len){
	if(EventQueueIsFull(event_obj)==EVENT_SUCCESS){
		printf("EVENT_BUF is full\r\n");
		return EVENT_FLASE;
	}
	if(*buf_len>EVENT_MEMBER_BUF_LEN){
		printf("EVENT_MEMBER_BUF_LEN is overlayout\r\n");
		return EVENT_FLASE;
	}
	
	event_obj->member[event_obj->writeindex].BusyOrFree=EVENT_MEMBER_BUSY;
	event_obj->member[event_obj->writeindex].EvenID=ID;
	if(event_obj->member[event_obj->writeindex].BufLen!=0){
		memcpy(event_obj->member[event_obj->writeindex].EventBuf,eventbuf,*buf_len);
	}
	event_obj->writeindex=(event_obj->writeindex+1)%event_obj->size;
	event_obj->currentcount++;
	return EVENT_SUCCESS;
}

//���¼�������λ����ȡ��һ���¼�
uint8_t Read_Event(EventQueue_Typdef *event_obj,EventMember_Typedef *event_member){
	if(EventQueueIsEmpty(event_obj)==EVENT_SUCCESS){
			printf("EVENT_BUF is empty\r\n");
		return EVENT_FLASE;
	}
	memcpy(event_member,&event_obj->member[event_obj->readindex],sizeof(EventMember_Typedef));//ȡ���¼�
	event_obj->readindex=(event_obj->readindex+1)%event_obj->size;
	event_obj->currentcount--;
	return EVENT_SUCCESS;
}

uint8_t EventCheckCallback(EventQueue_Typdef *event_obj){
	int16_t adc_value;
	EventMember_Typedef member;
	if(EVENT_SUCCESS==Check_Event(&member)){
		switch(member.EvenID){
			case EVENT_NO_EVENT://û���¼�
				break;
			case ADC_SAMPLE_ORDER://ADC�ɼ��������һ��ADC�������������ݴ���˫����Ĵ��������˫����Ĵ���
				if(AD7606_Read8Channel(&adc_value)==AD7606_SUCESS){
						DoubleBuf_LoadData(&DoubuleBuf,adc_value);
				}else{
					printf("%s %d ADC����ʧ��",__FILE__,__LINE__);
					return EVENT_FLASE;
				}
				break;
				
				default:{
					break;
				}
		}
	}
	return EVENT_SUCCESS;
}


//���¼�������λ���м���¼�
__weak uint8_t Check_Event(EventMember_Typedef *event_member){
		

	return EVENT_SUCCESS;
}






