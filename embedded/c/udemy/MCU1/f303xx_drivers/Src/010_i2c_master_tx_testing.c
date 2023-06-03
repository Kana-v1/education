/*
 * 010_i2c_master_tx_testing.c
 *
 *  Created on: Jun 3, 2023
 *      Author: Kana
 */

#include <string.h>
#include "stm32f303xx.h"

I2C_Handle_t I2C1_Handle;

uint8_t someData[] = "testing string\n";

void delay(void) {
	for (uint32_t i = 0; i < 500000 / 2; i++)
		;
}

// this function is used to initialize the GPIO pins to behave as I2C pins
// PB6 -> SCL
// PB7 -> SDA
void I2C1_GPIOInit(void) {
	GPIO_Handle_t I2CPins;

	I2CPins.pGPIOX = GPIOB;
	I2CPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALT_FN;
	I2CPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	I2CPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	I2CPins.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	I2CPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	// scl
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
	GPIO_Init(&I2CPins);

	// sda
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
	GPIO_Init(&I2CPins);

}

void I2C1_Init(void) {
	I2C1_Handle.pI2Cx = I2C1;
	I2C1_Handle.I2C_Config.I2C_AckControl = I2C_ACK_ENABLE;
	I2C1_Handle.I2C_Config.I2C_DeviceAddress = 0x61; // is not required for the master
	I2C1_Handle.I2C_Config.I2C_FMDutyCycle = I2C_FM_DUTY_2;
	I2C1_Handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SLEED_SM;

	I2C_Init(&I2C1_Handle);
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

int main(void) {
	GPIO_ButtonInit();
	// i2c pin init
	I2C1_GPIOInit();

	// i2c peripheral configuration
	I2C1_Init();

	// wait for button press
	while (1) {
		// wait until button is pressed
		while (!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0))
			;

		// handle button de-bounce trouble
		delay();

		I2C_MasterSendData(&I2C1_Handle, someData, strlen((char*) someData), 0x68);
	}

	while (1)
		;

	return 0;
}
