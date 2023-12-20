/*
 * task_motor.h
 *
 *  Created on: Dec 19, 2023
 *      Author: gourd
 */

#ifndef INC_TASK_MOTOR_H_
#define INC_TASK_MOTOR_H_

#include "cmsis_os.h"
#include "motor_drv.h"

typedef struct command_s
{
	h_motor_state dir; // Direction du moteur (FWD, REV, STOP)
	uint8_t speed; // 0-100
}command_t;

extern command_t command_motor_left;
extern command_t command_motor_right;

// Pour la tâche qui gère la commande pour le pid en fonction de la position
void positionMotorTaskCreate(void);
TaskHandle_t *getPositionMotorTaskHandle(void);
void vTaskPositionMotor(void *param);

void angleToCommand(float angle);
void distanceToCommand(uint16_t distance, float angle);

// Pour la tâche d'asservissement
void asservMotorTaskCreate(void);
TaskHandle_t *getAsservMotorTaskHandle(void);
void vTaskAsservMotor(void *param);


#endif /* INC_TASK_MOTOR_H_ */
