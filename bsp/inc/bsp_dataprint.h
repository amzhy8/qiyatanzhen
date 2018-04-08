#ifndef _BSP_DATAPRINT_H__
#define _BSP_DATAPRINT_H__

#include "bsp_uart.h"
#include "bsp_calculation.h"

#define DATAPRINT_SUCCESS 0x00
#define DATAPRINT_FALSE 0x01

#define DATAPRINT_OUTPUTTASK_ENABLE 0x00
#define DATAPRINT_OUTPUTTASK_DISABLE 0x01

#define DATAPRINT_MODE_FRAME 0x00
#define DATAPRINT_MODE_ASSIC 0x01

#define DATAPRINT_UNIT_MV 0x00
#define DATAPRINT_UNIT_V  0x01

#define DATAPRINT_DECIMALS_0  0x00
#define DATAPRINT_DECIMALS_1  0x01
#define DATAPRINT_DECIMALS_2  0x02
#define DATAPRINT_DECIMALS_3  0x03
#define DATAPRINT_DECIMALS_4  0x04


#define DATAPRINT_FRE_MAX CONFIG_DATAPRINT_FRE_MAX 
#define DATAPRINT_FRE_MIN CONFIG_DATAPRINT_FRE_MIN 

#define DATAPRINT_FRE_DEFAULT 1

#define DATAPRIINT_CIRCLE BSP_CONFIG_CIRCLE_COUTPUT
#define DATAPRIINT_SINGLE BSP_CONFIG_SINGLE_COUTPUT





#define DATAPRIINT_START BSP_CONFIG_OUTPUT_START
#define DATAPRIINT_STOP BSP_CONFIG_OUTPUT_STOP



#define DATAPRINT_FRAME_LEN 13

#define DATAPRIINT_FRAME_HEAD 0xA5
#define DATAPRIINT_FRAME_TAIL 0x5A

#define DATAPRIINT_FRAME_HEAD_INDEX 0
#define DATAPRIINT_FRAME_DATA_INDEX 1
#define DATAPRIINT_FRAME_XOR_INDEX 11



/*
typedef struct{
	uint8_t output_datamode;//结果输出数据模式  //ASSIC明码模式   //数据帧模式
	uint8_t unit;//输出结果单位
	uint8_t decimals;//小数位数
	uint8_t frame_fre;//输出帧率   0~600
	uint8_t circle_output;//循环输出位
	uint8_t start_stop;//开始输出  
	
	uint8_t output_task;//输出使能标志位

}DataPrint_Typedef;

*/
/*

typedef __packed struct{
	uint8_t head;
	uint8_t data[5];
	uint8_t pority;
	uint8_t tail;
}DataPrintFrame_Typedef;
*/

typedef struct{
	uint32_t Counter;
	uint32_t threshold;
	uint32_t timerfreuency;
}DataPrintTimer_Typedef;



//extern DataPrint_Typedef DataPrint_obj;
extern DataPrintTimer_Typedef DataPrintTimer_obj;

void DataPrintFrameSend(uint8_t *frame,uint16_t len);
void DataPrintFrameBuild(int16_t *adcdata,uint8_t ch,uint8_t *frame);
uint8_t DataPrintSetOutputTask(Bsp_Config_Typedef *Bsp_Config_obj,DataPrintTimer_Typedef *DataPrintTimer_obj,	
															uint8_t output_datamode,
															uint8_t unit,
															uint8_t decimals,
															uint32_t frame_fre,
															uint8_t circle_output,
															uint8_t start_stop);
uint8_t DataPrintModifyOutputFre(Bsp_Config_Typedef *Bsp_Config_obj,DataPrintTimer_Typedef *DataPrintTimer_obj,uint32_t frame_fre);
uint8_t DataPrintTimCallback(Bsp_Config_Typedef *Bsp_Config_obj,DataPrintTimer_Typedef *DataPrintTimer_obj);
uint8_t DataPrintEnableOutputTask(Bsp_Config_Typedef *Bsp_Config_obj,uint8_t flag);
uint8_t DataPrintInit(Bsp_Config_Typedef *Bsp_Config_obj,DataPrintTimer_Typedef *DataPrintTimer_obj,uint8_t output_datamode,
															uint8_t unit,
															uint8_t decimals,
															uint32_t frame_fre,
															uint8_t circle_output,
															uint8_t start_stop);
#endif

