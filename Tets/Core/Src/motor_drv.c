/*
 * motor_drv.c
 *
 *  Created on: Sep 27, 2023
 *      Author: Gaël
 */

#include "motor_drv.h"
#include <stdio.h>

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
		STOP_PWM(*motor->rev);
	}
	else if(motor->state == FWD)
	{
		STOP_PWM(*motor->fwd);
	}
	motor->speed = 0;
	motor->state = STOP;
}

void setSpeedMotor(h_motor_t *motor, uint8_t speed)
{


	motor->speed = speed;
}

void forwardMotor(h_motor_t *motor, uint8_t speed)
{
	// Check speed
	if(speed > MAX_SPEED)
	{
		speed = MAX_SPEED;
	}
	else if(speed < MIN_SPEED)
	{
		speed = MIN_SPEED;
	}

	// Check direction
	if(motor->state == REV)
	{
		STOP_PWM(*motor->rev);
		motor->state = FWD;
		if(START_PWM(*motor->fwd) != HAL_OK)
		{
			printf("Error start fwd pwm\r\n");
			Error_Handler();
		}
	}
	else if(motor->state == STOP)
	{
		motor->state = FWD;
		if(START_PWM(*motor->fwd) != HAL_OK)
		{
			printf("Error start fwd pwm\r\n");
			Error_Handler();
		}
	}

	motor->speed = speed;
	setPwmDutyCycle(motor->fwd, speed);
}

void reverseMotor(h_motor_t *motor, uint8_t speed)
{
	// Check speed
	if(speed > MAX_SPEED)
	{
		speed = MAX_SPEED;
	}
	else if(speed < MIN_SPEED)
	{
		speed = MIN_SPEED;
	}

	// Check direction
	if(motor->state == FWD)
	{
		STOP_PWM(*motor->fwd);
		motor->state = REV;
		if(START_PWM(*motor->rev) != HAL_OK)
		{
			printf("Error start rev pwm\r\n");
			Error_Handler();
		}
	}
	else if(motor->state == STOP)
	{
		motor->state = REV;
		if(START_PWM(*motor->rev) != HAL_OK)
		{
			printf("Error start rev pwm\r\n");
			Error_Handler();
		}
	}

	motor->speed = speed;
	setPwmDutyCycle(motor->rev, speed);
}
