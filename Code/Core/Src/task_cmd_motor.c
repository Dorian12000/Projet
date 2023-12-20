/*
 * task_motor.c
 *
 *  Created on: Dec 19, 2023
 *      Author: gourd
 */

#include "task_motor.h"
#include "FreeRTOS.h"
#include "main.h"
#include <stdio.h>
#include <limits.h>

typedef struct position_s{
	float angle;
	uint16_t distance;
}position_t;

typedef enum {
	BUMPER_F_NOTIFY,
	BUMPER_B_NOTIFY,
	BUMPER_R_NOTIFY,
	BUMPER_L_NOTIFY,
	BORDER_F_NOTIFY,
	BORDER_B_NOTIFY,
}ISRnotify_t;


#define STACK_SIZE_POSITION_MOTOR 1000
#define TASK_PRIORITY_POSITION_MOTOR 90


command_t command_motor_left;
command_t command_motor_right;

TaskHandle_t position_motor_task_h = NULL;

void positionMotorTaskCreate(void)
{
	if(xTaskCreate(vTaskPositionMotor, "Position Motor Task", STACK_SIZE_POSITION_MOTOR, NULL, TASK_PRIORITY_POSITION_MOTOR, &position_motor_task_h) != pdPASS)
	{
		printf("Error position motor task create\r\n");
		Error_Handler();
	}
}


TaskHandle_t *getPositionMotorTaskHandle(void)
{
	return &position_motor_task_h;
}


void vTaskPositionMotor(void *param)
{
	uint32_t notify_value;


	for(;;)
	{
		if(xTaskNotifyWait(0, ULONG_MAX, &notify_value, portMAX_DELAY) == pdTRUE)
		{
			if(notify_value == BORDER_F_NOTIFY || notify_value == BORDER_B_NOTIFY) // Notification bordure
			{
				// Gauche stop, Droit stop
				command_motor_left.speed  = 0;
				command_motor_left.dir  = STOP;
				command_motor_right.speed = 0;
				command_motor_right.dir = STOP;
			}
			else // Nouvelle notification de position
			{
				// TODO : Choper la structure position_t du lidar
				position_t position_to_go = {19.0, 10};
				float angle = position_to_go.angle;
				uint16_t distance = position_to_go.distance;

				// Gestion du positionnement en angle en priorité
				if(angle != 0 || angle != 180)
				{
					// Met à jour les état et la vitesse de chaque moteur
					angleToCommand(angle);
				}
				// Vers la direction en ligne droite une fois que le robot est orienté correctement
				else if(distance > 0)
				{
					// Met à jour les état et la vitesse de chaque moteur
					distanceToCommand(distance, angle);
				}
			}

		}
	}
}


void angleToCommand(float angle)
{
	if(angle > 0 && angle <= 90)
	{
		// Gauche en avant, Droit stop
		command_motor_left.speed  = 100;
		command_motor_left.dir  = FWD;
		command_motor_right.speed = 0;
		command_motor_right.dir = STOP;
	}
	else if(angle > 90 && angle <= 180)
	{
		// Gauche en arrière, Droit stop
		command_motor_left.speed  = 100;
		command_motor_left.dir  = REV;
		command_motor_right.speed = 0;
		command_motor_right.dir = STOP;
	}
	else if(angle > 180 && angle <= 270)
	{
		// Gauche stop, Droit en arrière
		command_motor_left.speed  = 0;
		command_motor_left.dir  = STOP;
		command_motor_right.speed = 100;
		command_motor_right.dir = REV;
	}
	else
	{
		// Gauche stop, Droit en avant
		command_motor_left.speed  = 0;
		command_motor_left.dir  = STOP;
		command_motor_right.speed = 100;
		command_motor_right.dir = FWD;
	}
}

void distanceToCommand(uint16_t distance, float angle)
{
	if(angle == 180)
	{
		// Gauche en arrière, Droit en arrière
		command_motor_left.speed  = 100;
		command_motor_left.dir  = REV;
		command_motor_right.speed = 100;
		command_motor_right.dir = REV;
	}
	else if (angle == 0)
	{
		// Gauche en avant, Droit en avant
		command_motor_left.speed  = 100;
		command_motor_left.dir  = FWD;
		command_motor_right.speed = 100;
		command_motor_right.dir = FWD;
	}
}

