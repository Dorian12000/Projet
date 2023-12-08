/*
 * encoder.h
 *
 *  Created on: Oct 5, 2023
 *      Author: gourd
 */

#ifndef INC_ENCODER_H_
#define INC_ENCODER_H_

#include <stdint.h>
#include "tim.h"

#define RES_ENCODER 224.4 // Impulsions de l'encoder pour 1 tour

// Macros
#define READ_ENCODER(enc_timer) (enc_timer).Instance->CNT

float convertEncoderToSpeed(uint32_t enc_prev, uint32_t enc_curr, uint16_t time_ms);

#endif /* INC_ENCODER_H_ */
