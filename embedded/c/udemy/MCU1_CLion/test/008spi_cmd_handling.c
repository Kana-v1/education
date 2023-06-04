/*
 * 006spi_tx_testing.c
 *
 *  Created on: May 11, 2023
 *      Author: Kana
 */

// PB12 -> SPI2_NSS
// PB13 -> SPI2_SCK
// PB14 -> SPI2_MISO
// PB15 -> SPI2_MOSI
// ALT function mode 5
#include "stm32f303xx.h"
#include <string.h>

// command codes
#define COMMAND_LED_CTRL		0x50
#define COMMAND_SENSOR_READ	0x51
#define COMMAND_LED_READ		0x52
#define COMMAND_PRINT			0x53
#define COMMAND_ID_READ		0x54

#define LED_ON		1
#define LED_OFF	0

#define ANALOG_PIN0		0
#define ANALOG_PIN1		1
#define ANALOG_PIN2		2
#define ANALOG_PIN3		3
#define ANALOG_PIN4		4

#define LED_PIN	9

// this function is used to initialize the GPIO pins to behave as SPI2 pins
void SPI2_GPIOInit(void) {
	GPIO_Handle_t SPIPins;
	SPIPins.pGPIOX = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALT_FN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	// SCLK
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

	// MOSI
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);

	// MISO
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GPIO_Init(&SPIPins);

	// NSS
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Init(&SPIPins);
}

void SPI2_Init(void) {
	SPI_Handle_t SPI2Handle;

	SPI2Handle.pSPIx = SPI2;
	SPI2Handle.SPIConfig.SPI_DeviceBusConfig = SPI_BUS_CONFIG_FD;
	SPI2Handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2Handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV8; // generates sclk of 2Mhz
	SPI2Handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2Handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2Handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2Handle.SPIConfig.SPI_SSM = SPI_SSM_DI; // software slave management enabled for NSS pin

	SPI_Init(&SPI2Handle);
}

void GPIO_ButtonInit() {
	GPIO_Handle_t GPIOBtn;

	GPIOBtn.pGPIOX = GPIOA;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_Init(&GPIOBtn);
}

void delay(void) {
	for (uint32_t i = 0; i < 500000 / 2; i++)
		;
}

uint8_t SPI_VerifyResponse(uint8_t ackByte) {
	return ackByte == 0xF5;
}

uint8_t SPI_SendCommand(uint8_t commandCode, uint8_t length, uint8_t arg1, uint8_t arg2) {
	uint8_t dummyWrite = 0xff;
	uint8_t dummyRead;
	uint8_t ackByte;
	uint8_t args[2];

	SPI_SendData(SPI2, &commandCode, 1);

	// do dummy read to clear off the RXNE
	SPI_ReceiveData(SPI2, &dummyRead, 1);

	// send some dummy bits (1 or 2 bytes according to your communication mode) to fetch the response from the slave
	SPI_SendData(SPI2, &dummyWrite, 1);

	// read the acknowledge byte received from the slave
	SPI_ReceiveData(SPI2, &ackByte, 1);

	// send command
	if (SPI_VerifyResponse(ackByte)) {
		args[0] = arg1;
		args[1] = arg2;
		SPI_SendData(SPI2, args, length);

		return 1;
	}

	return 0;
}

int spiCmdMain(void) {
	uint8_t dummyWrite = 0xff;
	uint8_t ackByte;

	SPI2_GPIOInit();

	SPI2_Init();

	GPIO_ButtonInit();

	/*
	 *	making SSOE 1 does NSS output enable
	 *	the NSS pin is automatically managed by the hardware
	 *	i.e. when SPE=1, NSS will be pulled to low
	 *	and NSS pin will be high when SPE=0
	 */
	SPI_SSOEConfig(SPI2, ENABLE);

	while (1) {
		// wait until button is pressed
		while (!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0))
			;

		// handle button de-bounce trouble
		delay();

		// enable the SPI2 peripheral
		SPI_PeripheralControl(SPI2, ENABLE);

		// 1. CMD_LED_CTRL <pin no(1)> <value(1)>
		SPI_SendCommand(COMMAND_LED_CTRL, LED_PIN, LED_ON, 2);

		// 2. CMD_SENSOR_READ <analog pin number(1)>
		// wait until button is pressed
		while (!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0))
			;

		// handle button de-bounce trouble
		delay();

		SPI_SendCommand(COMMAND_SENSOR_READ, ANALOG_PIN0, 0, 1);

		// read the acknowledge byte received from the slave
		SPI_ReceiveData(SPI2, &ackByte, 1);

		// slave needs some time for the analog-digital conversion
		delay();

		// send some dummy bits (1 or 2 bytes according to your communication mode) to fetch the response from the slave
		SPI_SendData(SPI2, &dummyWrite, 1);
		uint8_t analogRead;
		SPI_ReceiveData(SPI2, &analogRead, 1);

		while (SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG))
			;

		SPI_PeripheralControl(SPI2, DISABLE);
	}

	return 0;
}

