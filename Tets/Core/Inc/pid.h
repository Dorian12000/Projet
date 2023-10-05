/*
 * pid.h
 *
 *  Created on: Oct 5, 2023
 *      Author: Gaël
 */

#ifndef INC_PID_H_
#define INC_PID_H_

#include <stdint.h>

typedef struct PID_struct
{
	float b0; // coef z
	float b1; // coef z-1
	float b2; // coef z-2
	uint8_t index;
	uint8_t ordre;
}PID;

void initPID(PID *pid, float kp, float ki, float kd, float Te);
void error(PID *pid, float *inputs, float speed, float spead_mes);
void correcteur(PID *pid, float *inputs, float *outputs);

#endif /* INC_PID_H_ */
