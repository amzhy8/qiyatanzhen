#ifndef _BSP_UARTPROTOCOL_H__
#define _BSP_UARTPROTOCOL_H__

#include "stm32f4xx_hal.h"
#include "bsp_uart.h"
#define FRAME_SUCCESS 0
#define FRAME_FAIL 1






#define UART_RECV_FRAME_HEAD 0xA5

#define UART_RECV_FRAME_WORK_START  0
#define UART_RECV_FRAME_WORK_STOP   1
#define UART_RECV_FRAME_TAIL 0x5A

//协议帧状态
#define UART_RECV_FRAME_HEAD1_STATE 0
#define UART_RECV_FRAME_SAMPLEFRE_STATE  1
#define UART_RECV_FRAME_AVERAGETIMES_STATE  2
#define UART_RECV_FRAME_STARTSTOP_STATE 3
#define UART_RECV_FRAME_TAIL_STATE 4

#define UART_RECV_FRAME_STATE_BUSY 1
#define UART_RECV_FRAME_STATE_IDLE 0

//协议帧结构
typedef struct {
	uint8_t head;
	uint32_t samplefre;
	uint32_t averagetimes;
	uint8_t start_stop;
	uint8_t tail;
}ProtocolFrameStructure_Typedef;


typedef struct{
	uint16_t index;
	uint8_t busy;
	uint8_t stateindex;
}ProtocolFrameState_Typedef;

typedef struct{
	ProtocolFrameStructure_Typedef framebuf;
	ProtocolFrameState_Typedef state;
	uint8_t (*ProtocolFrameInputADataFunc)(Uart_Fifo_Point_Typedef fifo, uint8_t *data);
	//uint8_t inputdata;
}ProtocolFrame_Typedef;
//接收协议帧   循环接收



extern ProtocolFrame_Typedef UartFrame;
extern ProtocolFrame_Typedef UartFrame2;//串口2帧结构检测状态控制器


uint8_t ProtocolFrame_Init(ProtocolFrame_Typedef *frame,uint8_t (*Func)(Uart_Fifo_Point_Typedef fifo,uint8_t *data));
uint8_t ProtocolFrame_Check(ProtocolFrame_Typedef *frame,Uart_Fifo_Point_Typedef fifo,ProtocolFrameStructure_Typedef *resFrame);
uint8_t UartReceiveServer1(void);
uint8_t UartReceiveServer2(void);
uint8_t UartReceiveServer(void);
#endif


