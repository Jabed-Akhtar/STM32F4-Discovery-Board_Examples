/*
 ******************************************************************************
 * @file: main.c
 * @author: Jabed Akhtar
 * @date: 07-December-2020
 *****************************************************************************
 * led: Pin D12 (On-Board)
 * Timer: TIM10
 * USART: USART2 (Baudrate: 9600, Stop Bit: 1, Data: 8 bit)
 * System Clock: 24 MHz
 * Timer (TIM10) interrupt: 1sec
 * Description: Here I am using Timer (TIM10) to send data through USART2 every
 * 		half second. And at every half second LED-D12 is blinking.
 ******************************************************************************
*/

/* Includes */
#include "stm32f4xx.h"
#include <stdio.h>

/* Functions */
void initLED(void);
void initTIM10(void);
void initPinsUSART(void);
void initUSART(void);
void sendUSART(char* chValue);

/* Global Parameters */
volatile uint32_t count = 0;
char couChar[20];

// -------------- main program -------------------
int main(void)
{
	SystemInit(); // Initializing System

	initLED(); // initializing LED
	initTIM10(); // initializing TIM10
	initPinsUSART(); // initializing Pins for USART
	initUSART(); // initializing USART

	__enable_irq(); // enabling interrupts to be sure

	// sending data through USART at the beginning of Programm
	char stri[] = "Programm started!!!\n\r";
	sendUSART(stri);

	/* Infinite loop */
	while (1)
	{
		__WFI; // wait for Interrupt
	}
}

// TIM10 Handler
void TIM1_UP_TIM10_IRQHandler(void){
	count++; // increasing counter every second
	sprintf(couChar, "Timer: %d\n\r", count); // changing interger to char
	sendUSART(couChar); // sending data through USART every second

	// LED blinking every second
	GPIO_ToggleBits(GPIOD, GPIO_Pin_12);

	// clearing TIM10 interrupt pending bits
	TIM_ClearITPendingBit(TIM10, TIM_IT_Update);
}

// Function for sending data through USART
void sendUSART(char* charVal){
	while(*charVal != 0x00){ // sending data till there's nothing at end
		USART_SendData(USART2, *charVal);
		for(int i=0; i<2500;i++); // waiting few milliseconds before sending next value
		//while(USART_GetFlagStatus(USART2, USART_FLAG_TC)==RESET);
		charVal++;
	}
}

// initializing USART2
void initUSART(void){
	// Enabling clock for USART
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	// defining USART structure
	USART_InitTypeDef us;
	us.USART_BaudRate = 9600;
	us.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	us.USART_Parity = USART_Parity_No;
	us.USART_WordLength = USART_WordLength_8b;
	us.USART_StopBits = USART_StopBits_1;
	us.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART2, &us); // initializing USART2
	USART_Cmd(USART2, ENABLE); // enabling USART2
}

// Initializing Pins for USART
void initPinsUSART(void){
	// Initializing clock for USART Pins
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	// defining Pins structure for USART
	GPIO_InitTypeDef usPin;
	usPin.GPIO_Mode = GPIO_Mode_AF;   // actiting alternate function mode
	usPin.GPIO_OType = GPIO_OType_PP;
	usPin.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	usPin.GPIO_PuPd = GPIO_PuPd_NOPULL;
	usPin.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &usPin); // initializing Pins
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5 | GPIO_PinSource5 , GPIO_AF_USART2);
}

// Initializing Timer
void initTIM10(void){
	// Enabling Clock for Timer
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
	// defining TIM10 structure
	TIM_TimeBaseInitTypeDef tim;
	tim.TIM_ClockDivision = TIM_CKD_DIV1;
	tim.TIM_CounterMode = TIM_CounterMode_Up;
	tim.TIM_Period = 999; // -> 1/s
	tim.TIM_Prescaler = 23999; // 80000 -> 1000/s
	TIM_TimeBaseInit(TIM10, &tim);
	TIM_ITConfig(TIM10, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM10, ENABLE); // enabling TIM10
	NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
}

// Initializing LED
void initLED(void){
	// Enabling Clock for Pins for LEDs
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	// defining Pin for LED structure
	GPIO_InitTypeDef led;
	led.GPIO_Mode = GPIO_Mode_OUT;
	led.GPIO_OType = GPIO_OType_PP;
	led.GPIO_Pin = GPIO_Pin_12;
	led.GPIO_PuPd = GPIO_PuPd_NOPULL;
	led.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &led); // initializing Pin for LED
}

// ----------------------- END OF FILE ------------------------
