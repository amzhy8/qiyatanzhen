#include "bsp_dataprint.h"
#include <string.h>
#include "bsp_calculation.h"
#include "bsp_config.h"

#include "bsp_InOutBuf.h"
#include "bsp_dmasend.h"
#include "bsp_buildframe.h"

uint8_t test[]="0123456789abcdefghijk\n";

//默认参数
DataPrintTimer_Typedef DataPrintTimer_obj;


uint8_t DataPrintInit(Bsp_Config_Typedef *Bsp_Config_obj,
											DataPrintTimer_Typedef *DataPrintTimer_obj,
											uint8_t output_datamode,
											uint8_t unit,
											uint8_t decimals,
											uint32_t frame_fre,
											uint8_t circle_output,
											uint8_t start_stop){
	DataPrintTimer_obj->timerfreuency=1000;
	DataPrintSetOutputTask(Bsp_Config_obj,DataPrintTimer_obj,output_datamode,unit,decimals,frame_fre,circle_output,start_stop);		
	DataPrintEnableOutputTask(Bsp_Config_obj,DATAPRINT_OUTPUTTASK_ENABLE);//擦除输出任务
	return DATAPRINT_SUCCESS;									
}
//打印输出
uint8_t DataPrintSend(uint8_t *buf,uint16_t len){
	return DATAPRINT_SUCCESS;	
}
//串口2输出
void DataPrintFrameSend(uint8_t *frame,uint16_t len){
	Uart2_Send(frame,len);
	Uart1_Send(frame,len);
}
//组建帧
void DataPrintFrameBuild(int16_t *adcdata,uint8_t ch,uint8_t *frame){
	uint16_t i=0;
	uint8_t pority;
	frame[DATAPRIINT_FRAME_HEAD_INDEX]=DATAPRIINT_FRAME_HEAD;
	frame[DATAPRIINT_FRAME_HEAD_INDEX+ch*2+2]=DATAPRIINT_FRAME_TAIL;
	pority=frame[0];
	for(i=0;i<ch;i++){
		frame[DATAPRIINT_FRAME_DATA_INDEX+2*i]=(uint8_t)(adcdata[i]&0x00ff);
		frame[DATAPRIINT_FRAME_DATA_INDEX+2*i+1]=(uint8_t)((adcdata[i]>>8)&0x00ff);
	}
	//memcpy((uint8_t *)&frame[DATAPRIINT_FRAME_DATA_INDEX],(uint8_t *)adcdata,sizeof(int16_t)*ch);
	for(i=1;i<ch*2+1;i++){
		pority^=frame[i];
	}
	frame[ch*2+1]=pority;
}

//使能输出任务
uint8_t DataPrintEnableOutputTask(Bsp_Config_Typedef *Bsp_Config_obj,uint8_t flag){
	Bsp_Config_obj->output_task=flag;
	return DATAPRINT_SUCCESS;	
}																											

