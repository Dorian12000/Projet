/*
 * task_asserv.c
 *
 *  Created on: Dec 19, 2023
 *      Author: gourd
 */


#include "task_motor.h"
#include "motor_drv.h"
#include "pid.h"
#include "FreeRTOS.h"
#include <stdio.h>
#include "timers.h"

#define STACK_SIZE_ASSERV_MOTOR 1000
#define TASK_PRIORITY_ASSERV_MOTOR 60

TaskHandle_t asserv_motor_task_h = NULL;
TimerHandle_t timer_asserv_h;

void asservMotorTaskCreate(void)
{
	if(xTaskCreate(vTaskAsservMotor, "Motor Asserv Task", STACK_SIZE_ASSERV_MOTOR, NULL, TASK_PRIORITY_ASSERV_MOTOR, &asserv_motor_task_h) != pdPASS)
	{
		printf("ERROR: Asserv Motor Task Creation\r\n");
		Error_Handler();
	}

	timer_asserv_h = xTimerCreate("Asserv Timer", pdMS_TO_TICKS(TE), pdTRUE, NULL, vTaskAsservMotor);
	if(timer_asserv_h == NULL)
	{
		printf("EROR: Asserv Timer Creation\r\n");
		Error_Handler();
	}

	if(xTimerStart(timer_asserv_h, 0) != pdPASS)
	{
		printf("Error Asserv Timer Start\r\n");
		Error_Handler();
	}
}


TaskHandle_t getAsservMotorTaskHandle(void)
{
	return asserv_motor_task_h;
}


void vTaskAsservMotor(void *param)
{
	uint32_t current_pos_motor_left  = 0;
	uint32_t current_pos_motor_right = 0;
	uint32_t last_pos_motor_left  = READ_MOTOR_ENCODER(motor_left);
	uint32_t last_pos_motor_right = READ_MOTOR_ENCODER(motor_right);

	for(;;)
	{
		// Commande pour les moteur
//		uint8_t speed_cmd_motor_left  = command_motor_left.speed;
//		uint8_t speed_cmd_motor_right = command_motor_right.speed;
//		h_motor_state dir_cmd_motor_left = command_motor_left.dir;
//		h_motor_state dir_cmd_motor_right = command_motor_right.dir;

		uint8_t speed_cmd_motor_left  = 50;
		uint8_t speed_cmd_motor_right = 50;
		h_motor_state dir_cmd_motor_left = REV;
		h_motor_state dir_cmd_motor_right = FWD;

		setMotorSpeed(&motor_left, 50, FWD);
		//setMotorSpeed(&motor_left, (uint8_t)speed_cmd_motor_left, dir_cmd_motor_left);
		setMotorSpeed(&motor_right, (uint8_t)speed_cmd_motor_right, dir_cmd_motor_right);

		// Lire valeur des encodeurs des deux moteurs
//		current_pos_motor_left  = READ_MOTOR_ENCODER(motor_left);
//		current_pos_motor_right = READ_MOTOR_ENCODER(motor_right);
//
//		// Lire vitesse des moteurs
//		float speed_mes_motor_left  = getSpeed(last_pos_motor_left, current_pos_motor_left, TE);
//		float speed_mes_motor_right = getSpeed(last_pos_motor_right, current_pos_motor_right, TE);
//
//		// Erreur vitesse entre commande et mesure pour les deux pid
//		error(&pid_motor_left, speed_cmd_motor_left, speed_mes_motor_left);
//		error(&pid_motor_right, speed_cmd_motor_right, speed_mes_motor_right);
//
//		// Appliquer correcteur pid et récupérer la sortie
//		float new_speed_motor_left  = correcteur(&pid_motor_left);
//		float new_speed_motor_right = correcteur(&pid_motor_left);
//
//		// Envoyer la commande au driver de moteur pour la génération des pwm
//		setMotorSpeed(&motor_left, (uint8_t)new_speed_motor_left, dir_cmd_motor_left);
//		setMotorSpeed(&motor_right, (uint8_t)new_speed_motor_right, dir_cmd_motor_right);
//
//		// Mettre a jour la dernière position des encodeurs
//		last_pos_motor_left  = current_pos_motor_left;
//		last_pos_motor_right = current_pos_motor_right;
	}
}
