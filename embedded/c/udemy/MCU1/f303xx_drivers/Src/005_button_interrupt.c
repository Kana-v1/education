/*
 * 001_led_toggle.c
 *
 *  Created on: Apr 23, 2023
 *      Author: kana
 */


#include "stm32f303xx_gpio_driver.h"

#define HIGH 1
#define BTN_PRESSED HIGH

void delay(void) {
	for (uint32_t i = 0; i < 500000 / 2; i++);
}

int main(void) {
	GPIO_Handle_t GpioLED, GPIOBtn;
	GpioLED.pGPIOX = GPIOE;
	GpioLED.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_11;
	GpioLED.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUTPUT;
	GpioLED.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpioLED.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioLED.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriClockControl(GPIOE, ENABLE);
	GPIO_Init(&GpioLED);

	GPIOBtn.pGPIOX = GPIOA;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IT_FT;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_PeriClockControl(GPIOA, ENABLE);
	GPIO_Init(&GPIOBtn);

	// IRQ configurations
	GPIO_IRQPriorityConfig(IRQ_NO_EXTI0, NVIC_IRQ_PRIO12);
	GPIO_IRQInterruptConfig(IRQ_NO_EXTI0, ENABLE);


	while(1);
}

void EXTI0_IRQHandler(void) {
	GPIO_IRQHandling(GPIO_PIN_NO_0);
	GPIO_ToggleOutputPin(GPIOE, GPIO_PIN_NO_11);
}
