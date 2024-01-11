/*
 * motor_drv.c
 *
 *  Created on: Dec 1, 2023
 *      Author: gourd
 */

#include "motor_drv.h"
#include <stdlib.h>

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

uint8_t getSpeed(uint32_t enc_prev, uint32_t enc_curr, uint16_t time_ms)
{
	// 5000 ms
	uint16_t time_s = time_ms/1000;
	// 5 s
	uint32_t distance = abs(enc_curr - enc_prev);
	distance /= 4;
	distance = distance * 60 / time_s;
	uint32_t speed = distance / (int)RES_ENCODER; // RPM
	printf("distance = %d,speed = %d\r\n",distance,speed);
	return 100 * speed / MAX_RPM; // Vitesse entre 0 et 100
}
