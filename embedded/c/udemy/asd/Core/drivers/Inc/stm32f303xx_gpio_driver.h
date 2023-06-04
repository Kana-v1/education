/*
 * stm32f303xx_gpio_driver.h
 *
 *  Created on: Apr 16, 2023
 *      Author: kana
 */

#ifndef INC_STM32F303XX_GPIO_DRIVER_H_
#define INC_STM32F303XX_GPIO_DRIVER_H_


#include "stm32f303xx.h"

typedef struct {
	uint8_t GPIO_PinNumber;			// possible values from @GPIO_PIN_NUMBER
	uint8_t GPIO_PinMode;			// possible values from @GPIO_PIN_MODES
	uint8_t GPIO_PinSpeed;			// possible values from @GPIO_PIN_SPEED
	uint8_t GPIO_PinPuPdControl;	// possible values from @GPIO_PIN_PULL_DOWN_UP
	uint8_t GPIO_PinOPType;			// possible values from @GPIO_PIN_PULL_DOWN
	uint8_t GPIO_PinAltFunMode;		// possible values from @
}GPIO_PinConfig_t;

typedef struct {
	GPIO_RegDef_t *pGPIOX; // this holds the base address of the GPIO port to which the pin belongs
	GPIO_PinConfig_t GPIO_PinConfig; // this golds GPIO pin configuration settings
}GPIO_Handle_t;

// @GPIO_PIN_MODES
#define GPIO_MODE_IN	   0
#define GPIO_MODE_OUTPUT 1
#define GPIO_MODE_ALT_FN 2
#define GPIO_MODE_ANALOG 3
#define GPIO_MODE_IT_FT  4 // falling edge trigger
#define GPIO_MODE_IT_RT  5 // rising edge trigger
#define GPIO_MODE_IT_RFT 6 // rising edge, falling edge trigger

// @GPIO_PIN_PULL_DOWN
#define GPIO_OP_TYPE_PP	 0	// push-pull
#define GPIO_OP_TYPE_OD	 1	// open drain

// @GPIO_PIN_SPEED
#define GPIO_SPEED_LOW    0
#define GPIO_SPEED_MEDIUM 1
#define GPIO_SPEED_FAST   2
#define GPIO_SPEED_HIGH   3

// @GPIO_PIN_PULL_DOWN_UP
#define GPIO_NO_PUPD 0 // no pull-up, pull-down
#define GPIO_PIN_PU  1 // pull up
#define GPIO_PIN_PD  2 // pull-down

// @GPIO_PIN_NUMBER
#define GPIO_PIN_NO_0  0
#define GPIO_PIN_NO_1  1
#define GPIO_PIN_NO_2  2
#define GPIO_PIN_NO_3  3
#define GPIO_PIN_NO_4  4
#define GPIO_PIN_NO_5  5
#define GPIO_PIN_NO_6  6
#define GPIO_PIN_NO_7  7
#define GPIO_PIN_NO_8  8
#define GPIO_PIN_NO_9  9
#define GPIO_PIN_NO_10 10
#define GPIO_PIN_NO_11 11
#define GPIO_PIN_NO_12 12
#define GPIO_PIN_NO_13 13
#define GPIO_PIN_NO_14 14
#define GPIO_PIN_NO_15 15


void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);

void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnOrDi);

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);
uint32_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value);
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value);
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber);

void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnOrDi);
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void GPIO_IRQHandling(uint8_t PinNumber);

#endif /* INC_STM32F303XX_GPIO_DRIVER_H_ */
