#include "bsp_single_buf.h"

SingleBuf_Typedef SingleBuf;

uint8_t InitSingleBuf(SingleBuf_Typedef *singlebuf,uint16_t deep,uint16_t member_len){
	singlebuf->fullorempty=SIGNLE_BUF_EMPTY;
	singlebuf->buf_deep=deep;//Buf���
	singlebuf->buf_member_len=member_len;
	singlebuf->buf_index=0;
	singlebuf->buf_cnt=0;
	memset((uint8_t *)singlebuf->buf,0,sizeof(int16_t)*singlebuf->buf_deep*singlebuf->buf_member_len);
	return SIGNLE_BUF_SUCCESS;
}

//��ʼ��
uint8_t ReInitSingleBuf(SingleBuf_Typedef *singlebuf){
	singlebuf->fullorempty=SIGNLE_BUF_EMPTY;
	singlebuf->buf_index=0;
	singlebuf->buf_cnt=0;
	memset(singlebuf->buf,0,sizeof(int16_t)*singlebuf->buf_member_len*singlebuf->buf_deep);
	return SIGNLE_BUF_SUCCESS;
}

//���BUF�Ƿ��Ѿ�����
uint8_t SingleBufIsFull(SingleBuf_Typedef *singlebuf){
	if(singlebuf->buf_cnt>=(singlebuf->buf_deep*singlebuf->buf_member_len)){
		return SIGNLE_BUF_SUCCESS;
	}else{
		return SIGNLE_BUF_FALSE;
	}
}
//װ��һ������
uint8_t SingleBufLoadData(SingleBuf_Typedef *singlebuf,int16_t *data){
	if(SingleBufIsFull(singlebuf)==SIGNLE_BUF_SUCCESS){//����Ƿ�����
		return SIGNLE_BUF_FALSE;
	}
	memcpy((uint8_t *)&(singlebuf->buf[singlebuf->buf_index]),(uint8_t *)data,sizeof(uint16_t)*(singlebuf->buf_member_len));
	singlebuf->buf_cnt+=singlebuf->buf_member_len;
	singlebuf->buf_index+=singlebuf->buf_member_len;
	if(SingleBufIsFull(singlebuf)==SIGNLE_BUF_SUCCESS){//����Ƿ�����
		singlebuf->fullorempty=SIGNLE_BUF_FULL;
	}
	return SIGNLE_BUF_SUCCESS;
}

//����������
uint8_t SingleBufReadAll(SingleBuf_Typedef *singlebuf, int16_t *data){
	//���ȼ�⻺�����Ƿ�����
	if(SingleBufIsFull(singlebuf)==SIGNLE_BUF_SUCCESS){//��������	
		memcpy((uint8_t *)data,(uint8_t *)singlebuf->buf,sizeof(uint16_t)*(singlebuf->buf_deep)*(singlebuf->buf_member_len));
		ReInitSingleBuf(singlebuf);//��λBUF���в���
		return SIGNLE_BUF_SUCCESS;
	}
	return SIGNLE_BUF_FALSE;
}

