#include "bsp_buildframe.h"
#include <string.h>

BuildFrame_Typedef BuildFrameC;




void BuildFrame_Init(BuildFrame_Typedef *obj){
	obj->inputbussy=BUILDFRAME_BUSSY;
	obj->framelen=BUILD_FRAME_TOTAL_LEN;
	obj->tmpdata.pressurelen=PRESSURE_DATA_LEN;
	obj->tmpdata.pressuretemplen=PRESSURETEMP_DATA_LEN;
	obj->tmpdata.temperaturelen=TEMPRATURE_DATA_LEN;
	
	memset(obj->tmpdata.pressure,0,obj->tmpdata.pressurelen);
	memset(obj->tmpdata.pressuretemp,0,obj->tmpdata.pressuretemplen);
	memset(obj->tmpdata.temperature,0,obj->tmpdata.temperaturelen);
	
	memset(obj->BuildFrameOutputBuf,0,BUILDFRAMEBUFLEN);
	memset(obj->BuildFrameSendBuf,0,BUILDFRAMEBUFLEN);
	
	obj->inputbussy=BUILDFRAME_NOBUSSY;
	memset(obj->BuildFrameOutputShadowBuf,0,BUILDFRAMEBUFLEN);

}




//֡���� ��������ÿһ��

uint8_t BuildFrameSingleInput(BuildFrame_Typedef *obj,uint8_t id,uint8_t *buf,uint8_t len){

		obj->inputbussy=BUILDFRAME_BUSSY;
		//������
		
		return BUILDFRAME_SUCCESS;
}



//֡����  ȫ������
uint8_t BuildFrameAllInput(BuildFrame_Typedef *obj,uint8_t *adc_buf,uint16_t adclen,
																									 uint8_t *pressure_buf,uint16_t pressurelen,
																									 uint8_t *pressuretemp_buf,uint16_t pressuretemplen,
																									 uint8_t *temprature_buf,uint16_t temperaturelen)
{
		uint8_t i;
		uint8_t parity=0x00;
		uint16_t framelen=adclen+pressurelen+pressuretemplen+temperaturelen+3;
		obj->inputbussy=BUILDFRAME_BUSSY;
		//������
		obj->BuildFrameOutputBuf[BUILD_FRAME_A_HEAD_INDEX]=BUILD_FRAME_A_HEAD;//֡ͷ
		memcpy(&obj->BuildFrameOutputBuf[BUILD_FRAME_A_ADC_INDEX],adc_buf,adclen);//5·ADC����ֵ
		memcpy(&obj->BuildFrameOutputBuf[BUILD_FRAME_A_PRESSURE_INDEX],pressure_buf,pressurelen);//MS5540ѹ��ֵ
		memcpy(&obj->BuildFrameOutputBuf[BUILD_FRAME_A_PRE_TEMP_INDEX],pressuretemp_buf,pressuretemplen);//MS5540�¶�ֵ
		memcpy(&obj->BuildFrameOutputBuf[BUILD_FRAME_A_TEMP_INDEX],temprature_buf,temperaturelen);//18B20�¶�ֵ
		//У��
		for(i=0;i<(1+adclen+pressurelen+pressuretemplen+temperaturelen);i++){
			parity^=obj->BuildFrameOutputBuf[i];
		}
		obj->BuildFrameOutputBuf[BUILD_FRAME_A_XOR_INDEX]=parity;
		obj->BuildFrameOutputBuf[BUILD_FRAME_A_TAIL_INDEX]=BUILD_FRAME_A_TAIL;//֡β
		obj->inputbussy=BUILDFRAME_NOBUSSY;//�ͷ��ź���
		
		memcpy(obj->BuildFrameOutputShadowBuf,obj->BuildFrameOutputBuf,framelen);//��֡������ݿ�����Ӱ�ӼĴ���
		return BUILDFRAME_SUCCESS;			
}

//֡���
uint8_t* BuildFrameOutput(BuildFrame_Typedef *obj)
{
		if(obj->inputbussy==BUILDFRAME_NOBUSSY){
			return obj->BuildFrameOutputBuf;
		}else{
			return obj->BuildFrameOutputShadowBuf;
		}
}

