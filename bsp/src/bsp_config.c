#include   "bsp_config.h"
#include "bsp_interal_flash.h"

Bsp_Config_Typedef Bsp_Config;

uint8_t Bsp_Config_NeedReinitAll=BSP_CONFIG_NONEED_REINIT_ALL;

Bsp_Config_Typedef Bsp_Config_default={
	//更新标志位
	.update_flag=BSP_CONFIG_TABLE_FLAG,
	.range=CONFIG_ADC_RANGE_5,
	//输出结果控制相关

	.print_start_stop=BSP_CONFIG_OUTPUT_START,//开始输出  
	.adc_sample_fre=BSP_CONFIG_SAMPLE_FRE,
	.AverageTimes=AVERAGE_TIMES
};


//修改FLASH中的系统配置表
uint8_t ModifySysConfig(uint32_t item,uint32_t argument){
	Bsp_Config_Typedef obj;
	Interal_Flash_ReadByByte(USER_FLASH_ADDR_START,(uint8_t*)&obj,sizeof(Bsp_Config_Typedef));//读出配置表
	switch(item){//修改
		case SYS_CONFIG_CMD_INDEX_UPDATEFLAG:
			obj.update_flag=(uint8_t)argument;
			break;
		case SYS_CONFIG_CMD_INDEX_RANGE:
			obj.range=argument;
			break;
		case SYS_CONFIG_CMD_INDEX_PRINT_STARTSTOP:	//开始停止
			obj.print_start_stop=(uint8_t)argument;
			break;
		case SYS_CONFIG_CMD_INDEX_ADC_SAMPLEFRE:	//采样率
			obj.adc_sample_fre=(uint32_t)argument;
			break;

		default:break;
	}
	
	if(INTERAL_FLASH_SUCCESS==Interal_Flash_WriteByByte(USER_FLASH_ADDR_START,(uint8_t *)&obj,sizeof(Bsp_Config_Typedef))){
			printf("Modify sysconfig arguments done!restart.....\r\n");
			//打印参数表		
			return CONFIG_SUCCESS;
		}else{
			printf("Modify sysconfig arguments error!\r\n");
			return CONFIG_FALSE;
		}
}


//修改当前临时配置
uint8_t ModifyTempConfig(Bsp_Config_Typedef *obj,uint32_t item,uint32_t argument){

	switch(item){//修改
		case SYS_CONFIG_CMD_INDEX_UPDATEFLAG:
			obj->update_flag=(uint8_t)argument;
			break;
		case SYS_CONFIG_CMD_INDEX_RANGE:
			obj->range=argument;
			break;

		case SYS_CONFIG_CMD_INDEX_PRINT_STARTSTOP:
			obj->print_start_stop=(uint8_t)argument;
			break;
		case SYS_CONFIG_CMD_INDEX_ADC_SAMPLEFRE:
			obj->adc_sample_fre=argument;
			break;
		case SYS_CONFIG_CMD_INDEX_AVERAGETIMES:
			obj->AverageTimes=argument;
			break;

		default:break;
	}
	
	
	return CONFIG_SUCCESS;
}


void Bsp_Config_NeedReInit(void){
	Bsp_Config_NeedReinitAll=BSP_CONFIG_NEED_REINIT_ALL;
}
