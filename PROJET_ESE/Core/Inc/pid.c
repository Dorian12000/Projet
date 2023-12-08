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
}
