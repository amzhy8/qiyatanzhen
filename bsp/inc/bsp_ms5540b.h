#ifndef __BSP_MS5540B_H__
#define __BSP_MS5540B_H__
#include "stm32f4xx_hal.h"


#define MS5540_SUCCESS 0
#define MS5540_FALSE 1

#define MS5540_DIN_PIN GPIO_PIN_15
#define MS5540_DIN_PORT GPIOB

#define MS5540_DOUT_PIN GPIO_PIN_14
#define MS5540_DOUT_PORT GPIOB

#define MS5540_SCK_PIN GPIO_PIN_13
#define MS5540_SCK_PORT GPIOB



#define MS5540_ID_READ_D1 0x000a
#define MS5540_ID_READ_D2 0x0009
#define MS5540_ID_READ_WORD1 0x0015
#define MS5540_ID_READ_WORD2 0x0016
#define MS5540_ID_READ_WORD3 0x0019
#define MS5540_ID_READ_WORD4 0x001A



#define MS5540_CTRL_DIN 1
#define MS5540_CTRL_SCK 2






typedef struct{
	uint16_t W1;
	uint16_t W2;
	uint16_t W3;
	uint16_t W4;
	
	long C1;
	long C2;
	long C3;
	long C4;
	long C5;
	long C6;
	
	uint16_t D1;
	uint16_t D2;
	
	long UT1;
	long dT;
	
	long OFF;
	long SENS;
	long X;
	
	int16_t temperatureT;
	int16_t PreesureT;
	
	int16_t temperature;
	int16_t Preesure;
	
	
	uint8_t temperaturebuf[2];
	uint8_t pressurebuf[2];

}MS5540_Typedef;



extern MS5540_Typedef MS5540;





/*
uint8_t  Ms5540_PinCtrl(uint8_t pin,GPIO_PinState value);
//读DOUT管脚
uint8_t Ms5540_ReadDoutPin(void);
//start-bit
void Ms5540_StartBit(void);
//setup-bit
void Ms5540_SetupBit(uint16_t id);
//stop-bit
void Ms5540_StopBit(void);
//SCK脉冲
void Ms5540_SckPlus(int8_t n);
void Ms5540_Reset(void);

//初始化
void Ms5540_Init(MS5540_Typedef *obj);
//读一个字节
uint8_t Ms5540_ReadByte(void);
//读D1
uint8_t Ms5540_ReadData(uint16_t id,uint16_t *value);
uint8_t Ms5540_ReadTempPressure(MS5540_Typedef *obj);
*/



void Ms5540_Delay(uint32_t nCount);
void setSCLK  (unsigned char state);
void setDIN   (unsigned char state);
unsigned char getDOUT  (void);
unsigned short SerialGet16(void);
void SerialSendLsbFirst(unsigned char pattern, unsigned char nbr_clock);
void reset(void);
unsigned short getW  (unsigned char index);
unsigned short getD1(void);
unsigned short getD2(void);
long multiply(short a, short b);

void Ms5540_GetW2C(MS5540_Typedef *obj);

//测量温度及压力
void Ms5540_ReadTempPressure(MS5540_Typedef *obj);
void Ms5540_Init(MS5540_Typedef *obj);//初始化气压传感器
void Ms5540_SencondCompenssation(MS5540_Typedef *obj,int16_t pressure,int16_t temperature);


#endif

