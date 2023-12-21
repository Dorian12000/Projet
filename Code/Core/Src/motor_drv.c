/*
 * motor_drv.c
 *
 *  Created on: Dec 1, 2023
 *      Author: gourd
 */

#include "motor_drv.h"

h_motor_t motor_left;
h_motor_t motor_right;

void initMotor(h_motor_t *motor, PWM *fwd, PWM *rev, TIM_HandleTypeDef *htim_enc)
{
	motor->state = STOP;
	motor->fwd = fwd;
	motor->rev = rev;
	motor->htim_enc = htim_enc;
}

void setMotorSpeedFwd(h_motor_t *motor, uint8_t speed)
{
	if(motor->state == REV)
	{
		setPwmDutyCycle(motor->rev, 0, MAX_DUTY_CYCLE_MOTOR);
	}

	motor->state = FWD;
	setPwmDutyCycle(motor->fwd, speed, MAX_DUTY_CYCLE_MOTOR);
}

void setMotorSpeedRev(h_motor_t *motor, uint8_t speed)
{
	if(motor->state == FWD)
	{
		setPwmDutyCycle(motor->fwd, 0, MAX_DUTY_CYCLE_MOTOR);
	}

	motor->state = REV;
	setPwmDutyCycle(motor->rev, speed, MAX_DUTY_CYCLE_MOTOR);
}

void stopMotor(h_motor_t *motor)
{
	if(motor->state == REV)
	{
		setPwmDutyCycle(motor->rev, 0, MAX_DUTY_CYCLE_MOTOR);
	}
	else if(motor->state == FWD)
	{
		setPwmDutyCycle(motor->fwd, 0, MAX_DUTY_CYCLE_MOTOR);
	}
	motor->state = STOP;
}

void setMotorSpeed(h_motor_t *motor, uint8_t speed, h_motor_state dir)
{
	if(dir == REV)
	{
		setMotorSpeedRev(motor, speed);
	}
	else if(dir == FWD)
	{
		setMotorSpeedFwd(motor, speed);
	}
	else if(dir == STOP)
	{
		stopMotor(motor);
	}
}

float getSpeed(uint32_t enc_prev, uint32_t enc_curr, uint16_t time_ms)
{
	float distance = (enc_curr - enc_prev) / RES_ENCODER;
	float speed = (distance / (time_ms / 1000.0)) * 60.0; // RPM

	return speed / MAX_RPM; // Vitesse entre 0 et 100
}
