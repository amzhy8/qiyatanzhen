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


//写数据
uint8_t InOutBuf_Write(InOutList_Typedef *obj,int16_t *point){
	if(obj->key==INOUTBUF_BUSSY){
		return INOUTBUF_FALSE;
	}else{
		obj->key=INOUTBUF_BUSSY;
	}
	
	
	if((obj->InputInidex+1)%obj->inoutbuflen==obj->OutputIndex){//buf已经满 覆盖前面的数据
		memcpy(obj->buf[obj->InputInidex],point,sizeof(int16_t)*obj->inoutbufmemlen);//写数据
		obj->OutputIndex=(obj->OutputIndex+1)%obj->inoutbuflen;//Output指针前移
		obj->InputInidex=(obj->InputInidex+1)%obj->inoutbuflen;//Input指针前移
	}else{
		memcpy(obj->buf[obj->InputInidex],point,sizeof(int16_t)*obj->inoutbufmemlen);//写数据
		obj->InputInidex++;////Input指针前移
	}
	obj->key=INOUTBUF_NOBUSSY;
	return INOUTBUF_SUCCESS;
}

//读数据
uint8_t InOutBuf_Read(InOutList_Typedef *obj,int16_t *point){
	if(obj->key==INOUTBUF_BUSSY){
		return INOUTBUF_FALSE;
	}else{
		obj->key=INOUTBUF_BUSSY;
	}

	if(obj->InputInidex==obj->OutputIndex){//buf为空
		obj->key=INOUTBUF_NOBUSSY;
		return INOUTBUF_FALSE;
	}
	//如果只有一个数据  只读数据不前移指针
	if((obj->OutputIndex+1)%obj->inoutbuflen==obj->InputInidex){
		memcpy(point,obj->buf[obj->OutputIndex],sizeof(int16_t)*obj->inoutbufmemlen);//读数据
		obj->key=INOUTBUF_NOBUSSY;
		return INOUTBUF_SUCCESS;
	}
	
	memcpy(point,obj->buf[obj->OutputIndex],sizeof(int16_t)*obj->inoutbufmemlen);//读数据
	obj->OutputIndex=(obj->OutputIndex+1)%obj->inoutbuflen;//指针前移
	obj->key=INOUTBUF_NOBUSSY;
	return INOUTBUF_SUCCESS;
}

