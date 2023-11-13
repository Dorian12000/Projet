/*
 * pwm.c
 *
 *  Created on: Oct 5, 2023
 *      Author: gourd
 */

#include "pwm.h"
#include <stdio.h>

// duty_cycle entre 0 et 100
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

    printf("duty cycle = %d\r\n", duty_cycle16);
    __HAL_TIM_SET_COMPARE(pwm->timer, pwm->channel, duty_cycle16);
}
