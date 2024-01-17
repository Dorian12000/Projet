/*
 * task_motor.c
 *
 *  Created on: Dec 19, 2023
 *      Author: gourd
 */

#include "task_motor.h"
#include "mainTask.h"
#include "drv_lidar.h"
#include "FreeRTOS.h"
#include "main.h"
#include <stdio.h>
#include <limits.h>

#define STACK_SIZE_POSITION_MOTOR 1000
#define TASK_PRIORITY_POSITION_MOTOR 90


command_t command_motor_left;
command_t command_motor_right;

TaskHandle_t position_motor_task_h = NULL;

/**
 * @brief Crée la tâche de positionnement moteur.
 *
 * Cette fonction crée une tâche de positionnement moteur avec les paramètres spécifiés
 * dans les macros correspondantes.
 *
 * @note Assurez-vous que les macros STACK_SIZE_POSITION_MOTOR et TASK_PRIORITY_POSITION_MOTOR
 *       sont définies avec les valeurs appropriées avant d'appeler cette fonction.
 *
 * @see vTaskPositionMotor, STACK_SIZE_POSITION_MOTOR, TASK_PRIORITY_POSITION_MOTOR
 */
void positionMotorTaskCreate(void)
{
    if (xTaskCreate(vTaskPositionMotor, "Position Motor Task", STACK_SIZE_POSITION_MOTOR, NULL, TASK_PRIORITY_POSITION_MOTOR, &position_motor_task_h) != pdPASS)
    {
        printf("Error position motor task create\r\n");
        Error_Handler();
    }
}


/**
 * @brief Obtient la poignée de la tâche de positionnement moteur.
 *
 * Cette fonction renvoie la poignée (handle) de la tâche de positionnement moteur,
 * permettant ainsi d'accéder à la tâche depuis d'autres parties du programme.
 *
 * @return La poignée (handle) de la tâche de positionnement moteur.
 */
TaskHandle_t getPositionMotorTaskHandle(void)
{
    return position_motor_task_h;
}


/**
 * @brief Tâche de positionnement moteur.
 *
 * Cette tâche gère le positionnement du moteur en réagissant aux notifications reçues.
 * Elle interprète les notifications des capteurs de bordure et de la position Lidar,
 * puis ajuste les commandes des moteurs en conséquence.
 *
 * @param param Pointeur vers les paramètres de la tâche (non utilisé ici).
 */
void vTaskPositionMotor(void *param)
{
    uint32_t notify_value;
    position_t position_to_go = {19.0, 10};

    for (;;)
    {
        if (xTaskNotifyWait(0, ULONG_MAX, &notify_value, portMAX_DELAY) == pdTRUE)
        {
            if (notify_value == BORDER_F_NOTIFY) // Notification ISR capteurs bordure
            {
                // Gauche stop, Droit stop
                command_motor_left.speed = 0;
                command_motor_left.dir   = STOP;
                command_motor_right.speed = 0;
                command_motor_right.dir  = STOP;
            }
            else // Nouvelle notification de position
            {
                // TODO: Choper la structure position_t du Lidar

                float angle = position_to_go.angle;
                uint16_t distance = position_to_go.distance;

                // Gestion du positionnement en angle en priorité
                if (angle != 0 || angle != 180)
                {
                    // Met à jour les états et la vitesse de chaque moteur en fonction de l'angle
                    angleToCommand(angle);
                }
                // Vers la direction en ligne droite une fois que le robot est orienté correctement
                else if (distance > 0)
                {
                    // Met à jour les états et la vitesse de chaque moteur en fonction de la distance
                    distanceToCommand(distance, angle);
                }
            }
        }
    }
}


/**
 * @brief Convertit un angle en commandes moteur.
 *
 * Cette fonction convertit un angle en commandes moteur pour orienter le robot
 * dans la direction spécifiée. Elle ajuste les vitesses et directions des moteurs
 * en fonction des quadrants de l'angle.
 *
 * @param angle Angle de rotation du robot.
 */
void angleToCommand(float angle)
{
    if (angle > 0 && angle <= 90)
    {
        // Gauche en avant, Droit moins vite
        command_motor_left.speed  = 100;
        command_motor_left.dir    = FWD;
        command_motor_right.speed = 20;
        command_motor_right.dir   = FWD;
    }
    else if (angle > 90 && angle <= 180)
    {
        // Gauche en arrière, Droit moins vite
        command_motor_left.speed  = 100;
        command_motor_left.dir    = REV;
        command_motor_right.speed = 20;
        command_motor_right.dir   = REV;
    }
    else if (angle > 180 && angle <= 270)
    {
        // Gauche moins vite, Droit en arrière
        command_motor_left.speed  = 20;
        command_motor_left.dir    = REV;
        command_motor_right.speed = 100;
        command_motor_right.dir   = REV;
    }
    else
    {
        // Gauche moins vite, Droit en avant
        command_motor_left.speed  = 20;
        command_motor_left.dir    = FWD;
        command_motor_right.speed = 100;
        command_motor_right.dir   = FWD;
    }
}


/**
 * @brief Convertit une distance en commandes moteur pour une trajectoire en ligne droite.
 *
 * Cette fonction convertit une distance en commandes moteur pour déplacer le robot
 * dans une trajectoire en ligne droite. Elle ajuste les vitesses et directions des moteurs
 * en fonction de l'angle spécifié (0 ou 180 degrés).
 *
 * @param distance Distance à parcourir en ligne droite.
 * @param angle    Angle de la trajectoire (0 ou 180 degrés).
 */
void distanceToCommand(uint16_t distance, float angle)
{
    if (angle == 180)
    {
        // Gauche en arrière, Droit en arrière
        command_motor_left.speed  = 100;
        command_motor_left.dir    = REV;
        command_motor_right.speed = 100;
        command_motor_right.dir   = REV;
    }
    else if (angle == 0)
    {
        // Gauche en avant, Droit en avant
        command_motor_left.speed  = 100;
        command_motor_left.dir    = FWD;
        command_motor_right.speed = 100;
        command_motor_right.dir   = FWD;
    }
}
