/*
******************************************************************************
* @file: main.c
* @author: Jabed-Akhtar (Github)
* @date: 01-May-2022
******************************************************************************
* led: Pin D12 (On-Board)
******************************************************************************
*/

/* Includes */
#include "stm32f4xx.h"

/* Functions */
void init_LED(void); // configuring LEDs


/**
**===========================================================================
**  Abstract: main program
**===========================================================================
*/
int main(void)
{
	SystemInit(); // initializing System

	init_LED(); // configuring LEDs

	uint32_t counter = 0;

    /* Infinite loop */
  	while (1)
    {
  		GPIO_ToggleBits(GPIOD, GPIO_Pin_12);
  		for (int i = 0; i < 1680000; i++); // Waiting for 0.5 sec (Delay)
  		GPIO_ToggleBits(GPIOD, GPIO_Pin_13);
  		for (int i = 0; i < 1680000; i++); // Waiting for 0.5 sec (Delay)
  		GPIO_ToggleBits(GPIOD, GPIO_Pin_14);
  		for (int i = 0; i < 1680000; i++); // Waiting for 0.5 sec (Delay)
  		GPIO_ToggleBits(GPIOD, GPIO_Pin_15);
  		for (int i = 0; i < 1680000; i++); // Waiting for 0.5 sec (Delay)
    }
}

// Function for initializing LED ******************************************
void init_LED(void){
	// enabling Clock
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);   // Clock for LED

	// GPIO Init structure definition
	GPIO_InitTypeDef led;
	led.GPIO_Mode = GPIO_Mode_OUT;
	led.GPIO_OType = GPIO_OType_PP;
	led.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	led.GPIO_PuPd = GPIO_PuPd_UP;
	led.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &led);   // Initializing the GPIO
}

// ================================= END OF FILE =================================
