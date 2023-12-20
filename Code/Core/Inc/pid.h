/*
 * pid.h
 *
 *  Created on: Oct 5, 2023
 *      Author: Gaël
 */

#ifndef INC_PID_H_
#define INC_PID_H_

#include <stdint.h>

#define PID_ORDRE 3
#define TE 10 // ms

typedef struct PID_struct
{
	float b0; // coef z
	float b1; // coef z-1
	float b2; // coef z-2
	float inputs[PID_ORDRE];
	float outputs[PID_ORDRE];
	uint8_t index;
}PID;

extern PID pid_motor_left;
extern PID pid_motor_right;

void initPID(PID *pid, float kp, float ki, float kd, float Te);
void error(PID *pid, uint8_t speed_cmd, float speed_mes);
float correcteur(PID *pid);

#endif /* INC_PID_H_ */
