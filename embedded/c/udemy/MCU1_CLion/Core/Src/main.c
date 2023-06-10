/*
 * uart_tx.c
 *
 *  Created on: Jan 22, 2019
 *      Author: admin
 */

#include<stdio.h>
#include<string.h>
#include "stm32f303xx.h"



//we have 3 different messages that we transmit to arduino
//you can by all means add more messages
char *msg[3] = {"hihihihihihi123", "Hello How are you ?" , "Today is Monday !"};

//reply from arduino will be stored here
char rx_buf[1024] ;

USART_Handle_t usart_handle;


//This flag indicates reception completion
uint8_t rxCmplt = RESET;

uint8_t g_data = 0;

extern void initialise_monitor_handles();

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
int main(void)
{
    uint32_t cnt = 0;


    initialise_monitor_handles();

    USART1_GPIOInit();
    USART1_Init();

    USART_IRQInterruptConfig(IRQ_NO_USART1,ENABLE);

    USART_PeripheralControl(USART1,ENABLE);

    printf("Application is running\n");

    button_GPIOInit();

    //do forever
    while(1)
    {
        //wait till button is pressed
        while( ! GPIO_ReadFromInputPin(GPIOA,GPIO_PIN_NO_0) );

        //to avoid button de-bouncing related issues 200ms of delay
        delay();

        // Next message index ; make sure that cnt value doesn't cross 2
        cnt = cnt % 3;

        //First lets enable the reception in interrupt mode
        //this code enables the receive interrupt
        //while ( USART_ReceiveDataIT(&usart_handle,rx_buf,strlen(msg[cnt])) != USART_READY );

        //Send the msg indexed by cnt in blocking mode
        USART_SendData(&usart_handle,(uint8_t*)msg[cnt],strlen(msg[cnt]));

        printf("Transmitted : %s\n",msg[cnt]);


        //Now lets wait until all the bytes are received from the arduino .
        //When all the bytes are received rxCmplt will be SET in application callback
        while(rxCmplt != SET);

        //just make sure that last byte should be null otherwise %s fails while printing
        rx_buf[strlen(msg[cnt])+ 1] = '\0';

        //invalidate the flag
        rxCmplt = RESET;

        //move on to next message indexed in msg[]
        cnt ++;
    }


    return 0;
}


void USART1_IRQHandler(void)
{
    USART_IRQHandling(&usart_handle);
}



void USART_ApplicationEventCallback( USART_Handle_t *pUSARTHandle,uint8_t ApEv)
{
    if(ApEv == USART_EVENT_RX_CMPLT)
    {
        rxCmplt = SET;

    }else if (ApEv == USART_EVENT_TX_CMPLT)
    {
        ;
    }
}