#ifndef __BSP_ADDER_H__
#define __BSP_ADDER_H__

#include "stm32f4xx_hal.h"
#include "bsp_Ad7606.h"


#define ADCADDER_SUCCESS 0
#define ADCADDER_FALSE 1


#define ADCADDER_EMPTY 0
#define ADCADDER_FULL 1


typedef struct{

	//8通道累加寄存器
	int64_t DataAdder[8];
	//累加器状态标志寄存器
	int8_t EmptyOrFull;
	
	uint32_t AdderTimesCunter;
	uint32_t AdderTimes;
	uint32_t AdderTimesThreshold;
	
	
}AdcDataAdder_Typedef;




extern AdcDataAdder_Typedef AdcDataAdder;


//初始化
void AdcAdder_Init(AdcDataAdder_Typedef *obj,uint32_t threshold);

//数据输入
int8_t AdcAdder_DataInput(AdcDataAdder_Typedef *obj,uint8_t *bytes,uint8_t channels);

//累加输出


int8_t AdcAdder_DataOutput(AdcDataAdder_Typedef *obj,int64_t *output,uint8_t channels);

#endif


