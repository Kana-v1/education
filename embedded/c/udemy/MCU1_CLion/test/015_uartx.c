//
// Created by kana on 6/6/23.
//

#include "stm32f303xx.h"

char msg[1024] = "USART Tx testing... \n\r";
USART_Handle_t usart_handle;

void USART1_Init(void) {
    usart_handle.pUSARTx = USART1;
    usart_handle.USART_Config.USART_Baud = USART_STD_BAUD_9600;
    usart_handle.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
    usart_handle.USART_Config.USART_Mode = USART_MODE_ONLY_TX;
    usart_handle.USART_Config.USART_NoOfStopBits = USART_STOPBITS_1;
    usart_handle.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;
    usart_handle.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
    USART_Init(&usart_handle);
}

void USART1_GPIOInit(void) {
    GPIO_Handle_t usart_gpios;

    usart_gpios.pGPIOX = GPIOA;
    usart_gpios.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALT_FN;
    usart_gpios.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    usart_gpios.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
    usart_gpios.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    usart_gpios.GPIO_PinConfig.GPIO_PinAltFunMode = 7;

    usart_gpios.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_9;
    GPIO_Init(&usart_gpios);

    usart_gpios.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_10;
    GPIO_Init(&usart_gpios);
}

void delay(void) {
    for (uint32_t i = 0; i < 500000 / 2; i++);
}

void button_GPIOInit(void) {
    GPIO_Handle_t GPIOBtn;
    GPIOBtn.pGPIOX = GPIOA;
    GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
    GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
    GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
    GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

    GPIO_PeriClockControl(GPIOA, ENABLE);
    GPIO_Init(&GPIOBtn);
}

// PA9 - USART1 tx
// PA10 - USART1 rx
int main(void) {
    USART1_GPIOInit();

    USART1_Init();

    USART_PeripheralControl(USART1, ENABLE);


    button_GPIOInit();

    while (1) {
        while (!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));

        delay();

        USART_SendData(&usart_handle, (uint8_t *) msg, strlen(msg));
    }

    return 0;
}