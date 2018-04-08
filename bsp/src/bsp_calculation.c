#include "bsp_calculation.h"




Calculation_Typedef Calculation;






//算发层初始化
uint8_t CalculationInit(Calculation_Typedef *obj){
	uint8_t i;
	for(i=0;i<8;i++){
		obj->dataoutputreg[i]=0;
		obj->datainputreg[i]=0;
	}
	obj->bussy=CALCULATION_NOBUSSY;

	return CALCULATION_SUCCESS;
}


//求平均


uint8_t AdcAverage(Calculation_Typedef *obj,uint32_t times){
	uint8_t i;
	if(obj->bussy==CALCULATION_NOBUSSY){
		obj->bussy=CALCULATION_BUSSY;
		//求平均
		for(i=0;i<8;i++){
			obj->dataoutputreg[i]=(int16_t)(obj->datainputreg[i]/times);//计算平均值
			//计算电压值
			//this->voltage[i]=((float)((float)this->adc_ch[i]*this->ref_voltage))/65536;//设置电压值
			
			
			
			//填充传输buf
			obj->dataoutputbuf[2*i]=(uint8_t)(obj->dataoutputreg[i]&0x00ff);
			obj->dataoutputbuf[2*i+1]=(uint8_t)((obj->dataoutputreg[i]>>8)&0x00ff);
		}
		return CALCULATION_SUCCESS;
	}
	return CALCULATION_FALSE;
}
				


