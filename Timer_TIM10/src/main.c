/*
 ******************************************************************************
 * @file: main.c
 * @author: Jabed Akhtar
 * @date: 08-December-2020
 *****************************************************************************
 * System Clock: 24 MHz
 * led: Pin D12, PD13, PD15, PD15 (On-Board)
 * Timer: TIM10
 * Timer (TIM10) interrupt: 1sec
 * Description: Here I am using Timer (TIM10) and I am blinking the LEDs every second
 ******************************************************************************
*/

/* Includes */
#include "stm32f4xx.h"
#include <stdio.h>

/* Functions */
void initLED(void);
void initTIM10(void);

/* Global Parameters */
volatile uint32_t count = 0;

// -------------- main program -------------------
int main(void)
{
	SystemInit(); // Initializing System

	initLED(); // initializing LED
	initTIM10(); // initializing TIM10

	__enable_irq(); // enabling interrupts to be sure


	/* Infinite loop */
	while (1)
	{
		__WFI; // wait for Interrupt
	}
}

// TIM10 Handler
void TIM1_UP_TIM10_IRQHandler(void){
	count++;
	if(count==1){ // at count 1 -> blink LED PD12
		GPIO_SetBits(GPIOD, GPIO_Pin_12);
		GPIO_ResetBits(GPIOD, GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
	}
	else if(count==2){ // at count 1 -> blink LED PD13
		GPIO_SetBits(GPIOD, GPIO_Pin_13);
		GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_14 | GPIO_Pin_15);
	}
	else if(count==3){ // at count 1 -> blink LED PD14
		GPIO_SetBits(GPIOD, GPIO_Pin_14);
		GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15);
	}
	else{ // at count 1 -> blink LED PD15
		GPIO_SetBits(GPIOD, GPIO_Pin_15);
		GPIO_ResetBits(GPIOD, GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14);
		count = 0;
	}

	// clearing TIM10 interrupt pending bits
	TIM_ClearITPendingBit(TIM10, TIM_IT_Update);
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
	led.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	led.GPIO_PuPd = GPIO_PuPd_NOPULL;
	led.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &led); // initializing Pin for LED
}

// ----------------------- END OF FILE ------------------------
