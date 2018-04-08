#ifndef BSP_CALCULATION_H__
#define BSP_CALCULATION_H__
#include "stm32f4xx_hal.h"
#include "bsp_Ad7606.h"
#include "bsp_uart.h"
#include "string.h"
#include   "bsp_config.h"





#define CALCULATION_SUCCESS 0
#define CALCULATION_FALSE   1


#define CALCULATION_EMPTY 0
#define CALCULATION_FULL 1


#define CALCULATION_BUSSY 1
#define CALCULATION_NOBUSSY 0


typedef struct{
	//数据输入寄存器
	int64_t datainputreg[8];
	int16_t dataoutputreg[8];
	float dataoutputvoltage[8];
	uint8_t dataoutputbuf[16];
	//Bussy标志
	int8_t bussy;
	uint32_t averagetimes;

}Calculation_Typedef;



extern Calculation_Typedef Calculation;



uint8_t CalculationInit(Calculation_Typedef *obj);
uint8_t AdcAverage(Calculation_Typedef *obj,uint32_t times);

#endif
