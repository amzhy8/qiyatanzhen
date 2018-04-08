#ifndef __DS18B20_H
#define __DS18B20_H
#include "stm32f4xx_hal.h"

#define TMPERATURE_BUSSY  1
#define TMPERATURE_NOBUSSY  0


#define  TMPERATURE_BUF_LEN 50

typedef struct{
	uint8_t buf[TMPERATURE_BUF_LEN];	
	uint32_t buflen;
	uint32_t buf_index;
	//uint32_t to_avrage_cunter;
	uint32_t to_avrage_cunt;
	
	uint8_t bussy;
	
	//temperature[0]Ϊ���洢�Ĵ���   temperature[1]Ϊ���ݼĴ���   �����ڶ�ȡ�¶�ֵ��ʱ��������ж�  ���ӱ��ݼĴ����ж�ȡ����
	short temperature[2];  
	

}Temperature_Typedef;


extern Temperature_Typedef TemperatureObj;


#define DS18B20_GPIO_PORT GPIOC
#define DS18B20_GPIO_PIN GPIO_PIN_7


//IO��������
#define DS18B20_IO_IN()  {DS18B20_GPIO_PORT->MODER&=~(3<<(7*2));DS18B20_GPIO_PORT->MODER|=0<<7*2;}	//PB5����ģʽ
#define DS18B20_IO_OUT() {DS18B20_GPIO_PORT->MODER&=~(3<<(7*2));DS18B20_GPIO_PORT->MODER|=1<<7*2;} 	//PB5���ģʽ
 
////IO��������											   
//#define	DS18B20_DQ_OUT PBout(12) //���ݶ˿�	PB12
//#define	DS18B20_DQ_IN  PBin(12)  //���ݶ˿�	PB12 

   	
uint8_t DS18B20_Init(void);			//��ʼ��DS18B20
short DS18B20_Get_Temp(void);	//��ȡ�¶�
void DS18B20_Start(void);		//��ʼ�¶�ת��
void DS18B20_Write_Byte(uint8_t dat);//д��һ���ֽ�
uint8_t DS18B20_Read_Byte(void);		//����һ���ֽ�
uint8_t DS18B20_Read_Bit(void);		//����һ��λ
uint8_t DS18B20_Check(void);			//����Ƿ����DS18B20
void DS18B20_Rst(void);			//��λDS18B20 



void Ds18b20_OUT(uint8_t value);
uint8_t Ds18b20_IN(void);

void TemperatureServerInit(Temperature_Typedef *obj);

//�¶ȼ�������
void TemperatureServer(Temperature_Typedef *obj);


//����10  ���¶�����ֵ   ��10������С��ֵ

short TemperatureGetData(Temperature_Typedef *obj);


#endif
