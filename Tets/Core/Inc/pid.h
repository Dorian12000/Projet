/*
 * pid.h
 *
 *  Created on: Oct 5, 2023
 *      Author: Gaël
 */

#ifndef INC_PID_H_
#define INC_PID_H_

extern float pid_error_input[3]; // mettre tout à 0
extern float pid_output[3]; 	 // mettre tout à 0

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
void correcteur(PID *pid, float *inputs, float *outputs);

#endif /* INC_PID_H_ */
