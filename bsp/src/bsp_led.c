#include "bsp_led.h"
#include "tim.h"
#include "bsp_uart.h"

#define LED_TOGGLE_PERIOD 1000   //MS
uint32_t Led_Cunter=0;
 //uint8_t test[]="0123456789abcdefghijk\n";
void LED_Control(uint8_t state){
	if(state){
		HAL_GPIO_WritePin(LED_GPIO_PORT,LED_GPIO_PIN,GPIO_PIN_RESET);
	}else{
		HAL_GPIO_WritePin(LED_GPIO_PORT,LED_GPIO_PIN,GPIO_PIN_SET);
	}
}

static void LED_Toggle(void){
	HAL_GPIO_TogglePin(LED_GPIO_PORT,LED_GPIO_PIN);
}



void LED_Time_Callback(){
	if(Led_Cunter==LED_TOGGLE_PERIOD){
				LED_Toggle();
				Led_Cunter=0;
			}
			Led_Cunter++;
}





