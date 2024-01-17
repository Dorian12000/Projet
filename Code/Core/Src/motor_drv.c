/*
 * motor_drv.c
 *
 *  Created on: Dec 1, 2023
 *      Author: gourd
 */

#include "motor_drv.h"
#include <stdlib.h>

h_motor_t motor_left;
h_motor_t motor_right;

void initMotor(h_motor_t *motor, PWM *fwd, PWM *rev, TIM_HandleTypeDef *htim_enc)
{
	motor->state = STOP;
	motor->fwd = fwd;
	motor->rev = rev;
	motor->htim_enc = htim_enc;
}

/**
 * @brief Configure la vitesse du moteur en marche avant.
 *
 * Cette fonction configure la vitesse du moteur en marche avant en ajustant le rapport
 * cyclique PWM. Si le moteur était précédemment en marche arrière, le rapport cyclique
 * correspondant à la marche arrière est ajusté à zéro.
 *
 * @param motor Pointeur vers la structure représentant le moteur.
 * @param speed Vitesse du moteur en marche avant.
 */
void setMotorSpeedFwd(h_motor_t *motor, uint8_t speed)
{
    if (motor->state == REV)
    {
        setPwmDutyCycle(motor->rev, 0, MAX_DUTY_CYCLE_MOTOR);
    }

    motor->state = FWD;
    setPwmDutyCycle(motor->fwd, speed, MAX_DUTY_CYCLE_MOTOR);
}


/**
 * @brief Configure la vitesse du moteur en marche arrière.
 *
 * Cette fonction configure la vitesse du moteur en marche arrière en ajustant le rapport
 * cyclique PWM. Si le moteur était précédemment en marche avant, le rapport cyclique
 * correspondant à la marche avant est ajusté à zéro.
 *
 * @param motor Pointeur vers la structure représentant le moteur.
 * @param speed Vitesse du moteur en marche arrière.
 */
void setMotorSpeedRev(h_motor_t *motor, uint8_t speed)
{
    if (motor->state == FWD)
    {
        setPwmDutyCycle(motor->fwd, 0, MAX_DUTY_CYCLE_MOTOR);
    }

    motor->state = REV;
    setPwmDutyCycle(motor->rev, speed, MAX_DUTY_CYCLE_MOTOR);
}


/**
 * @brief Arrête le moteur en ajustant le rapport cyclique PWM.
 *
 * Cette fonction arrête le moteur en ajustant le rapport cyclique PWM à zéro.
 * La direction du moteur est mise à l'arrêt, indiquée par l'état "STOP".
 *
 * @param motor Pointeur vers la structure représentant le moteur.
 */
void stopMotor(h_motor_t *motor)
{
    if (motor->state == REV)
    {
        setPwmDutyCycle(motor->rev, 0, MAX_DUTY_CYCLE_MOTOR);
    }
    else if (motor->state == FWD)
    {
        setPwmDutyCycle(motor->fwd, 0, MAX_DUTY_CYCLE_MOTOR);
    }
    motor->state = STOP;
}


/**
 * @brief Configure la vitesse et la direction du moteur.
 *
 * Cette fonction configure la vitesse du moteur en fonction de la direction spécifiée.
 * Si la direction est "REV" (marche arrière), la fonction appelle setMotorSpeedRev.
 * Si la direction est "FWD" (marche avant), la fonction appelle setMotorSpeedFwd.
 * Si la direction est "STOP" (arrêt), la fonction appelle stopMotor.
 *
 * @param motor Pointeur vers la structure représentant le moteur.
 * @param speed Vitesse du moteur.
 * @param dir   Direction du moteur (REV, FWD, STOP).
 */
void setMotorSpeed(h_motor_t *motor, uint8_t speed, h_motor_state dir)
{
    if (dir == REV)
    {
        setMotorSpeedRev(motor, speed);
    }
    else if (dir == FWD)
    {
        setMotorSpeedFwd(motor, speed);
    }
    else if (dir == STOP)
    {
        stopMotor(motor);
    }
}


/**
 * @brief Calcule la vitesse du moteur en RPM.
 *
 * Cette fonction calcule la vitesse du moteur en tours par minute (RPM) en utilisant
 * les valeurs des compteurs d'encodeur précédentes et actuelles, ainsi que le temps écoulé.
 * La vitesse calculée est normalisée dans la plage de 0 à 100.
 *
 * @param motor    Pointeur vers la structure représentant le moteur.
 * @param enc_prev Valeur précédente du compteur d'encodeur.
 * @param enc_curr Valeur actuelle du compteur d'encodeur.
 * @param time_ms  Temps écoulé en millisecondes.
 *
 * @return Vitesse du moteur normalisée entre 0 et 100.
 */
uint8_t getSpeed(h_motor_t *motor, uint32_t enc_prev, uint32_t enc_curr, uint16_t time_ms)
{
    uint16_t time_s = time_ms / 1000;
    uint32_t distance = 0;

    if (motor->state == REV)
    {
        if (enc_prev < enc_curr) // Si dépassement de la valeur max du compteur
        {
            enc_prev = MAX_ENCODER_VALUE - enc_prev;
            distance = enc_prev + enc_curr;
        }
        else
        {
            distance = enc_curr - enc_prev;
        }
    }
    else if (motor->state == FWD)
    {
        if (enc_prev < enc_curr) // Si dépassement de la valeur max du compteur
        {
            enc_curr = MAX_ENCODER_VALUE - enc_curr;
            distance = enc_prev + enc_curr;
        }
        else
        {
            distance = enc_prev - enc_curr;
        }
    }

    distance /= 4;
    distance = distance * 60 / time_s;
    uint32_t speed = distance / (int)RES_ENCODER; // RPM
    return (uint8_t)(100 * speed / MAX_RPM); // Vitesse entre 0 et 100
}
