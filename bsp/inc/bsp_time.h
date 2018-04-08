#ifndef _BSP_TIME__
#define _BSP_TIME__
#include "tim.h"

#define TIME2_FREUENCEY 1000  	//定时器3频率中断为1Khz  1ms
#define TIME3_FREUENCEY 100000  //定时器3频率中断为100Khz  10us




typedef struct{
		uint32_t adc_frequency;//ADC的采样频率 单位hz
		uint32_t adc_fre_counter;
		uint32_t adc_fre_threshold;
}Time3_Typedef;

extern Time3_Typedef Time3;

extern uint8_t timer2_bussy;
extern uint8_t timer3_bussy;



extern int64_t gDelay_Coutner;


void DelayUs(uint32_t value);
void DelayMs(uint32_t value);
void NopDelay(int32_t n);
#endif

