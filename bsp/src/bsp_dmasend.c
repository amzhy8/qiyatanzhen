#include "bsp_dmasend.h"
#include <string.h>
#include "bsp_Ad7606.h"

DmaSendTypedef DmaSendMoudle;

//初始化
void DmaSendInit(DmaSendTypedef *obj){
	obj->dmabussy=DMA_BUSSY;
	memset(obj->sendbuf,0,DMASEND_INPUTBUF_LEN);
	obj->dmabussy=DMA_NOBUSSY;
}



//设置DMA发送忙标志
void DmaSendSetBussy(DmaSendTypedef *obj){
		obj->dmabussy=DMA_BUSSY;
}

//设置DMA发送忙标志
void DmaSendSetNoBussy(DmaSendTypedef *obj){
		obj->dmabussy=DMA_NOBUSSY;
}

//DMA发送数据
uint8_t DmaSend(DmaSendTypedef *obj,uint8_t *buf,uint16_t len){
	


	//先检查Dma是否处于发送状态
	if(obj->dmabussy==DMA_BUSSY){
		return DMASEND_FALSE;
	}

	//拷贝远端数据到DMA模块发送缓冲器
	memcpy(obj->sendbuf,buf,len);
	//设置DMA忙状态     此处需要在DMA发送完成以后清空该标志
	obj->dmabussy=DMA_BUSSY;

	//开始发送
	Uart2_Send(obj->sendbuf,len);
	return DMASEND_SUCCESS;
}


//DMA发送数据
uint8_t DmaSendTask(DmaSendTypedef *obj){
	


	//先检查Dma是否处于发送状态
	if(obj->dmabussy==DMA_BUSSY){
		return DMASEND_FALSE;
	}

	//拷贝远端数据到DMA模块发送缓冲器
	memcpy(obj->sendbuf,obj->sendbuf,obj->len);
	//设置DMA忙状态     此处需要在DMA发送完成以后清空该标志
	obj->dmabussy=DMA_BUSSY;

	//开始发送
	Uart2_Send(obj->sendbuf,obj->len);
	return DMASEND_SUCCESS;
}


void DmaSendCplt(struct __DMA_HandleTypeDef *obj){
	//DmaSendSetNoBussy(&DmaSendMoudle);
		DmaSendMoudle.dmabussy=DMA_NOBUSSY;
		

}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  DmaSendMoudle.dmabussy=DMA_NOBUSSY;
	//Adc_SampleOneTimeFlag=0;
}


