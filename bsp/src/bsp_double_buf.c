#include "bsp_double_buf.h"
#include <string.h>

//����buf�ռ�
uint8_t DoubleBuf_buf1[DOUBLE_BUF_MAXDEEP]={0};
uint8_t DoubleBuf_buf2[DOUBLE_BUF_MAXDEEP]={0};

//���岢��ʼ��˫buf
DoubleBuf_Typedef DoubuleBuf={
	
	.buf1={
		.deep=SINGLE_BUF_DEEP_DEFAULT,
		.count=SINGLE_BUF_COUNT_DEFAULT,
		.index=SINGLE_BUF_INDEX_DEFAULT,
		.buf=DoubleBuf_buf1,
		.status=SINGLE_BUF_STATUS_DEFAULT
	},
	.buf2={
		.deep=SINGLE_BUF_DEEP_DEFAULT,
		.count=SINGLE_BUF_COUNT_DEFAULT,
		.index=SINGLE_BUF_INDEX_DEFAULT,
		.buf=DoubleBuf_buf2,
		.status=SINGLE_BUF_STATUS_DEFAULT
	},
	.buf_load_index=LOAD_INDEX_BUF1,
	.Get_Double_Deep_Callback=NULL//ע������˫����������ȵĻص�����
};

//��λ��������
static void ResetSingleBuf(SingleBuf_Typedef *buf_obj){
	//memset(&buf_obj->buf[0],0,buf_obj->deep);  //�����ȡ�������ж��е��� ע�͵����У���Ϊ����ʱ�����
	buf_obj->count=0;
	buf_obj->index=0;
	buf_obj->status=SINGLE_BUF_IS_EMPTY;
}

//��λ˫������
static void ResetDoubleBuf(DoubleBuf_Typedef *buf_obj){
	ResetSingleBuf(&buf_obj->buf1);
	ResetSingleBuf(&buf_obj->buf2);
	buf_obj->buf_load_index=LOAD_INDEX_BUF1;
}

//��ʼ��˫������
void DoubleBuf_Init(DoubleBuf_Typedef *buf_obj){
	//����˫�������������
	uint32_t deep=buf_obj->Get_Double_Deep_Callback();
	buf_obj->buf1.deep=deep;
	buf_obj->buf2.deep=deep;
	ResetDoubleBuf(buf_obj);
}

//װ�����ݺ���  ��ʱ�ж����ò����¼�    ����������װ��
uint8_t DoubleBuf_LoadData(DoubleBuf_Typedef *dbuf,uint8_t data){
	SingleBuf_Typedef *buf_obj;
	
	if(dbuf->buf_load_index==LOAD_INDEX_BUF1){//Ŀǰ����װ��Buf1
		if(dbuf->buf1.status==SINGLE_BUF_IS_FULL){//���װ���ˣ��л�װ��ָ�뵽buf2
			dbuf->buf_load_index=LOAD_INDEX_BUF2;//�л�װ��buf1��buf2
			buf_obj=&dbuf->buf2;
		}
	}else{ 
		if(dbuf->buf2.status==SINGLE_BUF_IS_FULL){//���װ���ˣ��л�װ��ָ�뵽buf1
			dbuf->buf_load_index=LOAD_INDEX_BUF1;//�л�װ��buf2��buf1
			buf_obj=&dbuf->buf1;
		}
	}
	if(buf_obj->count==buf_obj->deep){
		return DOUBLEBUF_FALSE;
	}
	//װ������
	buf_obj->buf[buf_obj->index++]=data;
	buf_obj->count++;
	if(buf_obj->count==buf_obj->deep){buf_obj->status=SINGLE_BUF_IS_FULL;}
	return DOUBLEBUF_SUCESS;
}


//��˫�����ж�ȡ����
uint8_t DoubleBuf_ReadData(DoubleBuf_Typedef *dbuf,SingleBuf_Typedef *buf_obj){
	//�жϷǷ�
	uint8_t res=DOUBLEBUF_FALSE;
	if(dbuf->buf1.status==SINGLE_BUF_IS_FULL){
		memcpy(buf_obj->buf,dbuf->buf1.buf,dbuf->buf1.deep);//��ȡ����
		ResetSingleBuf(&dbuf->buf1);//�ָ�״̬
		res=DOUBLEBUF_SUCESS;
	}else if(dbuf->buf2.status==SINGLE_BUF_IS_FULL){
		memcpy(buf_obj->buf,dbuf->buf2.buf,dbuf->buf2.deep);//��ȡ����
		ResetSingleBuf(&dbuf->buf2);	//�ָ�״̬
		res=DOUBLEBUF_SUCESS;
	}else{
		res=DOUBLEBUF_FALSE;
	}
	return res;
}



//�����Ҫ���û��������   ��Ҫд�ú���  �����³�ʼ��˫������
__weak uint32_t Get_Double_Deep_Callback(void){

	return SINGLE_BUF_DEEP_DEFAULT;
}

