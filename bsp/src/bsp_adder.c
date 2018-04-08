
#include "bsp_adder.h"
#include <string.h>
AdcDataAdder_Typedef AdcDataAdder;


//初始化
void AdcAdder_Init(AdcDataAdder_Typedef *obj,uint32_t threshold){
	int8_t i;
	obj->EmptyOrFull=ADCADDER_EMPTY;
	obj->AdderTimes=0;
	obj->AdderTimesCunter=0;
	obj->AdderTimesThreshold=threshold;
	for(i=0;i<ADC_CHANNEL_CNT;i++){
		obj->DataAdder[i]=0;
	}
}

//数据输入
int8_t AdcAdder_DataInput(AdcDataAdder_Typedef *obj,uint8_t *bytes,uint8_t channels){
	__IO int16_t tmp=0;
	__IO int16_t tmp2=0;
	__IO  uint8_t i=0;
	
	if(obj->EmptyOrFull==ADCADDER_EMPTY){
		
		for(i=0;i<channels;i++){
			tmp=0;
			tmp2=bytes[2*i];
			tmp|=(tmp2<<8);
			tmp2=bytes[2*i+1];
			tmp|=tmp2;
			obj->DataAdder[i]+=tmp;
		}
		//累加次数自加
		obj->AdderTimesCunter++;
		
		//判断累加次数是否到达设定值
		if(obj->AdderTimesCunter==obj->AdderTimesThreshold){
			obj->AdderTimesCunter=0;

			obj->EmptyOrFull=ADCADDER_FULL;
			
		}
	}else{
		return ADCADDER_FALSE;
	}
	return ADCADDER_SUCCESS;
}




//累加输出
int8_t AdcAdder_DataOutput(AdcDataAdder_Typedef *obj,int64_t *output,uint8_t channels){
	uint8_t i;
	if(obj->EmptyOrFull==ADCADDER_FULL){
		for(i=0;i<channels;i++){
			output[i]=obj->DataAdder[i];
			obj->DataAdder[i]=0;
		}
		obj->EmptyOrFull=ADCADDER_EMPTY;
		return ADCADDER_SUCCESS;
	}
	return ADCADDER_FALSE;
}
