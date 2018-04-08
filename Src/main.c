/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_hal.h"
#include "dma.h"
#include "iwdg.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "bsp_led.h"
#include "bsp_uart.h"
#include "bsp_UartProtocl.h"
#include "bsp_Ad7606.h"
#include "bsp_interal_flash.h"
#include <string.h>
#include <stdio.h>
//#include "bsp_double_buf.h"
#include "bsp_calculation.h"
#include "bsp_buildframe.h"
#include "bsp_dmasend.h"
#include "bsp_ms5540b.h"


#include "bsp_adder.h"
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
//static uint8_t press_data_test[2]={0xb1,0xb2};
//static uint8_t presstmp_data_test[2]={0xc1,0xc2};
static uint8_t tmp_data_test[2]={0xd1,0xd2};
static uint8_t tmp_sendbuf[30]={0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */
	static int16_t i=0;
	//uint8_t buf[]="hello\r\n";
	
  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  MX_SPI1_Init();
  MX_TIM3_Init();
  MX_USART2_UART_Init();
  //MX_IWDG_Init();

  /* USER CODE BEGIN 2 */
	
	ProtocolFrame_Init(&UartFrame,Uart_ReadAByteFromRxFifo);//注册协议组包函数的数据输入接口回调函数
	ProtocolFrame_Init(&UartFrame2,Uart_ReadAByteFromRxFifo);//注册协议组包函数的数据输入接口回调函数
	Uart_Receive_Start(&huart1,Uart_Fifo_Point);//开启串口中断，将数据存入环形缓冲器 串口1
	Uart_Receive_Start(&huart2,Uart_Fifo_Point2);//开启串口中断，将数据存入环形缓冲器 串口2
	Interal_Flash_ReadByByte(USER_FLASH_ADDR_START,(uint8_t*)&Bsp_Config,sizeof(Bsp_Config_Typedef));	//读取模块配置表
	if(Bsp_Config.update_flag!=BSP_CONFIG_TABLE_FLAG){//配置出厂配置
		if(INTERAL_FLASH_SUCCESS==Interal_Flash_WriteByByte(USER_FLASH_ADDR_START,(uint8_t *)&Bsp_Config_default,sizeof(Bsp_Config_Typedef))){
			printf("Firt using of this device,Config default arguments done!\r\n");
			Interal_Flash_ReadByByte(USER_FLASH_ADDR_START,(uint8_t*)&Bsp_Config,sizeof(Bsp_Config_Typedef));	//读取模块配置表
			//打印参数表
		}else{
			printf("Firt using of this device,Config default arguments error!\r\n");
		}
	}
	
	Ms5540_Init(&MS5540);//初始化气压传感器
	AdcAdder_Init(&AdcDataAdder,Bsp_Config.AverageTimes);
	DmaSendInit(&DmaSendMoudle);
	BuildFrame_Init(&BuildFrameC);//组帧模块初始化

	CalculationInit(&Calculation);//算法层初始化	
	AD7606_Init(Bsp_Config.adc_sample_fre);//初始化AD7606  默认采样频率10k

	
	//HAL_TIM_Base_Start_IT(&htim3);  //开始定时器
	HAL_TIM_Base_Start_IT(&htim2);  //开始定时器
	
	
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
		
		
		
		
		
		UartReceiveServer();//监测是否有通信指令送达  如果有  解析指令  并完成相应工作
		
		
	
		/*
		if(ADCADDER_SUCCESS== AdcAdder_DataOutput(&AdcDataAdder,Calculation.datainputreg,8)){
			AdcAverage(&Calculation,AdcDataAdder.AdderTimesThreshold);
			Ms5540_ReadTempPressure(&MS5540);//测量气压和温度
			BuildFrameAllInput(&BuildFrameC,(uint8_t*)Calculation.dataoutputbuf,10,MS5540.pressurebuf,2,MS5540.temperaturebuf,2,tmp_data_test,2);
			DmaSend(&DmaSendMoudle,BuildFrameC.BuildFrameOutputBuf,BuildFrameC.framelen);
			Calculation.bussy=CALCULATION_NOBUSSY;
		}
		*/
		
		if(AD7606_SUCESS== AD7606_Read8Channel(Adc_CodeBytes)){//采样成功  装入BUF		
			tmp_sendbuf[0]=0xA5;
		
			tmp_sendbuf[1]=Adc_CodeBytes[1];
			tmp_sendbuf[2]=Adc_CodeBytes[0];

			tmp_sendbuf[3]=Adc_CodeBytes[3];
			tmp_sendbuf[4]=Adc_CodeBytes[2];
			tmp_sendbuf[5]=Adc_CodeBytes[5];
			tmp_sendbuf[6]=Adc_CodeBytes[4];
			tmp_sendbuf[7]=Adc_CodeBytes[7];
			tmp_sendbuf[8]=Adc_CodeBytes[6];
			tmp_sendbuf[9]=Adc_CodeBytes[9];
			tmp_sendbuf[10]=Adc_CodeBytes[8];		
		
			tmp_sendbuf[11]=0;
			tmp_sendbuf[12]=0;
			tmp_sendbuf[13]=0;
			tmp_sendbuf[14]=0;
			tmp_sendbuf[15]=0;
			tmp_sendbuf[16]=0;
		
			tmp_sendbuf[17]=0;
		
			for(i=0;i<17;i++){
				tmp_sendbuf[17]^=tmp_sendbuf[i];
			}
			tmp_sendbuf[18]=0x5A;
			DmaSend(&DmaSendMoudle,tmp_sendbuf,19);
			HAL_Delay(20);

		}	




	
		
		//喂我中华田园犬
		//HAL_IWDG_Refresh(&hiwdg);
		
	}
		
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
