/*
 * iic_slave.c
 *
 *  Created on: Dec 20, 2024
 *      Author: jabed
 */


/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "iic_slave.h"
#include <stdio.h>

/* Defines -------------------------------------------------------------------*/
#define IICRxLength 6

/* Variables -----------------------------------------------------------------*/
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart2;

int counter = 0;
uint8_t RxData[IICRxLength];

/* Functions -----------------------------------------------------------------*/
/*
 * Callback function for HAL_I2C_EnableListen_IT
 */
void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
	HAL_I2C_EnableListen_IT(&hi2c1);	// Put IIC back to listen mode
}

/*
 * Callback function for IIC-Slave-Address-matched
 */
void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
	if (TransferDirection == I2C_DIRECTION_TRANSMIT)
	{
		HAL_I2C_Slave_Seq_Receive_IT(&hi2c1, RxData, IICRxLength, I2C_FIRST_AND_LAST_FRAME);

		if (RxData[1]==0x0 && RxData[2]==0x0 && RxData[3]==0x0  && RxData[4]==0x0 && RxData[5]==0x0)
		{
			int val_tmp = RxData[0];
			char numberString[30];
			sprintf(numberString, "Counter msg via IIC: %d\r\n", val_tmp);
			HAL_GPIO_TogglePin(GPIOD, LED_Green_Pin);	// Toggle Green LED
			HAL_GPIO_TogglePin(GPIOD, LED_Orange_Pin);	// Toggle Green LED
			HAL_UART_Transmit(&huart2, numberString, sizeof(numberString), HAL_MAX_DELAY);
		}
		else
		{
			char hexString[80];
			sprintf(hexString, "Msg on Push-Button via IIC: 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x\r\n", RxData[0], RxData[1], RxData[2], RxData[3], RxData[4], RxData[5]);
			HAL_GPIO_TogglePin(GPIOD, LED_Blue_Pin);	// Toggle Blue LED
			HAL_GPIO_TogglePin(GPIOD, LED_Red_Pin);	// Toggle Green LED
			HAL_UART_Transmit(&huart2, hexString, sizeof(hexString), HAL_MAX_DELAY);
		}
	}
	else
	{
		Error_Handler();
	}
}

/*
 * Callback function IIC-Slave-Rx-Transfer completion.
 */
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	counter++;
}

/*
 * Callback function for IIC Error
 */
void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
	HAL_I2C_EnableListen_IT(&hi2c1);
}

// ================================= END OF FILE =================================
