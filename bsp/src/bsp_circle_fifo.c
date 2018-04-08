
#include "bsp_circle_fifo.h"
#include <string.h>

CircleFifo_Typedef CircleFifo;





//��ʼ��  ���û��λ��������  ��Ա����
uint8_t CircleFifoInit(CircleFifo_Typedef *obj,uint32_t fifodeep,uint32_t memberlen){
	uint32_t i;
	obj->bussy_flag=CIRCLE_FIFO_BUSSY;
	obj->buf_cnt=0;
	obj->fifo_deep=fifodeep+1;
	obj->fifo_member_len=memberlen;
	obj->fifo_writeindex=0;
	obj->fifo_readindex=0;
	
	for(i=0;i<fifodeep;i++){
		memset(obj->buf[i],0,memberlen);
	}

	obj->bussy_flag=CIRCLE_FIFO_NOBUSSY;
	return CIRCLE_FIFO_SUCCESS;
}



//��������
uint8_t CircleFifoInput(CircleFifo_Typedef *obj,uint8_t *data,uint32_t len){
	obj->bussy_flag=CIRCLE_FIFO_BUSSY;//��ȡ�ź���
	if(obj->fifo_readindex==(obj->fifo_writeindex+1)%obj->fifo_deep){//FIFO�Ѿ����� �������������
		memcpy(&obj->buf[obj->fifo_writeindex][0],data,len*sizeof(uint8_t));//д������
		obj->bufmemberlen[obj->fifo_writeindex]=len;
		obj->fifo_readindex=(obj->fifo_readindex+1)%obj->fifo_deep;
		obj->fifo_writeindex=(obj->fifo_writeindex+1)%obj->fifo_deep;
	}else{
		memcpy(&obj->buf[obj->fifo_writeindex][0],data,len*sizeof(uint8_t));//д������
		obj->bufmemberlen[obj->fifo_writeindex]=len;
		//obj->fifo_readindex=(obj->fifo_readindex+1)%obj->fifo_deep;
		obj->fifo_writeindex=(obj->fifo_writeindex+1)%obj->fifo_deep;
	}
	obj->bussy_flag=CIRCLE_FIFO_NOBUSSY;//�ͷ��ź���
	return CIRCLE_FIFO_SUCCESS;
}


//��������
uint8_t CircleFifoOutput(CircleFifo_Typedef *obj,uint8_t *data,uint32_t *len){
	if(obj->bussy_flag==CIRCLE_FIFO_BUSSY){//�ź�����ȡʧ��
		return CIRCLE_FIFO_FALSE;
	}
	obj->bussy_flag=CIRCLE_FIFO_BUSSY;//��ȡ�ź���
	
	if(obj->fifo_readindex==obj->fifo_writeindex){//fifoΪ��
		obj->bussy_flag=CIRCLE_FIFO_NOBUSSY;//�ͷ��ź���
		return CIRCLE_FIFO_FALSE;
	}
	
	//��������
	memcpy(data,&obj->buf[obj->fifo_readindex][0],obj->bufmemberlen[obj->fifo_readindex]);
	*len=obj->bufmemberlen[obj->fifo_readindex];
	//��ָ��ǰ��
	obj->fifo_readindex=(obj->fifo_readindex+1)%obj->fifo_deep;
	
	obj->bussy_flag=CIRCLE_FIFO_NOBUSSY;//�ͷ��ź���
	return CIRCLE_FIFO_SUCCESS;
}
