/*
 * motor_drv.h
 *
 *  Created on: Sep 27, 2023
 *      Author: Gaël
 */

#ifndef INC_MOTOR_DRV_H_
#define INC_MOTOR_DRV_H_

#include "pwm.h"

#define MAX_RPM 300
#define MAX_SPEED 100
#define MIN_SPEED 0

#define RES_ENCODER 224.4
#define READ_MOTOR_ENCODER(motor_t) (motor_t).htim_enc->Instance->CNT

typedef enum h_motor_state
{
	FWD,
	REV,
	STOP
}h_motor_state;

typedef struct h_motor_struct
{
	h_motor_state state;
	PWM *fwd;
	PWM *rev;
	TIM_HandleTypeDef *htim_enc;
}h_motor_t;

extern h_motor_t motor_left;
extern h_motor_t motor_right;


void initMotor(h_motor_t *motor, PWM *fwd, PWM *rev, TIM_HandleTypeDef *htim_enc);

void setMotorSpeedFwd(h_motor_t *motor, uint8_t speed);
void setMotorSpeedRev(h_motor_t *motor, uint8_t speed);
void setMotorSpeed(h_motor_t *motor, uint8_t speed, h_motor_state dir);
void stopMotor(h_motor_t *motor);
float getSpeed(h_motor_t *motor, uint32_t enc_prev, uint32_t enc_curr, uint16_t time_ms);

#endif /* INC_MOTOR_DRV_H_ */
