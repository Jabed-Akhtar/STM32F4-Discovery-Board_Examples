 /*
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @author         : Jabed-Akhtar (Github)
 * @date           : 18.05.2022
 ******************************************************************************
 * - MCU: STM32F767ZI
 * Note: To send CAN-messages to the CAN-Bus (Network) ot to receive CAN-messages
 * 		from the CAN-Bus, a CAN-Transceiver must be connected between MCU and CAN-Bus
 * System Clock: 48 MHz
 * LEDs:
 * 		Pin PD12 (On-Board) - for Push-Button and CAN-Messages sent after pressing the Button
 * 		Pin PD13 (On-Board) - for Timer and CAN-Messages sent from Timer
 * 		Pin PD14 (On-Board) - for Reception of CAN-Messages
 * Timer: TIM10
 * CAN:
 * 		Baudrate: 125 kBd
 * 		Pins: PD0 and PD1
 * 		Rx-Messages:
 * 			Message 1: 0x200
 * 		Tx-Messages:
 * 			Message 1: 0x100 (Message sent after pressing the Button)
 * 			Message 2: 0x101 (Message sent from Timer)
 * 			Message 3: 0x102 (Message sent when any Message is received)
 * Description:
 * 		-
 ******************************************************************************
 */


/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stdio.h"

/* global variables ----------------------------------------------------------*/
volatile CanRxMsg RxMsg1;
volatile CanTxMsg TxMsg1, TxMsg2, TxMsg3;
volatile uint8_t cou = 0;

// Private functions
void initLED(void);
void initButton(void);
void initTIM10(void);
void initCAN(void);


/* ------------------- Main Function -----------------------------------------*/
int main(void)
{
	// Initializing the System
	SystemInit();

	initLED(); // initializing LEDs
	initButton(); // initializing Push-Button
	initTIM10(); // initializing Timer
	initCAN(); // initializing CAN

	// Infinite loop
	while (1)
	{
        __WFI(); // wait-for-interrupt -> sleep mode
	}
}


/* ------------------- FUNCTIONS ---------------------------------------------*/
// Interrupt Handler for returning Data Field 2nd received message with ID 0x27 (through 0x14C)
void CAN1_RX0_IRQHandler(void){
	if(CAN_GetFlagStatus(CAN1, CAN_FLAG_FMP0)){ // when any message is received
		CAN_Receive(CAN1, CAN_FIFO0, &RxMsg1);
		// when message from ID 0x27 is received
		if(RxMsg1.StdId == 0x200){
			TxMsg3.StdId = 0x14C;
			TxMsg3.RTR = CAN_RTR_Data;
			TxMsg3.IDE = CAN_ID_STD;
			TxMsg3.DLC = 2;
			// Sending the received Messages in reversed form
			TxMsg3.Data[0] = RxMsg1.Data[1];
			TxMsg3.Data[1] = RxMsg1.Data[0];
			CAN_Transmit(CAN1, &TxMsg3);

			GPIO_ToggleBits(GPIOD, GPIO_Pin_14); // Toggling Pin-PD14
		}
	}
	CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
}

/* Interrupt Handler for Timer (TIM10) ---------------------------------------*/
void TIM1_UP_TIM10_IRQHandler(void){
	cou++; // increasing counter

	TxMsg2.StdId = 0x101;
	TxMsg2.DLC = 1;
	TxMsg2.RTR = CAN_RTR_Data;
	TxMsg2.IDE = CAN_ID_STD;
	TxMsg2.Data[1] = cou;
	CAN_Transmit(CAN1, &TxMsg2);

	GPIO_ToggleBits(GPIOD, GPIO_Pin_13); // Toggling Pin-PD13

	TIM_ClearITPendingBit(TIM10, TIM_IT_Update);
}

/* Interrupt Handler for Button ----------------------------------------------*/
void EXTI0_IRQHandler(void){
	if(EXTI_GetFlagStatus(EXTI_Line0) == SET){
		TxMsg1.StdId = 0x100;
		TxMsg1.DLC = 1;
		TxMsg1.RTR = CAN_RTR_Data;
		TxMsg1.IDE = CAN_ID_STD;
		TxMsg1.Data[0] = 0x12;
		CAN_Transmit(CAN1, &TxMsg1);

		GPIO_ToggleBits(GPIOD, GPIO_Pin_12); // Toggling Pin-PD12
	}
	EXTI_ClearITPendingBit(EXTI_Line0);
}

