#ifndef __BSP_ADDER_H__
#define __BSP_ADDER_H__

#include "stm32f4xx_hal.h"
#include "bsp_Ad7606.h"


#define ADCADDER_SUCCESS 0
#define ADCADDER_FALSE 1


#define ADCADDER_EMPTY 0
#define ADCADDER_FULL 1


typedef struct{

	//8ͨ���ۼӼĴ���
	int64_t DataAdder[8];
	//�ۼ���״̬��־�Ĵ���
	int8_t EmptyOrFull;
	
	uint32_t AdderTimesCunter;
	uint32_t AdderTimes;
	uint32_t AdderTimesThreshold;
	
	
}AdcDataAdder_Typedef;




extern AdcDataAdder_Typedef AdcDataAdder;


//��ʼ��
void AdcAdder_Init(AdcDataAdder_Typedef *obj,uint32_t threshold);

//��������
int8_t AdcAdder_DataInput(AdcDataAdder_Typedef *obj,uint8_t *bytes,uint8_t channels);

//�ۼ����


int8_t AdcAdder_DataOutput(AdcDataAdder_Typedef *obj,int64_t *output,uint8_t channels);

#endif


