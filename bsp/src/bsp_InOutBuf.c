#include "bsp_InOutBuf.h"
#include <string.h>
InOutList_Typedef InOutBuf;

void InOutBufInit(InOutList_Typedef *obj){
	uint8_t i;
	obj->key=INOUTBUF_NOBUSSY;
	obj->inoutbuflen=INOUTBUF_LEN;
	obj->inoutbufmemlen=INOUTBUFMEM_LEN;
	obj->InputInidex=0;
	obj->OutputIndex=0;
	for(i=0;i<INOUTBUF_LEN;i++){
		memset(obj->buf[i],0,INOUTBUFMEM_LEN);
	}
}


//д����
uint8_t InOutBuf_Write(InOutList_Typedef *obj,int16_t *point){
	if(obj->key==INOUTBUF_BUSSY){
		return INOUTBUF_FALSE;
	}else{
		obj->key=INOUTBUF_BUSSY;
	}
	
	
	if((obj->InputInidex+1)%obj->inoutbuflen==obj->OutputIndex){//buf�Ѿ��� ����ǰ�������
		memcpy(obj->buf[obj->InputInidex],point,sizeof(int16_t)*obj->inoutbufmemlen);//д����
		obj->OutputIndex=(obj->OutputIndex+1)%obj->inoutbuflen;//Outputָ��ǰ��
		obj->InputInidex=(obj->InputInidex+1)%obj->inoutbuflen;//Inputָ��ǰ��
	}else{
		memcpy(obj->buf[obj->InputInidex],point,sizeof(int16_t)*obj->inoutbufmemlen);//д����
		obj->InputInidex++;////Inputָ��ǰ��
	}
	obj->key=INOUTBUF_NOBUSSY;
	return INOUTBUF_SUCCESS;
}

//������
uint8_t InOutBuf_Read(InOutList_Typedef *obj,int16_t *point){
	if(obj->key==INOUTBUF_BUSSY){
		return INOUTBUF_FALSE;
	}else{
		obj->key=INOUTBUF_BUSSY;
	}

	if(obj->InputInidex==obj->OutputIndex){//bufΪ��
		obj->key=INOUTBUF_NOBUSSY;
		return INOUTBUF_FALSE;
	}
	//���ֻ��һ������  ֻ�����ݲ�ǰ��ָ��
	if((obj->OutputIndex+1)%obj->inoutbuflen==obj->InputInidex){
		memcpy(point,obj->buf[obj->OutputIndex],sizeof(int16_t)*obj->inoutbufmemlen);//������
		obj->key=INOUTBUF_NOBUSSY;
		return INOUTBUF_SUCCESS;
	}
	
	memcpy(point,obj->buf[obj->OutputIndex],sizeof(int16_t)*obj->inoutbufmemlen);//������
	obj->OutputIndex=(obj->OutputIndex+1)%obj->inoutbuflen;//ָ��ǰ��
	obj->key=INOUTBUF_NOBUSSY;
	return INOUTBUF_SUCCESS;
}

