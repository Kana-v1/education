/*
 * f303xx_spi_driver.h
 *
 *  Created on: May 9, 2023
 *      Author: Kana
 */

#ifndef INC_F303XX_SPI_DRIVER_H_
#define INC_F303XX_SPI_DRIVER_H_

#include "stm32f303xx.h"

typedef struct {
	uint8_t SPI_DeviceMode;			// slave or master
	uint8_t SPI_DeviceBusConfig;
	uint8_t SPI_SclkSpeed;
	uint8_t SPI_DFF;				// format of the data frame (8 or 16 bits)
	uint8_t SPI_CPOL;				// defines whether clock is 0 or 1 in the idle state
	uint8_t SPI_CPHA;				// read data on the leading or trailing edge of the clock signal
	uint8_t SPI_SSM;				// slave select management
} SPI_Config_t;

typedef struct {
	SPI_RegDef_t *pSPIx;
	SPI_Config_t SPIConfig;
} SPI_Handle_t;

#define SPI_DEVICE_MODE_MASTER	1
#define SPI_DEVICE_MODE_SLAVE		0

#define SPI_BUS_CONFIG_FD			1 // full duplex
#define SPI_BUS_CONFIG_HD			2 // half duplex
#define SPI_BUS_CONFIG_S_RXONLY 	3 // receive only

#define SPI_SCLK_SPEED_DIV2	0
#define SPI_SCLK_SPEED_DIV4	1
#define SPI_SCLK_SPEED_DIV8	2

#define SPI_DFF_8BITS		0
#define SPI_DFF_16BITS	1

#define SPI_CPOL_HIGH 1
#define SPI_CPOL_LOW  0

#define SPI_CPHA_HIGH 1
#define SPI_CPHA_LOW  0

#define SPI_SSM_EN 1
#define SPI_SSM_DI 0

#define SPI_TXE_FLAG (1 << SPI_SR_TXE_OFFSET)
#define SPI_RXNE_FLAG (1 << SPI_SR_RXNE_OFFSET)
#define SPI_BUSY_FLAG (1 << SPI_SR_BUSY_OFFSET)

// peripheral clock setup
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnOrDi);

void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_RegDef_t *pSPIx);

// data send and receive
void SPI_SendData(SPI_RegDef_t *pSPIx, uint8_t *dataPtr, uint32_t length);
void SPI_ReceiveData(SPI_RegDef_t *pSPIx, uint8_t *rxBufPtr, uint32_t length);

// IRQ Configuration and ISR handling
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnOrDi);
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t *pHandle);

// peripheral control APIs
void SPI_PeripheralControl(SPI_RegDef_t *pSPI, uint8_t enOrDi);

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx, uint32_t flagName);

void SPI_SSIConfig(SPI_RegDef_t *pSPI, uint8_t enOrDi);
void SPI_SSOEConfig(SPI_RegDef_t *pSPI, uint8_t enOrDi);

#endif /* INC_F303XX_SPI_DRIVER_H_ */
