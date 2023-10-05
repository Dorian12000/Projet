/*
 * pwm.c
 *
 *  Created on: Oct 5, 2023
 *      Author: gourd
 */

#include "pwm.h"

void initPwm(PWM *pwm)
{
	HAL_TIM_PWM_Start(pwm->timer, pwm->channel);
	setPwmDutyCycle(pwm, 0.0f);
}

void stopPwm(PWM *pwm)
{
	HAL_TIM_PWM_Stop(pwm->timer, pwm->channel);
}

void setPwmDutyCycle(PWM *pwm, float duty_cycle)
{
    if (duty_cycle < 0.0f)
    {
        duty_cycle = 0.0f;
    }
    else if (duty_cycle > 1.0f)
    {
        duty_cycle = 1.0f;
    }

    uint16_t period = pwm->timer->Init.Period;
    uint16_t ccr_value = (uint16_t)(duty_cycle * period);

    __HAL_TIM_SET_COMPARE(pwm->timer, pwm->channel, ccr_value);
}
