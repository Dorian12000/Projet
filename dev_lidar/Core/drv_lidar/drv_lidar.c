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

uint8_t buf[20];
lidar_t lidar;

static inline HAL_StatusTypeDef LidarUarTx(uint8_t *address, uint8_t *p_data, uint16_t size) {
	HAL_StatusTypeDef status;
	//LOG_MAIN_DEBUG("%02X%02X", address[0], address[1]);
	status = HAL_UART_Transmit(&huart1, address, 2, 500);
	//HAL_UART_Transmit(&huart1, p_data, size, HAL_MAX_DELAY);
	return status;
}

static inline HAL_StatusTypeDef LidarUartRx(uint8_t *address, uint8_t *p_data, uint16_t size) {
	HAL_StatusTypeDef status;
	status = HAL_UART_Receive(&huart1, p_data, size, 500);
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
	uint8_t command[2] = {0xA5, 0x90};
	//status = lidar.uart.tx(command, NULL, 0);
	status = HAL_UART_Transmit(&huart1, command, 2, 500);
	if(status != HAL_OK) {
		//LOG_MAIN_ERROR("transmit error: %d", status);
	}
	uint8_t *buf = (uint8_t*) malloc(20+7 * sizeof(uint8_t));
	//lidar.uart.rx(NULL, buf, 20);
	if(HAL_UART_Receive(&huart1, buf, 20+7, 500) == HAL_OK) {
		//LOG_MAIN_DEBUG("buf: %b", buf, 20+7);
		memcpy(&(devEUI->ModelNumber), &(buf[7]), 1);
		memcpy(&(devEUI->FirmWareVersion), &(buf[7+1]), 2);
		memcpy(&(devEUI->HardWareVersion), &(buf[7+3]), 1);
		memcpy(&(devEUI->SerialNumber), &(buf[7+4]), 16);
	}

	free(buf);
}


void LidarScanStart(lidar_scan_t *lidscan){
	HAL_StatusTypeDef status;
	uint8_t Startcmd[2]={0xA5,0x60};
	status = HAL_UART_Transmit(&huart1,Startcmd,2,500);
	if(status != HAL_OK) {
			LOG_MAIN_ERROR("transmit error: %d", status);
		}
	uint8_t *buf = (uint8_t*) malloc(20+7 * sizeof(uint8_t));
		//lidar.uart.rx(NULL, buf, 20);
		if(HAL_UART_Receive(&huart1, buf, 20+7, 500) == HAL_OK) {
			LOG_MAIN_DEBUG("buf: %b", buf, 20+7);
		}
		memcpy(&(lidscan->PH), &(buf[7]), 2);
		memcpy(&(lidscan->CT), &(buf[7+2]), 1);
		memcpy(&(lidscan->LSN), &(buf[7+3]), 1);
		memcpy(&(lidscan->FSA), &(buf[7+4]), 2);
		memcpy(&(lidscan->LSA), &(buf[7+6]), 2);
		memcpy(&(lidscan->CS), &(buf[7+8]), 2);
		memcpy(&(lidscan->SI), &(buf[7+10]),10);

		free(buf);
}
