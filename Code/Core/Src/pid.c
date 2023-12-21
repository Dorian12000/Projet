/*
 * pid.c
 *
 *  Created on: Dec 1, 2023
 *      Author: gourd
 */


#include "pid.h"

PID pid_motor_left;
PID pid_motor_right;


void initPID(PID *pid, float kp, float ki, float kd, float Te)
{
	float a = Te / 2;

	pid->b0 = kp + ki*a + kd*a;
	pid->b1 = ki*Te + kd*Te;
	pid->b2 = kp + ki*a + kd*a;
	pid->index = 0;

	for(int i = 0; i < PID_ORDRE; i++)
	{
		pid->inputs[i]  = 0.0;
		pid->outputs[i] = 0.0;
	}
}

// Erreur entre la commande et la vitesse mesurée
void error(PID *pid, uint8_t speed_cmd, float speed_mes)
{
	pid->inputs[pid->index] = speed_mes - speed_cmd;
}

// Calcule la nouvelle commande de vitesse à appliquer au moteur : 0-100
float correcteur(PID *pid)
{
	uint8_t idx = pid->index;

	pid->outputs[idx] = pid->outputs[(idx + 2) % PID_ORDRE]
				   + pid->b0 * pid->inputs[idx];
//				   + pid->b1 * pid->inputs[(idx + 1) % PID_ORDRE]
//				   + pid->b2 * pid->inputs[(idx + 2) % PID_ORDRE];

	//printf("Sortie pid = %f\r\n", outputs[idx]);
	pid->index = (idx + 1) % PID_ORDRE;

	return pid->outputs[idx];
}
