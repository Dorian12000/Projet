/*
 * motor_drv.h
 *
 *  Created on: Sep 27, 2023
 *      Author: Gaël
 */

#ifndef INC_MOTOR_DRV_H_
#define INC_MOTOR_DRV_H_

#include <pwm.h>
#include <stdint.h>
#include "tim.h"

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
}h_motor_t;

extern h_motor_t motor_left;
extern h_motor_t motor_right;


// Démarer et changer de direction (FWD/REV)
// Arreter le moteur
// Mettre à jour la vitesse de rotation (asservissement)
// Lire le codeur
// Calculer la vitesse à partir du codeur
// Position du robot à partir du codeur

void initMotor(h_motor_t *motor, PWM *fwd, PWM *rev);

void stopMotor(h_motor_t *motor);
void setSpeedMotor(h_motor_t *motor, uint8_t speed);
void fwdMotor(h_motor_t *motor);
void revMotor(h_motor_t *motor);

uint32_t speedMeasurement(h_motor_t *motor, uint16_t avrg_time_ms, uint16_t period_ms);


#endif /* INC_MOTOR_DRV_H_ */
