/*
 * HC04.c
 *
 *  Created on: Aug 17, 2021
 *      Author: Ahmet Salih Sancak
 */


#include "main.h"
#include "tim.h"
#include "gpio.h"
#include "HC04.h"

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim9;

bool FirstCapture_F = FALSE;
uint32_t val1;
uint32_t val2;
uint32_t diff;
uint8_t distance;

void Delay_us (uint16_t us)
{
	__HAL_TIM_SET_COUNTER(&htim9,0);
	while (__HAL_TIM_GET_COUNTER(&htim9) < us);
}


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)  // ch1
	{
		if (FirstCapture_F == FALSE) //
		{
			val1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1); // ilk capture oku
			FirstCapture_F = TRUE;
			// düşen kenar okuması için ayarla
			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
		}

		else if (FirstCapture_F == TRUE)
		{
			val2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);  // ikinci capture oku
			__HAL_TIM_SET_COUNTER(htim, 0);  //sayaç sıfırla

			if (val2 > val1)
			{
				diff = val2 - val1;
			}

			else if (val1 > val2)
			{
				diff = (0xffff - val1) + val2;
			}

			distance = diff * .034/2;
			if(distance > MAX_DISTANCE)distance = MAX_DISTANCE;

			/* giriş ayarlarına dön */

			FirstCapture_F = FALSE;

			__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
			__HAL_TIM_DISABLE_IT(&htim1, TIM_IT_CC1);
		}
	}
}

void HC04_Read (void)
{
	HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_SET);  // TRIG pin HIGH
	Delay_us(10);  // 10 us bekle
	HAL_GPIO_WritePin(TRIG_PORT, TRIG_PIN, GPIO_PIN_RESET);  // TRIG pin LOW
	__HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC1);
}
