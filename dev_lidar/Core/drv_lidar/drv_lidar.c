/**
 *     _______    _____     __     ______    _______        ____
 *    |   ____|  |     \   |  |   / _____)  |   ____|      /    \
 *    |  |__     |  |\  \  |  |  ( (____    |  |__        /  /\  \
 *    |   __|    |  | \  \ |  |   \____ \   |   __|      /  ____  \
 *    |  |____   |  |  \  \|  |   _____) )  |  |____    /  /    \  \
 *    |_______|  |__|   \_____|  (______/   |_______|  /__/      \__\
 *
 * @file    drv_lidar.c
 * @Author  Dorian Dalbin, Clement Du, Laksan Thirukumaran, Gael Gourdin
 * @Created	2023-10-28
 * @brief
 *
 **/

#include "main.h"
#include "usart.h"
#include "tim.h"
#include "gpio.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../Inc/log/logger.h"

#include "drv_lidar.h"

lidar_t lidar;

static inline HAL_StatusTypeDef LidarUarTx(uint8_t *address, uint8_t *p_data, uint16_t size) {
	HAL_StatusTypeDef status;
	LOG_LIDAR_DEBUG("transmit 0x%02X%02X", address[0], address[1]);
	status = HAL_UART_Transmit_DMA(&huart1, address, 2);
	return status;
}

static inline HAL_StatusTypeDef LidarUartRx(uint8_t *address, uint8_t *p_data, uint16_t size) {
	HAL_StatusTypeDef status;
	status = HAL_UART_Receive_DMA(&huart1, p_data, size);
	return status;
}

void LidarInit(void) {
	lidar.uart.tx = LidarUarTx;
	lidar.uart.rx = LidarUartRx;
}

/**
 * @brief Définit la vitesse du Lidar.
 *
 * Cette fonction permet de régler la vitesse du Lidar.
 *
 * @param speed La nouvelle valeur de vitesse à définir.
 *
 * @note 0: vitesse minimale
 * @note 50: vitesse maximale.
 */
void LidarSetSpeed(uint8_t speed) {
	speed += 49;
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1 ,speed);
}

void LidarGetInformation(lidar_devEUI_t *devEUI) {
	HAL_StatusTypeDef status;
	uint8_t command[COMMAND_SIZE] = {(LIDAR_GET_DEV_ID & 0xFF00) >> 8, (LIDAR_GET_DEV_ID & 0x00FF)};
	status = lidar.uart.tx(command, NULL, 0);
	if(status != HAL_OK) {
		LOG_LIDAR_ERROR("transmit error: %d", status);
	}

	uint8_t *version = (uint8_t*) malloc((DEVEUI_SIZE + HEADER_SIZE) * sizeof(uint8_t));
	status = lidar.uart.rx(NULL, version, (DEVEUI_SIZE + HEADER_SIZE));
	if(status == HAL_OK) {
		LOG_LIDAR_DEBUG("version: %b", version, (DEVEUI_SIZE + HEADER_SIZE));
		memcpy(&(devEUI->ModelNumber), &(version[HEADER_SIZE]), MODEL_NUMBER_SIZE);
		memcpy(&(devEUI->FirmWareVersion), &(version[HEADER_SIZE+1]), FIRMWARE_VERSION_SIZE);
		memcpy(&(devEUI->HardWareVersion), &(version[HEADER_SIZE+3]), HARDWARE_VERSION_SIZE);
		memcpy(&(devEUI->SerialNumber), &(version[HEADER_SIZE+4]), SERIAL_NUMBER_SIZE);
	}
	free(version);
}


void LidarScanStart(lidar_scan_t *lidscan){
	HAL_StatusTypeDef status;
	uint8_t command[COMMAND_SIZE] = {(LIDAR_SCAN_START & 0xFF00) >> 8, (LIDAR_SCAN_START & 0x00FF)};
	status = lidar.uart.tx(command, NULL, 0);
	if(status != HAL_OK) {
		LOG_LIDAR_ERROR("transmit error: %d", status);
	}

	uint8_t *buf = (uint8_t*) malloc((HEADER_SIZE + SCAN_RESPONSE_SIZE + 100 * Si_SIZE) * sizeof(uint8_t));
	lidar.uart.rx(NULL, buf, (HEADER_SIZE + SCAN_RESPONSE_SIZE + 100 * Si_SIZE));
	if(status == HAL_OK) {
		LOG_LIDAR_DEBUG("buf: %b", buf, (HEADER_SIZE + SCAN_RESPONSE_SIZE + 100 * Si_SIZE));
		memcpy(&(lidscan->PH), &(buf[HEADER_SIZE]), PH_SIZE);
		memcpy(&(lidscan->CT), &(buf[HEADER_SIZE+2]), CT_SIZE);
		memcpy(&(lidscan->LSN), &(buf[HEADER_SIZE+3]), LSN_SIZE);
		memcpy(&(lidscan->FSA), &(buf[HEADER_SIZE+4]), FSA_SIZE);
		memcpy(&(lidscan->LSA), &(buf[HEADER_SIZE+6]), LSA_SIZE);
		memcpy(&(lidscan->CS), &(buf[HEADER_SIZE+8]), CS_SIZE);
		memcpy(&(lidscan->SI), &(buf[HEADER_SIZE+10]), Si_SIZE);
	}
	free(buf);
}

void LidarScanStop(void) {
	HAL_StatusTypeDef status;
	uint8_t command[COMMAND_SIZE] = {(LIDAR_SCAN_STOP & 0xFF00) >> 8, (LIDAR_SCAN_STOP & 0x00FF)};
	status = lidar.uart.tx(command, NULL, 0);
	if(status != HAL_OK) {
		LOG_LIDAR_ERROR("transmit error: %d", status);
	}
}


void LidarHealthStatus(lidar_healthStatus_t *healthStatus) {
	HAL_StatusTypeDef status;
	uint8_t command[COMMAND_SIZE] = {(LIDAR_GET_HEALTH_STATUS & 0xFF00) >> 8, (LIDAR_GET_HEALTH_STATUS & 0x00FF)};
	status = lidar.uart.tx(command, NULL, 0);
	if(status != HAL_OK) {
		LOG_LIDAR_ERROR("transmit error: %d", status);
	}
	uint8_t *buf = (uint8_t*) malloc((HEADER_SIZE + HEALTH_STATUS_SIZE) * sizeof(uint8_t));
	status = lidar.uart.rx(NULL, buf,  (HEADER_SIZE + HEALTH_STATUS_SIZE));
	if(status == HAL_OK) {
		LOG_LIDAR_DEBUG("buf: %b", buf, (HEADER_SIZE + HEALTH_STATUS_SIZE));
		memcpy(&(healthStatus->StatusCode), &(buf[HEADER_SIZE]), STATUS_CODE_SIZE);
		memcpy(&(healthStatus->ErrorCode), &(buf[HEADER_SIZE + 1]), ERROR_CODE_SIZE);
	}
	else {
		LOG_LIDAR_ERROR("reception error");
	}
	free(buf);
}
