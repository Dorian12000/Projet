/*
 * pwm.c
 *
 *  Created on: Dec 1, 2023
 *      Author: gourd
 */

#include "pwm.h"

// Change le
void setPwmDutyCycle(PWM *pwm, uint8_t duty_cycle, uint16_t max_duty_cycle)
{
	uint16_t duty_cycle16 = duty_cycle * MAX_DUTY_CYCLE_MOTOR / 100;

    if (duty_cycle16 < 0)
    {
    	duty_cycle16 = 0;
    }
    else if (duty_cycle16 > max_duty_cycle)
    {
    	duty_cycle16 = max_duty_cycle;
    }

    __HAL_TIM_SET_COMPARE(pwm->timer, pwm->channel, duty_cycle16);
}
