#ifndef __BSP_CONFIG_H__
#define __BSP_CONFIG_H__
#include "stm32f4xx_hal.h"
#include "bsp_Ad7606.h"


#define CONFIG_SUCCESS 0
#define CONFIG_FALSE 1

#define CONFIG_DATAPRINT_FRE_MAX 1000
#define CONFIG_DATAPRINT_FRE_MIN 1

#define CONFIG_ADC_RANGE_5 0x05
#define CONFIG_ADC_RANGE_10 0x0a

#define CONFIG_SINGLE_BUF_DEEP  50
#define CONFIG_SINGLE_BUF_MEMBER_LEN  8

#define CONFIG_CALCULATION_AVERAGE_DEEP  CONFIG_SINGLE_BUF_DEEP
#define CONFIG_CALCULATION_AVERAGE_MEMLEN CONFIG_SINGLE_BUF_MEMBER_LEN

#define SINGLEBUF_DEEP 	 CALCULATION_AVERAGE_DEEP

#define BSP_CONFIG_TABLE_FLAG 0x85	//����Ĭ�����ñ��

#define BSP_CONFIG_NEED 0xA5 		//ϵͳ��Ҫ�������ñ�
#define BSP_CONFIG_NONEED 0x5A	//ϵͳ����Ҫ�������ñ�

//������ģʽ
#define BSP_CONFIG_OUTPUT_MODE_ASSIC 0	//��ASSICģʽ���
#define BSP_CONFIG_OUTPUT_MODE_HEX   1	//��ʮ������ģʽ���

//��������λ
#define BSP_CONFIG_UNIT_UV 	0	//�����ѹֵ��λ ΪUV
#define BSP_CONFIG_UNIT_MV 	1
#define BSP_CONFIG_UNIT_V 	2
//������С�����λ��

#define BSP_CONFIG_DECIMALS_0 0 //û��С��λ 
#define BSP_CONFIG_DECIMALS_1	1
#define BSP_CONFIG_DECIMALS_2 2
#define BSP_CONFIG_DECIMALS_3 3
#define BSP_CONFIG_DECIMALS_4 4
#define BSP_CONFIG_DECIMALS_5 5
#define BSP_CONFIG_DECIMALS_6 6

//���֡��
#define BSP_CONFIG_FRAME_FRE_1 						1	//���֡��

#define BSP_CONFIG_CIRCLE_COUTPUT 				0	//ѭ�����
#define BSP_CONFIG_SINGLE_COUTPUT 				1//�������

#define BSP_CONFIG_OUTPUT_START 				0 //�����ʼ
#define BSP_CONFIG_OUTPUT_STOP  				1 //���ֹͣ

#define BSP_CONFIG_SAMPLE_FRE 						10  //Ĭ�ϲ�����10k
#define AVERAGE_TIMES 1

#define BSP_CONFIG_ADC_BUF_MODE_DOUBLE  	0x01 //˫BUF
#define BSP_CONFIG_ADC_BUF_MODE_SINGLE   	0x000 //��BUF

#define BSP_CONFIG_ADC_BUF_DEEP 					10 //BUF���
#define BSP_CONFIG_ADC_BUF_MEMLEN 					8 //BUF���

#define BSP_CONFIG_ADC_CALCULATION_MODE 	0x00//�㷨ѡ��λ

#define BSP_CONFIG_NEED_REINIT_ALL 0
#define BSP_CONFIG_NONEED_REINIT_ALL 1

#define SYS_CONFIG_CMD_INDEX_UPDATEFLAG 1
#define SYS_CONFIG_CMD_INDEX_RANGE 2

#define SYS_CONFIG_CMD_INDEX_PRINT_STARTSTOP 3
#define SYS_CONFIG_CMD_INDEX_ADC_SAMPLEFRE 4
#define SYS_CONFIG_CMD_INDEX_AVERAGETIMES 5





typedef struct {
	//���±�־λ
	uint8_t update_flag;
	//range
	uint8_t range;
	//�������������

	uint8_t print_start_stop;//��ʼ���  

	uint32_t adc_sample_fre;
	uint32_t AverageTimes;

	
	//SingleBufConfig_Typedef SingleBufConfig;
	uint8_t output_task;
}Bsp_Config_Typedef;

extern Bsp_Config_Typedef Bsp_Config;
extern Bsp_Config_Typedef Bsp_Config_default;
extern uint8_t Bsp_Config_NeedReinitAll;

uint8_t ModifyTempConfig(Bsp_Config_Typedef *obj,uint32_t item,uint32_t argument);
uint8_t ModifySysConfig(uint32_t item,uint32_t argument);
void Bsp_Config_NeedReInit(void);




#endif

