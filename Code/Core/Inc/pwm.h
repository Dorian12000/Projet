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

#define MAX_DUTY_CYCLE_MOTOR 2560
#define MIN_DUTY_CYCLE_MOTOR 0

typedef struct PWM_struct
{
	TIM_HandleTypeDef *timer;
	uint32_t channel;
}PWM;

#define START_PWM(pwm) HAL_TIM_PWM_Start((pwm).timer, (pwm).channel)
#define STOP_PWM(pwm)  HAL_TIM_PWM_Stop((pwm).timer, (pwm).channel)
void setPwmDutyCycle(PWM *pwm, uint8_t duty_cycle);

#endif /* INC_PWM_H_ */
