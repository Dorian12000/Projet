/*
 * motor_drv.h
 *
 *  Created on: Sep 27, 2023
 *      Author: Gaël
 */

#ifndef INC_MOTOR_DRV_H_
#define INC_MOTOR_DRV_H_

#include "pwm.h"
#include "pid.h"
#include <stdint.h>
#include "tim.h"

#define MAX_RPM 300
#define MAX_SPEED 100
#define MIN_SPEED 0

typedef enum h_motor_state
{
	FWD,
	REV,
	STOP
}h_motor_state;

typedef struct h_motor_struct
{
	uint8_t speed; // Speed 0-100
	h_motor_state state;
	PWM *fwd;
	PWM *rev;
	PID pid;
	TIM_HandleTypeDef *htim_enc;
}h_motor_t;

extern h_motor_t motor_left;
extern h_motor_t motor_right;


// Démarer et changer de direction (FWD/REV)
// Arreter le moteur
// Mettre à jour la vitesse de rotation (asservissement)
// Lire le codeur
// Calculer la vitesse à partir du codeur
// Position du robot à partir du codeur

void initMotor(h_motor_t *motor, PWM *fwd, PWM *rev, PID pid, TIM_HandleTypeDef *htim_enc);

void setMotorSpeedFwd(h_motor_t *motor, uint8_t speed);
void stopMotor(h_motor_t *motor);


void angleToSpeed(h_motor_t *motor, float angle);
void distanceToSpeed(h_motor_t *motor, float distance);



#endif /* INC_MOTOR_DRV_H_ */
