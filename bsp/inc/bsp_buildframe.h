#ifndef __BSP_BUILDFRAME_H__
#define __BSP_BUILDFRAME_H__

#include "stm32f4xx_hal.h"


#define TEMPERATURE_BUF_LEN 2
#define PRESSURE_BUF_LEN 2
#define ADC_AVERAGE_BUF_LEN 16


#define BUILDFRAMEBUFLEN 128



#define BUILDFRAME_BUSSY 1
#define BUILDFRAME_NOBUSSY 0



#define BUILDFRAME_SUCCESS 0
#define BUILDFRAME_FALSE 1






//帧格式索引

#define BUILD_FRAME_A_HEAD_DATASIZE 1
#define BUILD_FRAME_A_ADC_DATASIZE 10
#define BUILD_FRAME_A_PRESSURE_DATASIZE 2
#define BUILD_FRAME_A_PRE_TEMP_DATASIZE 2
#define BUILD_FRAME_A_TEMP_DATASIZE 2
#define BUILD_FRAME_A_XOR_DATASIZE 1
#define BUILD_FRAME_A_TAIL_DATASIZE 1

#define BUILD_FRAME_TOTAL_LEN (BUILD_FRAME_A_HEAD_DATASIZE+\
																BUILD_FRAME_A_ADC_DATASIZE+\
																BUILD_FRAME_A_PRESSURE_DATASIZE+\
																BUILD_FRAME_A_PRE_TEMP_DATASIZE+\
																BUILD_FRAME_A_TEMP_DATASIZE+\
																BUILD_FRAME_A_XOR_DATASIZE+\
																BUILD_FRAME_A_TAIL_DATASIZE)


#define BUILD_FRAME_A_HEAD_INDEX 0x00
#define BUILD_FRAME_A_ADC_INDEX (BUILD_FRAME_A_HEAD_INDEX+BUILD_FRAME_A_HEAD_DATASIZE)
#define BUILD_FRAME_A_PRESSURE_INDEX (BUILD_FRAME_A_ADC_INDEX+BUILD_FRAME_A_ADC_DATASIZE)
#define BUILD_FRAME_A_PRE_TEMP_INDEX (BUILD_FRAME_A_PRESSURE_INDEX+BUILD_FRAME_A_PRESSURE_DATASIZE)
#define BUILD_FRAME_A_TEMP_INDEX (BUILD_FRAME_A_PRE_TEMP_INDEX+BUILD_FRAME_A_PRE_TEMP_DATASIZE)
#define BUILD_FRAME_A_XOR_INDEX (BUILD_FRAME_A_TEMP_INDEX+BUILD_FRAME_A_TEMP_DATASIZE)
#define BUILD_FRAME_A_TAIL_INDEX (BUILD_FRAME_A_XOR_INDEX+BUILD_FRAME_A_XOR_DATASIZE)


#define BUILD_FRAME_A_HEAD 0xA5
#define BUILD_FRAME_A_TAIL 0x5A


#define PRESSURE_DATA_LEN 2
#define PRESSURETEMP_DATA_LEN 2
#define TEMPRATURE_DATA_LEN 2


typedef struct{
	uint8_t pressure[PRESSURE_DATA_LEN];
	uint8_t pressurelen;
	
	uint8_t pressuretemp[PRESSURETEMP_DATA_LEN];
	uint8_t pressuretemplen;
	
	uint8_t temperature[TEMPRATURE_DATA_LEN];
	uint8_t temperaturelen;
}BuildFrameTmpData_Typedef;



typedef struct{
	BuildFrameTmpData_Typedef tmpdata;//用于数据临时输入

	
	uint16_t framelen;
	uint8_t BuildFrameOutputBuf[BUILDFRAMEBUFLEN];
	uint8_t BuildFrameOutputShadowBuf[BUILDFRAMEBUFLEN];
	
	uint8_t BuildFrameSendBuf[BUILDFRAMEBUFLEN];
	
	uint8_t inputbussy;
}BuildFrame_Typedef;

extern BuildFrame_Typedef BuildFrameC;


void BuildFrame_Init(BuildFrame_Typedef *obj);




//帧输入 单独输入每一项

uint8_t BuildFrameSingleInput(BuildFrame_Typedef *obj,uint8_t id,uint8_t *buf,uint8_t len);


//帧输入  全部输入
uint8_t BuildFrameAllInput(BuildFrame_Typedef *obj,uint8_t *adc_buf,uint16_t adclen,
																									 uint8_t *pressure_buf,uint16_t pressurelen,
																									 uint8_t *pressuretemp_buf,uint16_t pressuretemplen,
																									 uint8_t *temprature_buf,uint16_t temperaturelen);
//帧输出
uint8_t* BuildFrameOutput(BuildFrame_Typedef *obj);




//帧输入

//帧输出




#endif

