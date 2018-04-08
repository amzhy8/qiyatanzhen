#include "bsp_Ad7606.h"
#include "bsp_uart.h"
#include "string.h"
#include "bsp_config.h"
#include "bsp_time.h"
#include "bsp_circle_fifo.h"
#include "bsp_adder.h"

int16_t Adc_Code[ADC_CHANNEL_CNT];

uint8_t Adc_CodeBytes[ADC_CHANNEL_CNT*2];

uint8_t Adc_SampleOneTimeFlag=0;

void AD7606_SetOS(uint8_t mode)
{
	if(mode == AD7606_MODE_0){
		AD7606_OS3_L();
		AD7606_OS2_L();
		AD7606_OS1_L();
	}
	else if (mode == AD7606_MODE_1)
	{
		AD7606_OS3_L();
		AD7606_OS2_L();
		AD7606_OS1_H();
	}
	else if (mode == AD7606_MODE_2)
	{
		AD7606_OS3_L();
		AD7606_OS2_H();
		AD7606_OS1_L();
	}
	else if (mode == AD7606_MODE_3)
	{

		AD7606_OS3_L();
		AD7606_OS2_H();
		AD7606_OS1_H();
	}
	else if (mode == AD7606_MODE_4)
	{

		AD7606_OS3_H();
		AD7606_OS2_L();
		AD7606_OS1_L();
	}
	else if (mode == AD7606_MODE_5)
	{

		AD7606_OS3_L();
		AD7606_OS2_L();
		AD7606_OS1_H();
	}
	else if (mode == AD7606_MODE_6)
	{

		AD7606_OS3_H();
		AD7606_OS2_H();
		AD7606_OS1_L();
	}
	else	/* 按0处理 */
	{

		AD7606_OS3_L();
		AD7606_OS2_L();
		AD7606_OS1_L();
	}
}

//复位
void AD7606_Reset(void)
{
	AD7606_CS_H();
	AD7606_RESET_L();
	AD7606_RESET_H();
	HAL_Delay(1);
	AD7606_RESET_L();
	HAL_Delay(1);
	AD7606_CONVST_H();
	HAL_Delay(1);
}

void AD7606_StartConv(void)
{
	/* 上升沿开始转换，低电平持续时间至少25ns  */
	AD7606_CONVST_L();	/* 连续执行2次，低电平约50ns */
	AD7606_CONVST_L();	/* 连续执行2次，低电平约50ns */
	AD7606_CONVST_H();
}

uint8_t AD7606_Is_Busy_Low(void){
	if(GPIO_PIN_RESET==HAL_GPIO_ReadPin(AD7606_BUSY_PORT,AD7606_BUSY_PIN)){
		return AD7606_SUCESS;
	}else{
		return AD7606_FAIL;
	}
}

uint8_t AD7606_Spi_ReadByte(uint8_t *data){
	uint8_t res;
	if(HAL_SPI_Receive(&hspi1,data,2, 100)==HAL_OK){
				res=AD7606_SUCESS;
			}else{
				res=AD7606_FAIL;
			}
	return res;
}

uint8_t AD7606_Read8Channel(uint8_t *databuf) 		/* 此函数代码按照时序编写 */
{

	uint8_t res=AD7606_FAIL;
	//uint8_t buf[16]={0};
	/* BUSY = 0 时.ad7606处于空闲状态ad转换结束 */	
	if (AD7606_SUCESS==AD7606_Is_Busy_Low())
    {
		AD7606_CS_L(); /* SPI片选 = 0 */
		//HAL_Delay(1);
		if(HAL_SPI_Receive(&hspi1,databuf,16, 100)==HAL_OK){
			res=AD7606_SUCESS;
		}else{
			res=AD7606_FAIL;
		}
		AD7606_CS_H(); /* SPI片选 = 1 */
		//HAL_Delay(1);
		AD7606_StartConv();	/* 给开始信号 */	
	}else{
		res=AD7606_FAIL;
	}
	return res;
}

//数据处理转化
uint8_t AD7606_ConvertData(int16_t *databuf,float *convertres){
	
return AD7606_SUCESS;
}

void AD7606_Init(uint32_t fre){
		 AD7606_SetOS(0);
		 AD7606_Reset();
		 AD7606_CONVST_H();
		 ADC_Frequency_Modify(fre);
}
/*
uint8_t AD7606_Server_Func(int16_t *value){
	if(AD7606_Read8Channel(value)==AD7606_SUCESS){		
  	SingleBufLoadData(&SingleBuf,Adc_Code);
		return AD7606_SUCESS;
	}else{
		return AD7606_FAIL;
	}
}*/

/*
//定时器中ADC定时采样的回调函数
void ADC_Sample_Callback(void){
		if(AD7606_SUCESS== AD7606_Read8Channel(Adc_Code)){//采样成功  装入BUF		
			SingleBufLoadData(&SingleBuf,Adc_Code);
		}	
}
*/

//定时器中ADC定时采样的回调函数
void ADC_Sample_Callback(void){
		if(Adc_SampleOneTimeFlag==1)return;

		if(AD7606_SUCESS== AD7606_Read8Channel(Adc_CodeBytes)){//采样成功  装入BUF		
			Adc_SampleOneTimeFlag=1;
			//AdcAdder_DataInput(&AdcDataAdder,Adc_CodeBytes,ADC_CHANNEL_CNT);
		}	
}

//改变ADC采样频率
uint8_t ADC_Frequency_Modify(uint32_t fre){
	HAL_NVIC_DisableIRQ(TIM3_IRQn);
	Time3.adc_frequency=fre;
	Time3.adc_fre_threshold=TIME3_FREUENCEY/fre;
	Time3.adc_fre_counter=0;
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
	return AD7606_SUCESS;
}

