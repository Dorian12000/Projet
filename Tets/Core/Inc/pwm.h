/*
 * pwm.h
 *
 *  Created on: Oct 5, 2023
 *      Author: gourd
 */

#ifndef INC_PWM_H_
#define INC_PWM_H_

#include <stdint.h>
#include "tim.h"

typedef struct PWM_struct
{
	TIM_HandleTypeDef *timer;
	uint32_t channel;
	float alpha; // 0 - 1
}PWM;

void set_pwm(PWM *pwm, float alpha);

#endif /* INC_PWM_H_ */