/* Initializing CAN ----------------------------------------------------------*/
void initCAN(void){
	// Configuring Pins for CAN (PD0 and PD1)
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN1);
	GPIO_InitTypeDef pinCAN;
	pinCAN.GPIO_Mode = GPIO_Mode_AF; // Pins must be in AF (Alternative Function)
	pinCAN.GPIO_OType = GPIO_OType_PP;
	pinCAN.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	pinCAN.GPIO_PuPd = GPIO_PuPd_NOPULL; // GPIO_PuPd_DOWN
	pinCAN.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &pinCAN);

	// Enable the CAN controller interface clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	// Set CAN1 register to default values */
    CAN_DeInit(CAN1);
	// Initialize and configure the CAN
	CAN_InitTypeDef can;
	can.CAN_ABOM = DISABLE;
	can.CAN_AWUM = ENABLE;
	can.CAN_BS1 = CAN_BS1_7tq;
	can.CAN_BS2 = CAN_BS2_8tq;
	can.CAN_Mode = CAN_Mode_Normal;
	can.CAN_NART = ENABLE;
	can.CAN_Prescaler = 16;
	can.CAN_RFLM = DISABLE; // ENABLE
	can.CAN_SJW = CAN_SJW_1tq;
	can.CAN_TTCM = DISABLE; // ENABLE
	can.CAN_TXFP = DISABLE; // ENABLE
	CAN_Init(CAN1, &can);
	// CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
	CAN_FilterInitTypeDef canFilt;
	canFilt.CAN_FilterActivation = ENABLE;
	canFilt.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
	canFilt.CAN_FilterIdHigh = 0x0000;
	canFilt.CAN_FilterIdLow = 0x0000;
	canFilt.CAN_FilterMaskIdHigh = 0x0000;
	canFilt.CAN_FilterMaskIdLow = 0x0000;
	canFilt.CAN_FilterMode = CAN_FilterMode_IdMask;
	canFilt.CAN_FilterNumber = 0;
	canFilt.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInit(&canFilt);

	// interrupt controller for CAN
	NVIC_InitTypeDef nvic;
	nvic.NVIC_IRQChannel = CAN1_RX0_IRQn;
	nvic.NVIC_IRQChannelCmd = ENABLE;
	nvic.NVIC_IRQChannelPreemptionPriority = 0;
	nvic.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&nvic);
	//NVIC_EnableIRQ(CAN1_RX0_IRQn);
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

/* Initializing Timer10 ------------------------------------------------------*/
void initTIM10(void){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
	TIM_TimeBaseInitTypeDef tim;
	tim.TIM_ClockDivision = TIM_CKD_DIV4;
	tim.TIM_CounterMode = TIM_CounterMode_Up;
	tim.TIM_Period = 500;
	tim.TIM_Prescaler = 16800;
	// tim.TIM_RepetitionCounter =
	TIM_TimeBaseInit(TIM10, &tim);
	NVIC_InitTypeDef nvic_tim;
	nvic_tim.NVIC_IRQChannel = EXTI0_IRQn;
	nvic_tim.NVIC_IRQChannelCmd = ENABLE;
	nvic_tim.NVIC_IRQChannelPreemptionPriority = 1;
	nvic_tim.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&nvic_tim);
	TIM_ITConfig(TIM10,TIM_IT_CC4, ENABLE);
	TIM_Cmd(TIM10, ENABLE);
}

/* Initializing Pin for Push-Button (On-Board) -------------------------------*/
void initButton(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	GPIO_InitTypeDef pin;
	pin.GPIO_Mode = GPIO_Mode_IN;
	pin.GPIO_OType = GPIO_OType_PP;
	pin.GPIO_Pin = GPIO_Pin_0;
	pin.GPIO_PuPd = GPIO_PuPd_NOPULL;
	pin.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &pin);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	EXTI_InitTypeDef exti;
	exti.EXTI_Line = EXTI_Line0;
	exti.EXTI_LineCmd = ENABLE;
	exti.EXTI_Mode = EXTI_Mode_Interrupt;
	exti.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&exti);
	// interrupt controller for CAN
	NVIC_InitTypeDef nvic_but;
	nvic_but.NVIC_IRQChannel = EXTI0_IRQn;
	nvic_but.NVIC_IRQChannelCmd = ENABLE;
	nvic_but.NVIC_IRQChannelPreemptionPriority = 2;
	nvic_but.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&nvic_but);
}

/* Initializing LEDs ---------------------------------------------------------*/
void initLED(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef led;
	led.GPIO_Mode = GPIO_Mode_OUT;
	led.GPIO_OType = GPIO_OType_PP;
	led.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
	led.GPIO_PuPd = GPIO_PuPd_NOPULL;
	led.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &led);
}


// ================================= END OF FILE =================================
