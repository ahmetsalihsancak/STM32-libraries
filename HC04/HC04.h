/*
 * HC04.h
 *
 *  Created on: Aug 17, 2021
 *      Author: Ahmet Salih Sancak
 */

#ifndef INC_HC04_H_
#define INC_HC04_H_

#include "main.h"
#include "tim.h"
#include "gpio.h"

#define TRIG_PIN		HC04_TRIG_Pin
#define TRIG_PORT		HC04_TRIG_GPIO_Port

#define MAX_DISTANCE	200 /* mm */

void HC04_Read (void);
void Delay_us (uint16_t us);

#endif /* INC_HC04_H_ */
