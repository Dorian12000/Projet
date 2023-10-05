/*
 * motor_drv.c
 *
 *  Created on: Sep 27, 2023
 *      Author: Gaël
 */

#include "motor_drv.h"

h_motor_t motor_left;
h_motor_t motor_right;

void initMotor(h_motor_t *motor, PWM *fwd, PWM *rev)
{
	motor->speed = 0;
	motor->state = STOP;
	motor->fwd = fwd;
	motor->rev = rev;
}

void stopMotor(h_motor_t *motor)
{
	// Stop la PWM correspondant au sens de rotation
	if(motor->state == REV)
	{
		HAL_TIM_PWM_Stop(motor->rev->timer, motor->rev->channel);
	}
	else if(motor->state == FWD)
	{
		HAL_TIM_PWM_Stop(motor->fwd->timer, motor->fwd->channel);
	}
	motor->speed = 0;
	motor->state = STOP;
}

void setSpeedMotor(h_motor_t *motor, uint8_t speed)
{
	motor->speed = speed;

}

void fwdMotor(h_motor_t *motor)
{
	// Vérifie si le moteur ne tourne pas déjà dans l'autre sens et stop la PWM
	if(motor->state == REV)
	{
		HAL_TIM_PWM_Stop(motor->rev->timer, motor->rev->channel);
	}

	// Génération de la PWM pour aller en avant
	motor->state = FWD;
	HAL_TIM_PWM_Start(motor->fwd->timer, motor->fwd->channel);
}

void revMotor(h_motor_t *motor)
{
	// Vérifie si le moteur ne tourne pas déjà dans l'autre sens et stop la PWM
	if(motor->state == FWD)
	{
		HAL_TIM_PWM_Stop(motor->fwd->timer, motor->fwd->channel);
	}

	// Génération de la PWM pour aller en arrière
	motor->state = REV;
	HAL_TIM_PWM_Start(motor->rev->timer, motor->rev->channel);
}


//uint32_t speedMeasurement(h_motor_t *motor, uint16_t avrg_time_ms, uint16_t period_ms)
//{
//	uint16_t current_time_ms = 0;
//	uint16_t count = 0;
//	uint32_t speed = 0;
//
//	// Récupérer la valeur de l'encoder toutes les x ms sur un temps donné
//	while(current_time_ms <= avrg_time_ms)
//	{
//		count++;
//		speed += readEncoder(motor);
//		current_time_ms += period_ms;
//	}
//
//	// Conversions
//	speed /= RES_ENCODER; // Nombre de tour
//	speed = (speed * 60000) / (count * avrg_time_ms); // Tours/min
//
//	return speed/count;
//}
