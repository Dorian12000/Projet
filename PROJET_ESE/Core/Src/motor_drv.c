/*
 * motor_drv.c
 *
 *  Created on: Dec 1, 2023
 *      Author: gourd
 */


#include "motor_drv.h"

h_motor_t motor_left;
h_motor_t motor_right;

void initMotor(h_motor_t *motor, PWM *fwd, PWM *rev, PID pid, TIM_HandleTypeDef *htim_enc)
{
	motor->speed = 0;
	motor->state = STOP;
	motor->fwd = fwd;
	motor->rev = rev;
	motor->pid = pid;
	motor->htim_enc = htim_enc;
}

void setMotorSpeedFwd(h_motor_t *motor, uint8_t speed)
{
	if(motor->state == REV)
	{
		setPwmDutyCycle(motor->rev, 0.0);
	}

	motor->state = FWD;
	setPwmDutyCycle(motor->fwd, speed);
}

void stopMotor(h_motor_t *motor)
{
	if(motor->state == REV)
	{
		setPwmDutyCycle(motor->rev, 0.0);
	}
	else if(motor->state == FWD)
	{
		setPwmDutyCycle(motor->fwd, 0.0);
	}
	motor->state = STOP;
}
