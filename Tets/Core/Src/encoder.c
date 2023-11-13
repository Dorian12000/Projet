/*
 * encoder.c
 *
 *  Created on: Oct 5, 2023
 *      Author: Gaël
 */

#include "encoder.h"

float convertEncoderToSpeed(uint32_t enc_prev, uint32_t enc_curr, uint16_t time_ms)
{
	// Conversion en tour par minute
	uint32_t distance = (enc_curr - enc_prev) / RES_ENCODER;
	return ((float) distance * 60000.0f) /(time_ms);
}

