#ifndef __BSP_DOUBLE_BUF_H__
#define __BSP_DOUBLE_BUF_H__

#include "stm32f4xx_hal.h"

#define DOUBLEBUF_SUCESS 0
#define DOUBLEBUF_FALSE 1

//����������
#define DOUBLE_BUF_MAXDEEP 1000

//BUF״̬
#define SINGLE_BUF_IS_FULL 1
#define SINGLE_BUF_IS_EMPTY 0

//װ��ָ��
#define LOAD_INDEX_BUF1 0
#define LOAD_INDEX_BUF2 1


//Ĭ��ȱʡֵ
#define SINGLE_BUF_DEEP_DEFAULT  50
#define SINGLE_BUF_COUNT_DEFAULT 0
#define SINGLE_BUF_INDEX_DEFAULT LOAD_INDEX_BUF1
#define SINGLE_BUF_STATUS_DEFAULT SINGLE_BUF_IS_EMPTY
#define DOUBLE_BUF_INDEX_DEFAULT LOAD_INDEX_BUF1

//��������
typedef struct {
	uint32_t deep;//�������
	uint32_t count;//��ǰ����
	uint32_t index;//��ǰָ��
	uint8_t *buf;
	uint8_t status;//BUF��״̬
}SingleBuf_Typedef;


//˫������
typedef struct{
	SingleBuf_Typedef buf1;//buf1ʵ��
	SingleBuf_Typedef buf2;//buf2ʵ��
	uint8_t buf_load_index;//װ��ָ��
	uint32_t (*Get_Double_Deep_Callback)(void);
}DoubleBuf_Typedef;


extern DoubleBuf_Typedef DoubuleBuf;
extern uint8_t DoubleBuf_buf1[DOUBLE_BUF_MAXDEEP];
extern uint8_t DoubleBuf_buf2[DOUBLE_BUF_MAXDEEP];

static void ResetSingleBuf(SingleBuf_Typedef *buf_obj);//��λ��������
static void ResetDoubleBuf(DoubleBuf_Typedef *buf_obj);//��λ˫������
void DoubleBuf_Init(DoubleBuf_Typedef *buf_obj);//��ʼ��˫������
uint8_t DoubleBuf_LoadData(DoubleBuf_Typedef *dbuf,uint8_t data);
uint8_t DoubleBuf_ReadData(DoubleBuf_Typedef *dbuf,SingleBuf_Typedef *buf_obj);
uint32_t Get_Double_Deep_Callback(void);
#endif

