/*
 * stm32f303xx_i2c_driver.c
 *
 *  Created on: May 30, 2023
 *      Author: Kana
 */

#ifndef INC_STM32F303XX_I2C_DRIVER_C_
#define INC_STM32F303XX_I2C_DRIVER_C_

#include "stm32f303xx.h"
#include <stdint.h>

typedef struct {
		uint32_t I2C_SCLSpeed;
		uint8_t I2C_DeviceAddress;
		uint8_t I2C_AckControl;
		uint16_t I2C_FMDutyCycle;
} I2C_Config_t;

typedef struct {
		I2C_RegDef_t *pI2Cx;
		I2C_Config_t I2C_Config;
} I2C_Handle_t;

#define I2C_SCL_SLEED_SM	100000	// standard mode = 100kHz
#define I2C_SCL_SPEED_FM  400000	// fast mode = 400kHz

#define I2C_ACK_ENABLE	1
#define I2C_ACK_DISABLE	0

#define I2C_FM_DUTY_2		0
#define I2C_FM_DUTY_16_9	1

#define I2C1_RESET_OFFSET		21
#define I2C2_RESET_OFFSET		22


// peripheral clock setup
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx, uint8_t EnOrDi);

void I2C_Init(I2C_Handle_t *pI2CHandle);
void I2C_DeInit(I2C_RegDef_t *pI2Cx);

// IRQ Configuration and ISR handling
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnOrDi);
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);

// peripheral control APIs
void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx, uint8_t enOrDi);

uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx, uint32_t flagName);

void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t event);



#endif /* INC_STM32F303XX_I2C_DRIVER_C_ */
