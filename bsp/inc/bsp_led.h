#ifndef _BSP_LED_H__
#define _BSP_LED_H__
#include "gpio.h"
#include "stm32f4xx_hal.h"

#define LED_GPIO_PORT GPIOB
#define LED_GPIO_PIN GPIO_PIN_5

void LED_Control(uint8_t state);
static void LED_Toggle(void);
void LED_Time_Callback(void);
extern uint32_t Led_Cunter;

#endif

