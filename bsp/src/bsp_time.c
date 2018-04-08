#include "bsp_time.h"
#include "bsp_led.h"
#include "bsp_ad7606.h"
#include "bsp_dataprint.h"


int64_t gDelay_Coutner=0;

Time3_Typedef Time3={
	.adc_frequency=100000,
	.adc_fre_counter=0,//计数器
	.adc_fre_threshold=99
};

#define TIME_BUSSY 0
#define TIME_IDEL 1
 uint8_t timer2_bussy=TIME_IDEL;
 uint8_t timer3_bussy=TIME_IDEL;


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)  
{  
    if(htim->Instance==htim2.Instance)  
    {  					
					LED_Time_Callback();//呼吸灯

		}
		
		if(htim->Instance==htim3.Instance)  
    {  
					//AD采集
					if(Time3.adc_fre_counter==Time3.adc_fre_threshold){
					//	HAL_TIM_Base_Stop_IT(&htim3); 
						Time3.adc_fre_counter=0;
						ADC_Sample_Callback();
						//HAL_TIM_Base_Start_IT(&htim3); 
					}else{
						Time3.adc_fre_counter++;
					}
			} 	
}



void NopDelay(int32_t n){
	int32_t i;
	for(i=n;i>0;i--){
		__nop();
	}
}



/*
void DelayMs(uint32_t value){
	gDelay_Coutner=value;
	while(gDelay_Coutner>0);
}*/
