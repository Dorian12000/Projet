/*
 * pid.c
 *
 *  Created on: Dec 1, 2023
 *      Author: gourd
 */


#include "pid.h"

PID pid_motor_left;
PID pid_motor_right;


/**
 * @brief Initialise un contrôleur PID avec les paramètres spécifiés.
 *
 * Cette fonction initialise un contrôleur PID en calculant les coefficients
 * en fonction des gains proportionnel, intégral, dérivatif et de la période d'échantillonnage.
 *
 * @param pid     Pointeur vers la structure PID à initialiser.
 * @param kp      Gain proportionnel.
 * @param ki      Gain intégral.
 * @param kd      Gain dérivatif.
 * @param Te_ms   Période d'échantillonnage en millisecondes.
 */
void initPID(PID *pid, float kp, float ki, float kd, float Te_ms)
{
    float Te = Te_ms / 1000;
    float a = Te / 2;

    pid->b0 = kp + ki * a + kd * a;
    pid->b1 = ki * Te + kd * Te;
    pid->b2 = kp + ki * a + kd * a;
    pid->index = 0;

    for (int i = 0; i < PID_ORDRE; i++)
    {
        pid->inputs[i]  = 0.0;
        pid->outputs[i] = 0.0;
    }
}


/**
 * @brief Calcule l'erreur entre la consigne de vitesse et la vitesse mesurée.
 *
 * Cette fonction met à jour l'entrée actuelle du contrôleur PID avec l'erreur calculée
 * entre la consigne de vitesse (commande) et la vitesse mesurée.
 *
 * @param pid        Pointeur vers la structure PID.
 * @param speed_cmd  Consigne de vitesse.
 * @param speed_mes  Vitesse mesurée.
 */
void error(PID *pid, uint8_t speed_cmd, uint8_t speed_mes)
{
    pid->inputs[pid->index] = speed_mes - speed_cmd;
}


/**
 * @brief Applique le correcteur PID et renvoie la sortie corrigée.
 *
 * Cette fonction applique le correcteur PID en utilisant les coefficients et
 * l'entrée actuelle du contrôleur PID, puis met à jour l'index pour la prochaine itération.
 *
 * @param pid Pointeur vers la structure PID.
 *
 * @return La sortie corrigée du contrôleur PID.
 */
uint8_t correcteur(PID *pid)
{
    uint8_t idx = pid->index;

    pid->outputs[idx] = pid->outputs[(idx + 2) % PID_ORDRE]
                       + pid->b0 * pid->inputs[idx]
                       + pid->b1 * pid->inputs[(idx + 1) % PID_ORDRE]
                       + pid->b2 * pid->inputs[(idx + 2) % PID_ORDRE];

    //printf("Sortie pid = %f\r\n", outputs[idx]);
    pid->index = (idx + 1) % PID_ORDRE;

    return pid->outputs[idx];
}
