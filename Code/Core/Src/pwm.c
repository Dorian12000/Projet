/*
 * pwm.c
 *
 *  Created on: Dec 1, 2023
 *      Author: gourd
 */

#include "pwm.h"

/**
 * @brief Configure le rapport cyclique PWM.
 *
 * Cette fonction configure le rapport cyclique PWM en fonction du pourcentage de cycle
 * spécifié et de la valeur maximale autorisée. Le rapport cyclique est ajusté pour
 * se situer dans la plage de 0 à la valeur maximale spécifiée.
 *
 * @param pwm          Pointeur vers la structure représentant le signal PWM.
 * @param duty_cycle   Pourcentage du cycle PWM (0 à 100).
 * @param max_duty_cycle Valeur maximale autorisée du rapport cyclique.
 */
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
