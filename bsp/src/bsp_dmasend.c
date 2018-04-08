#include "bsp_dmasend.h"
#include <string.h>
#include "bsp_Ad7606.h"

DmaSendTypedef DmaSendMoudle;

//��ʼ��
void DmaSendInit(DmaSendTypedef *obj){
	obj->dmabussy=DMA_BUSSY;
	memset(obj->sendbuf,0,DMASEND_INPUTBUF_LEN);
	obj->dmabussy=DMA_NOBUSSY;
}



//����DMA����æ��־
void DmaSendSetBussy(DmaSendTypedef *obj){
		obj->dmabussy=DMA_BUSSY;
}

//����DMA����æ��־
void DmaSendSetNoBussy(DmaSendTypedef *obj){
		obj->dmabussy=DMA_NOBUSSY;
}

//DMA��������
uint8_t DmaSend(DmaSendTypedef *obj,uint8_t *buf,uint16_t len){
	


	//�ȼ��Dma�Ƿ��ڷ���״̬
	if(obj->dmabussy==DMA_BUSSY){
		return DMASEND_FALSE;
	}

	//����Զ�����ݵ�DMAģ�鷢�ͻ�����
	memcpy(obj->sendbuf,buf,len);
	//����DMAæ״̬     �˴���Ҫ��DMA��������Ժ���ոñ�־
	obj->dmabussy=DMA_BUSSY;

	//��ʼ����
	Uart2_Send(obj->sendbuf,len);
	return DMASEND_SUCCESS;
}


//DMA��������
uint8_t DmaSendTask(DmaSendTypedef *obj){
	


	//�ȼ��Dma�Ƿ��ڷ���״̬
	if(obj->dmabussy==DMA_BUSSY){
		return DMASEND_FALSE;
	}

	//����Զ�����ݵ�DMAģ�鷢�ͻ�����
	memcpy(obj->sendbuf,obj->sendbuf,obj->len);
	//����DMAæ״̬     �˴���Ҫ��DMA��������Ժ���ոñ�־
	obj->dmabussy=DMA_BUSSY;

	//��ʼ����
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


