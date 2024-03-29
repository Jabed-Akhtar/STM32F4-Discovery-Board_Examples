/*
******************************************************************************
* @file: main.c
* @author: Jabed Akhtar
* @date: 19-June-2021
******************************************************************************
* led: Pin D12 (On-Board)
* push Button: Pin A0 (On-Board)
* Description: When push Button is pressed, random value should be given by RNG.
******************************************************************************
*/

/* Includes */
#include "stm32f4xx.h"
#include "stdio.h"

/* Functions */
void init_LED(void); // configuring LEDs
void init_Button(void); // configuring external Interrupt (Button)
void init_RNG(void); // initializing Random Number Generator

/* Global Variable */
volatile uint8_t val_rng = 0;

/**
**===========================================================================
**  Abstract: main program
**===========================================================================
*/
int main(void)
{
	SystemInit(); // initializing System

	init_LED(); // configuring LEDs
	init_Button(); // configuring Button
	init_RNG(); // initializing RNG

    /* Infinite loop */
  	while (1)
    {
  		__WFI(); // sending CPU to sleep -> Wait For Interrupt
    }
}

// Function for Button Interrupt Handler ********************************
void EXTI0_IRQHandler(void){
    if(EXTI_GetFlagStatus(EXTI_Line0)== SET){ // when Button is pressed!
    	// Random Number Generator for generating Values
    	if(RNG_GetFlagStatus(RNG_FLAG_DRDY == SET)){
    		GPIO_ToggleBits(GPIOD, GPIO_Pin_12); // toggling the LED
    		val_rng = RNG_GetRandomNumber()%9;
    	}
	}
    // clearing the EXTI's line pending bits
    EXTI_ClearITPendingBit(EXTI_Line0);
}

// configuring RNG ****************************************************************
void init_RNG(void){
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG,ENABLE);
	RNG_Cmd(ENABLE);
}

// configuring Button ***************************************************
void init_Button(void){
	// enabling Clocks
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); // activate clock for GPIOA
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); // activate clock for EXTI
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0); // connect EXTI-Line

    // GPIO Init structure definition
    GPIO_InitTypeDef button;
    button.GPIO_Mode = GPIO_Mode_IN;
    button.GPIO_OType = GPIO_OType_PP;
    button.GPIO_Pin = GPIO_Pin_0;
    button.GPIO_PuPd = GPIO_PuPd_NOPULL;
    button.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA, &button);

    // EXTI Init Structure definition - onBoard Button
    EXTI_InitTypeDef iButton;
    iButton.EXTI_Line = EXTI_Line0;
    iButton.EXTI_LineCmd = ENABLE;
    iButton.EXTI_Mode = EXTI_Mode_Interrupt;
    iButton.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_Init(&iButton);

    // Enable EXTI0_IRQn Interrupt
    NVIC_EnableIRQ(EXTI0_IRQn); // register interrupt with NVIC default settings
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
