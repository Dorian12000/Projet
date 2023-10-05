/*
 * pid.c
 *
 *  Created on: Oct 5, 2023
 *      Author: Gaël
 */

#include "pid.h"

void initPID(PID *pid, float kp, float ki, float kd, float Te)
{
	float a = Te / 2;

	pid->b0 = kp + ki*a + kd*a;
	pid->b1 = ki*Te + kd*Te;
	pid->b2 = kp + ki*a + kd*a;
	pid->index = 0;
	pid->ordre = 3;
}

void correcteur(PID *pid, float *inputs, float *outputs)
{
	uint8_t ordre = pid->ordre;
	uint8_t idx = pid->index;

	outputs[idx] = outputs[(idx + 2)%ordre]
				   + pid->b0*inputs[idx]
				   + pid->b1*inputs[(idx + 1)%ordre]
				   + pid->b2*inputs[(idx + 2)%ordre];

	pid->index = (idx + 1) % ordre;
}


