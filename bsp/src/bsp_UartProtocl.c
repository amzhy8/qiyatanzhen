#include "bsp_UartProtocl.h"
#include <string.h>
#include "bsp_dataprint.h"
#include "bsp_time.h"
#include "bsp_interal_flash.h"

//����Э��֡   ѭ������
ProtocolFrame_Typedef UartFrame;//����1֡�ṹ���״̬������
ProtocolFrame_Typedef UartFrame2;//����2֡�ṹ���״̬������

//����Э�������غ�����ǰ  ��Ҫ��ʼ��  ע�ᴮ��ģ����Э�����ģ��֮��ԽӵĻص�����
uint8_t ProtocolFrame_Init(ProtocolFrame_Typedef *frame,uint8_t (*Func)(Uart_Fifo_Point_Typedef fifo,uint8_t *data)){
	memset((uint8_t *)&frame->framebuf,0,sizeof(ProtocolFrameStructure_Typedef));//��ʼ��֡������
	frame->ProtocolFrameInputADataFunc=Func;//ע��ص�����
	frame->state.stateindex=UART_RECV_FRAME_HEAD1_STATE;
	frame->state.busy=UART_RECV_FRAME_STATE_IDLE;
	return 0;
}
/*

#define UART_RECV_FRAME_FRE_STATE  1
#define UART_RECV_FRAME_AVERAGETIMES_STATE  2
#define UART_RECV_FRAME_STARTSTOP_STATE 3
#define UART_RECV_FRAME_TAIL_STATE 4
*/
uint8_t ProtocolFrame_Check(ProtocolFrame_Typedef *frame,Uart_Fifo_Point_Typedef fifo,ProtocolFrameStructure_Typedef *resFrame){
		uint8_t data;
		if(FRAME_FAIL==frame->ProtocolFrameInputADataFunc(fifo,&data)){//�ж��Ƿ�����Ч��������
			return FRAME_FAIL;
		}
		switch(frame->state.stateindex){
			case UART_RECV_FRAME_HEAD1_STATE:

					if(data==UART_RECV_FRAME_HEAD){//�ж�֡ͷ
						frame->framebuf.head=data;//����֡����
						frame->state.index=0;
						frame->framebuf.samplefre=0;
						
						frame->state.stateindex=UART_RECV_FRAME_SAMPLEFRE_STATE;
						break;
					}
				break;
			case UART_RECV_FRAME_SAMPLEFRE_STATE:
					
					frame->framebuf.samplefre|=data;
					frame->state.index++;
					if(frame->state.index!=4){
						frame->framebuf.samplefre=frame->framebuf.samplefre<<8;
					}else{
						frame->state.index=0;
						frame->framebuf.averagetimes=0;
						frame->state.stateindex=UART_RECV_FRAME_AVERAGETIMES_STATE;
					}
				break;
			case UART_RECV_FRAME_AVERAGETIMES_STATE:
					frame->framebuf.averagetimes|=data;
					frame->state.index++;
					if(frame->state.index!=4){
						frame->framebuf.averagetimes=frame->framebuf.averagetimes<<8;
					}else{
						frame->state.stateindex=UART_RECV_FRAME_STARTSTOP_STATE;
					}
				break;
			case UART_RECV_FRAME_STARTSTOP_STATE:
					frame->framebuf.start_stop=(uint8_t)data;//����֡����
					frame->state.stateindex=UART_RECV_FRAME_TAIL_STATE;
				break;

			case UART_RECV_FRAME_TAIL_STATE:
				if(data==UART_RECV_FRAME_TAIL){
					frame->framebuf.tail=(uint8_t)data;//����֡����
					frame->state.stateindex=UART_RECV_FRAME_HEAD1_STATE;
					memcpy(resFrame,&frame->framebuf,sizeof(ProtocolFrameStructure_Typedef));
					return FRAME_SUCCESS;
				}else{
					frame->state.stateindex=UART_RECV_FRAME_HEAD1_STATE;
				}
				break;
			default:break;
		}
		return FRAME_FAIL;
	}

	
	
	
uint8_t UartReceiveServer1(void){
	ProtocolFrameStructure_Typedef usart_frame;


	if(FRAME_SUCCESS==ProtocolFrame_Check(&UartFrame,Uart_Fifo_Point,&usart_frame)){//�յ�һ֡����  ����
		
		

	return FRAME_SUCCESS;
	}
	return FRAME_FAIL;
}
	

	
uint8_t UartReceiveServer2(void){
	ProtocolFrameStructure_Typedef usart_frame;

	if(FRAME_SUCCESS==ProtocolFrame_Check(&UartFrame2,Uart_Fifo_Point2,&usart_frame)){//�յ�һ֡����  ����
		//���Ƿ�����
		
		if(usart_frame.samplefre>300000){
			printf("Ƶ�ʹ���\r\n");
			return FRAME_FAIL;
		}
		//���Ƿ�����
		if(usart_frame.averagetimes>usart_frame.samplefre){
			printf("ƽ����������\r\n");
			return FRAME_FAIL;
		}

		
		if(usart_frame.start_stop==0xff){
			ModifyTempConfig(&Bsp_Config,SYS_CONFIG_CMD_INDEX_ADC_SAMPLEFRE,usart_frame.samplefre);//�������Ƶ��
			ModifyTempConfig(&Bsp_Config,SYS_CONFIG_CMD_INDEX_AVERAGETIMES,usart_frame.averagetimes);//�������Ƶ��
			//д��ע���  
			if(INTERAL_FLASH_SUCCESS==Interal_Flash_WriteByByte(USER_FLASH_ADDR_START,(uint8_t *)&Bsp_Config,sizeof(Bsp_Config_Typedef))){
				printf("Firt using of this device,Config default arguments done!\r\n");
			//��ӡ������
			}else{
				printf("Firt using of this device,Config default arguments error!\r\n");
			}
					//��λ
		//	NVIC_SystemReset();
		}else if(usart_frame.start_stop==UART_RECV_FRAME_WORK_STOP){
			//ֹͣ����
			HAL_TIM_Base_Stop_IT(&htim3); 
		}else if(usart_frame.start_stop==UART_RECV_FRAME_WORK_START){
			HAL_TIM_Base_Start_IT(&htim3); 
		}


		
	}

	return FRAME_SUCCESS;
}
	

	
uint8_t UartReceiveServer(void){
	uint8_t res;
	res|=UartReceiveServer1();
	res|=UartReceiveServer2();
	return res;
}
	


