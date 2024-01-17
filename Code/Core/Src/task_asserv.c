/*
 * task_asserv.c
 *
 *  Created on: Dec 19, 2023
 *      Author: gourd
 */


#include "task_motor.h"
#include "pid.h"
#include <stdio.h>

#define STACK_SIZE_ASSERV_MOTOR 1000
#define TASK_PRIORITY_ASSERV_MOTOR 60

TaskHandle_t asserv_motor_task_h = NULL;
TimerHandle_t timer_asserv_h;

/**
 * @brief Crée la tâche d'asservissement moteur.
 *
 * Cette fonction crée une tâche d'asservissement moteur ainsi qu'un timer associé.
 * La tâche et le timer sont configurés avec les paramètres spécifiés dans les macros correspondantes.
 *
 * @note Assurez-vous que les macros STACK_SIZE_ASSERV_MOTOR, TASK_PRIORITY_ASSERV_MOTOR
 *       et TE sont définies avec les valeurs appropriées avant d'appeler cette fonction.
 *
 * @see vTaskAsservMotor, STACK_SIZE_ASSERV_MOTOR, TASK_PRIORITY_ASSERV_MOTOR, TE
 */
void asservMotorTaskCreate(void)
{
    // Crée la tâche d'asservissement moteur
    if (xTaskCreate(vTaskAsservMotor, "Motor Asserv Task", STACK_SIZE_ASSERV_MOTOR, NULL, TASK_PRIORITY_ASSERV_MOTOR, &asserv_motor_task_h) != pdPASS)
    {
        printf("ERROR: Asserv Motor Task Creation\r\n");
        Error_Handler();
    }

    // Crée le timer d'asservissement
    timer_asserv_h = xTimerCreate("Asserv Timer", pdMS_TO_TICKS(TE), pdTRUE, NULL, vTaskAsservMotor);
    if (timer_asserv_h == NULL)
    {
        printf("ERROR: Asserv Timer Creation\r\n");
        Error_Handler();
    }

    // Démarre le timer d'asservissement
    if (xTimerStart(timer_asserv_h, 0) != pdPASS)
    {
        printf("Error Asserv Timer Start\r\n");
        Error_Handler();
    }
}


/**
 * @brief Obtient la poignée de la tâche d'asservissement moteur.
 *
 * Cette fonction renvoie la poignée (handle) de la tâche d'asservissement moteur,
 * permettant ainsi d'accéder à la tâche depuis d'autres parties du programme.
 *
 * @return La poignée (handle) de la tâche d'asservissement moteur.
 */
TaskHandle_t getAsservMotorTaskHandle(void)
{
    return asserv_motor_task_h;
}


/**
 * @brief Tâche d'asservissement moteur.
 *
 * Cette tâche gère l'asservissement des moteurs en utilisant des contrôleurs PID.
 * Elle effectue la lecture des encodeurs, calcule la vitesse des moteurs, applique
 * le correcteur PID, et envoie les commandes de vitesse aux moteurs.
 *
 * @param param Pointeur vers les paramètres de la tâche (non utilisé ici).
 */
void vTaskAsservMotor(void *param)
{
    uint32_t current_pos_motor_left  = 0;
    uint32_t current_pos_motor_right = 0;
    uint32_t last_pos_motor_left  = READ_MOTOR_ENCODER(motor_left);
    uint32_t last_pos_motor_right = READ_MOTOR_ENCODER(motor_right);

    for (;;)
    {
        // Commande pour les moteurs
        uint8_t speed_cmd_motor_left  = command_motor_left.speed;
        uint8_t speed_cmd_motor_right = command_motor_right.speed;
        h_motor_state dir_cmd_motor_left = command_motor_left.dir;
        h_motor_state dir_cmd_motor_right = command_motor_right.dir;

        // Lire valeur des encodeurs des deux moteurs
        current_pos_motor_left  = READ_MOTOR_ENCODER(motor_left);
        current_pos_motor_right = READ_MOTOR_ENCODER(motor_right);

        // Lire vitesse des moteurs
        uint8_t speed_mes_motor_left  = getSpeed(&motor_left, last_pos_motor_left, current_pos_motor_left, TE);
        uint8_t speed_mes_motor_right = getSpeed(&motor_right, last_pos_motor_right, current_pos_motor_right, TE);

        // Erreur de vitesse entre commande et mesure pour les deux PID
        error(&pid_motor_left, speed_cmd_motor_left, speed_mes_motor_left);
        error(&pid_motor_right, speed_cmd_motor_right, speed_mes_motor_right);

        // Appliquer correcteur PID et récupérer la sortie
        uint8_t new_speed_motor_left  = correcteur(&pid_motor_left);
        uint8_t new_speed_motor_right = correcteur(&pid_motor_right);

        // Envoyer la commande au driver de moteur pour la génération des PWM
        setMotorSpeed(&motor_left, new_speed_motor_left, dir_cmd_motor_left);
        setMotorSpeed(&motor_right, new_speed_motor_right, dir_cmd_motor_right);

        // Mettre à jour la dernière position des encodeurs
        last_pos_motor_left  = current_pos_motor_left;
        last_pos_motor_right = current_pos_motor_right;
    }
}

