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
	
	//temperature[0]为主存储寄存器   temperature[1]为备份寄存器   当正在读取温度值的时候产生了中断  将从备份寄存器中读取数据
	short temperature[2];  
	

}Temperature_Typedef;


extern Temperature_Typedef TemperatureObj;


#define DS18B20_GPIO_PORT GPIOC
#define DS18B20_GPIO_PIN GPIO_PIN_7


//IO方向设置
#define DS18B20_IO_IN()  {DS18B20_GPIO_PORT->MODER&=~(3<<(7*2));DS18B20_GPIO_PORT->MODER|=0<<7*2;}	//PB5输入模式
#define DS18B20_IO_OUT() {DS18B20_GPIO_PORT->MODER&=~(3<<(7*2));DS18B20_GPIO_PORT->MODER|=1<<7*2;} 	//PB5输出模式
 
////IO操作函数											   
//#define	DS18B20_DQ_OUT PBout(12) //数据端口	PB12
//#define	DS18B20_DQ_IN  PBin(12)  //数据端口	PB12 

   	
uint8_t DS18B20_Init(void);			//初始化DS18B20
short DS18B20_Get_Temp(void);	//获取温度
void DS18B20_Start(void);		//开始温度转换
void DS18B20_Write_Byte(uint8_t dat);//写入一个字节
uint8_t DS18B20_Read_Byte(void);		//读出一个字节
uint8_t DS18B20_Read_Bit(void);		//读出一个位
uint8_t DS18B20_Check(void);			//检测是否存在DS18B20
void DS18B20_Rst(void);			//复位DS18B20 



void Ds18b20_OUT(uint8_t value);
uint8_t Ds18b20_IN(void);

void TemperatureServerInit(Temperature_Typedef *obj);

//温度检测服务函数
void TemperatureServer(Temperature_Typedef *obj);


//除以10  是温度整数值   对10求余是小数值

short TemperatureGetData(Temperature_Typedef *obj);


#endif
