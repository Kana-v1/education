/*
 * 001_led_toggle.c
 *
 *  Created on: Apr 23, 2023
 *      Author: kana
 */


#include "stm32f303xx.h"


void delay(void) {
	for (uint32_t i = 0; i < 500000; i++);
}

int main(void) {
	GPIO_Handle_t GpioLED;
	GpioLED.pGPIOX = GPIOE;
	GpioLED.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_11;
	GpioLED.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUTPUT;
	GpioLED.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpioLED.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	GpioLED.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriClockControl(GPIOE, ENABLE);
	GPIO_Init(&GpioLED);

	while(1) {
		GPIO_ToggleOutputPin(GPIOE, GPIO_PIN_NO_11);
		delay();

	}

	return 0;
}