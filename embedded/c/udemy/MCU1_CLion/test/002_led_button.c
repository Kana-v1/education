/*
 * 001_led_toggle.c
 *
 *  Created on: Apr 23, 2023
 *      Author: kana
 */


#include "stm32f303xx.h"

#define HIGH 1
#define BTN_PRESSED HIGH

void delay(void) {
	for (uint32_t i = 0; i < 500000 / 2; i++);
}

int ledButtonMain(void) {
	GPIO_Handle_t GpioLED, GPIOBtn;
	GpioLED.pGPIOX = GPIOE;
	GpioLED.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_11;
	GpioLED.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUTPUT;
	GpioLED.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpioLED.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	GpioLED.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriClockControl(GPIOE, ENABLE);
	GPIO_Init(&GpioLED);


	GPIOBtn.pGPIOX = GPIOA;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriClockControl(GPIOA, ENABLE);
	GPIO_Init(&GPIOBtn);

	while(1) {
		if (GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0) == BTN_PRESSED) {
			delay();
			GPIO_ToggleOutputPin(GPIOE, GPIO_PIN_NO_11);
		}

	}

	return 0;
}