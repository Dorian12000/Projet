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
#include "cmsis_os.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "log/logger.h"
#include "log/types.h"

#include "lidarTask.h"
#include "drv_lidar.h"

static lidar_t lidar;
static SemaphoreHandle_t sem_uart_read = NULL;
static uint8_t lidarScanRawData[HEADER_SIZE + SCAN_RESPONSE_SIZE + 100 * Si_SIZE];
static lidar_scan_t lidarScanData;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1)
	{
		LOG_LIDAR_ENTER();
		BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
		xSemaphoreGiveFromISR(sem_uart_read, &pxHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
		if(isLidarScanning()) {
			LidarScanStop();
		}
	}
}

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

returncode_t LidarInit(void) {
	returncode_t ret = success;
	lidar.uart.tx = LidarUarTx;
	lidar.uart.rx = LidarUartRx;
	
	sem_uart_read = xSemaphoreCreateBinary();
	if(sem_uart_read == NULL) {
		return memoryfail;
	}
	LIDAR_DEV_ENABLE();
	//LIDAR_MOTOR_ENABLE();
	LIDAR_MOTOR_DISABLE();
	LidarSetSpeed(0);
	return ret;
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

returncode_t LidarGetInformation(lidar_devEUI_t *devEUI) {
	LOG_LIDAR_ENTER();
	HAL_StatusTypeDef status;
	uint8_t command[COMMAND_SIZE] = {(LIDAR_GET_DEV_ID & 0xFF00) >> 8, (LIDAR_GET_DEV_ID & 0x00FF)};
	status = lidar.uart.tx(command, NULL, 0);
	if(status != HAL_OK) {
		LOG_LIDAR_ERROR("transmit error: %d", status);
		return transmission_failed;
	}

	uint8_t *version = (uint8_t*) malloc((DEVEUI_SIZE + HEADER_SIZE) * sizeof(uint8_t));
	if(version == NULL) {
		return mallocfail;
	}
	status = lidar.uart.rx(NULL, version, (DEVEUI_SIZE + HEADER_SIZE));
	if(status == HAL_OK) {
		xSemaphoreTake(sem_uart_read, portMAX_DELAY);	// Wait for DMA Notification
		LOG_LIDAR_DEBUG("version: %b", version, (DEVEUI_SIZE + HEADER_SIZE));
		if(strncmp(version, (uint8_t[]){0xA5, 0x5A}, 2) == 0) {
			LOG_LIDAR_DEBUG(COLOR_GREEN"header OK");
			memcpy(&(devEUI->ModelNumber), &(version[HEADER_SIZE]), MODEL_NUMBER_SIZE);
			memcpy(&(devEUI->FirmWareVersion), &(version[HEADER_SIZE+1]), FIRMWARE_VERSION_SIZE);
			memcpy(&(devEUI->HardWareVersion), &(version[HEADER_SIZE+3]), HARDWARE_VERSION_SIZE);
			memcpy(&(devEUI->SerialNumber), &(version[HEADER_SIZE+4]), SERIAL_NUMBER_SIZE);
		}
		else {
			LOG_LIDAR_ERROR("Header error %b", version, 2);
			free(version);
			return wrongparameter;
		}
	}
	else {
		LOG_LIDAR_ERROR("received error");
		free(version);
		return transmission_failed;
	}
	free(version);
	return success;
}


returncode_t LidarScanStart(void){
	LOG_LIDAR_ENTER();
	HAL_StatusTypeDef status;
	uint8_t command[COMMAND_SIZE] = {(LIDAR_SCAN_START & 0xFF00) >> 8, (LIDAR_SCAN_START & 0x00FF)};
	status = lidar.uart.tx(command, NULL, 0);
	if(status != HAL_OK) {
		LOG_LIDAR_ERROR("transmit error: %d", status);
		return transmission_failed;
	}

	status = lidar.uart.rx(NULL, lidarScanRawData, (HEADER_SIZE + SCAN_RESPONSE_SIZE + 100 * Si_SIZE));
	if(status == HAL_OK) {
		return success;
	}
	else {
		return unknownerror;
	}
	
}

returncode_t waitLidarScanData(void) {
	if(xSemaphoreTake(sem_uart_read, portMAX_DELAY) == pdTRUE) {	// Wait for DMA Notification
		//LOG_LIDAR_DEBUG("lidarScanRawData: %b", lidarScanRawData, (HEADER_SIZE + SCAN_RESPONSE_SIZE + 100 * Si_SIZE));
		if(strncmp(lidarScanRawData, (uint8_t[]){0xA5, 0x5A}, 2) == 0) {
			memcpy(lidarScanData.PH, &(lidarScanRawData[HEADER_SIZE]), PH_SIZE);
			memcpy(lidarScanData.CT, &(lidarScanRawData[HEADER_SIZE+2]), CT_SIZE);
			memcpy(lidarScanData.LSN, &(lidarScanRawData[HEADER_SIZE+3]), LSN_SIZE);
			memcpy(lidarScanData.FSA, &(lidarScanRawData[HEADER_SIZE+4]), FSA_SIZE);
			memcpy(lidarScanData.LSA, &(lidarScanRawData[HEADER_SIZE+6]), LSA_SIZE);
			memcpy(lidarScanData.CS, &(lidarScanRawData[HEADER_SIZE+8]), CS_SIZE);
			memcpy(lidarScanData.SI, &(lidarScanRawData[HEADER_SIZE+10]), Si_SIZE * 50);
			return success;
		}
		else {
			LOG_LIDAR_ERROR("Header error %b", lidarScanRawData, 2);
			return wrongparameter;
		}
	}
	else {
		return transmission_no_response;
	}
}
returncode_t LidarScanStop(void) {
	LOG_LIDAR_ENTER();
	HAL_StatusTypeDef status;
	uint8_t command[COMMAND_SIZE] = {(LIDAR_SCAN_STOP & 0xFF00) >> 8, (LIDAR_SCAN_STOP & 0x00FF)};
	status = lidar.uart.tx(command, NULL, 0);
	if(status != HAL_OK) {
		LOG_LIDAR_ERROR("transmit error: %d", status);
		return transmission_failed;
	}
	return success;
}


returncode_t LidarHealthStatus(lidar_healthStatus_t *healthStatus) {
	HAL_StatusTypeDef status;
	uint8_t command[COMMAND_SIZE] = {(LIDAR_GET_HEALTH_STATUS & 0xFF00) >> 8, (LIDAR_GET_HEALTH_STATUS & 0x00FF)};
	status = lidar.uart.tx(command, NULL, 0);
	if(status != HAL_OK) {
		LOG_LIDAR_ERROR("transmit error: %d", status);
	}
	uint8_t *buf = (uint8_t*) malloc((HEADER_SIZE + HEALTH_STATUS_SIZE) * sizeof(uint8_t));
	if(buf == NULL) {
		return mallocfail;
	}
	status = lidar.uart.rx(NULL, buf,  (HEADER_SIZE + HEALTH_STATUS_SIZE));
	if(status == HAL_OK) {
		xSemaphoreTake(sem_uart_read, portMAX_DELAY);	// Wait for DMA Notification
		LOG_LIDAR_DEBUG("buf: %b", buf, (HEADER_SIZE + HEALTH_STATUS_SIZE));
		if(strncmp(buf, (uint8_t[]){0xA5, 0x5A}, 2) == 0) {
			memcpy(&(healthStatus->StatusCode), &(buf[HEADER_SIZE]), STATUS_CODE_SIZE);
			memcpy(&(healthStatus->ErrorCode), &(buf[HEADER_SIZE + 1]), ERROR_CODE_SIZE);
		}
		else {
			LOG_LIDAR_ERROR("Header error %b", buf, 2);
		}
	}
	else {
		LOG_LIDAR_ERROR("reception error");
		free(buf);
		return transmission_failed;
	}
	free(buf);
}

returncode_t lidarDataProcess(void) {
	LOG_LIDAR_ENTER();
	LOG_LIDAR_INFO("scan data si: %b", lidarScanData.SI, 100);
}