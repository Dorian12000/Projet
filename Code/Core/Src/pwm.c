/*
 * pwm.c
 *
 *  Created on: Dec 1, 2023
 *      Author: gourd
 */

#include "pwm.h"

void setPwmDutyCycle(PWM *pwm, uint8_t duty_cycle)
{
	uint16_t duty_cycle16 = duty_cycle * MAX_DUTY_CYCLE_MOTOR / 100;

    if (duty_cycle16 < MIN_DUTY_CYCLE_MOTOR)
    {
    	duty_cycle16 = MIN_DUTY_CYCLE_MOTOR;
    }
    else if (duty_cycle16 > MAX_DUTY_CYCLE_MOTOR)
    {
    	duty_cycle16 = MAX_DUTY_CYCLE_MOTOR;
    }

    __HAL_TIM_SET_COMPARE(pwm->timer, pwm->channel, duty_cycle16);
}
