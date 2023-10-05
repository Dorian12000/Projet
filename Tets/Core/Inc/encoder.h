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

uint32_t readEncoder(TIM_HandleTypeDef *enc_timer);
uint32_t convertEncoderToSpeed(uint32_t enc_prev, uint32_t enc_curr, uint16_t time_ms);
float convertEncoderToPosition(uint32_t enc_prev, uint32_t enc_curr);

#endif /* INC_ENCODER_H_ */
