#ifndef __BSP_INOUTBUF_H__
#define __BSP_INOUTBUF_H__
#include "stm32f4xx_hal.h"


#define INOUTBUF_SUCCESS 0
#define INOUTBUF_FALSE 1

#define INOUTBUF_LEN 2
#define INOUTBUFMEM_LEN 8

#define INOUTBUF_NOBUSSY 0
#define INOUTBUF_BUSSY 1


typedef struct{
	uint16_t inoutbuflen;
	uint16_t inoutbufmemlen;
	int16_t buf[INOUTBUF_LEN][INOUTBUFMEM_LEN];
	uint16_t InputInidex;
	uint16_t OutputIndex;
	
	uint8_t key;
	
}InOutList_Typedef;


extern InOutList_Typedef InOutBuf;



void InOutBufInit(InOutList_Typedef *obj);

//写数据
uint8_t InOutBuf_Write(InOutList_Typedef *obj,int16_t *point);

//读数据
uint8_t InOutBuf_Read(InOutList_Typedef *obj,int16_t *point);



#endif

