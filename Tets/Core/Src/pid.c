/*
 * pid.c
 *
 *  Created on: Oct 5, 2023
 *      Author: Gaël
 */

#include "pid.h"
#include <stdio.h>

PID pid_motor_left;
PID pid_motor_right;

void initPID(PID *pid, float kp, float ki, float kd, float Te)
{
	float a = Te / 2;

	pid->b0 = kp + ki*a + kd*a;
	pid->b1 = ki*Te + kd*Te;
	pid->b2 = kp + ki*a + kd*a;
	pid->index = 0;
	pid->ordre = 3;
}

void error(PID *pid, float *inputs, uint8_t speed, float speed_mes)
{
	inputs[pid->index] = speed_mes - speed;
	//printf("Erreur = %f\r\n", inputs[pid->index]);
}

void correcteur(PID *pid, float *inputs, float *outputs)
{
	uint8_t ordre = pid->ordre;
	uint8_t idx = pid->index;

	outputs[idx] = outputs[(idx + 2)%ordre]
				   + pid->b0*inputs[idx];
//				   + pid->b1*inputs[(idx + 1)%ordre]
//				   + pid->b2*inputs[(idx + 2)%ordre];

	//printf("Sortie pid = %f\r\n", outputs[idx]);
	pid->index = (idx + 1) % ordre;
}


