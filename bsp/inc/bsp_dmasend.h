#ifndef __BSP_DMASEND_H__
#define __BSP_DMASEND_H__
#include "bsp_uart.h"



#define DMA_BUSSY 1
#define DMA_NOBUSSY 0

#define DMASEND_SUCCESS 0
#define DMASEND_FALSE 1



#define DMASEND_INPUTBUF_LEN 128

#define DMA_NOBUSSY 0
#define DMA_BUSSY 1



typedef struct{
	uint8_t dmabussy;	
	uint8_t sendbuf[DMASEND_INPUTBUF_LEN];
	uint32_t len;
}DmaSendTypedef;




extern DmaSendTypedef DmaSendMoudle;

void DmaSendInit(DmaSendTypedef *obj);

//设置DMA发送忙标志
//设置DMA发送忙标志
void DmaSendSetBussy(DmaSendTypedef *obj);
//设置DMA发送忙标志
void DmaSendSetNoBussy(DmaSendTypedef *obj);


//DMA发送数据
uint8_t DmaSend(DmaSendTypedef *obj,uint8_t *buf,uint16_t len);


void DmaSendCplt(struct __DMA_HandleTypeDef  *obj);




#endif

