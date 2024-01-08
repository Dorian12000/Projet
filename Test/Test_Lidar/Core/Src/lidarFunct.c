/*
 * lidarFunct.c
 *
 *  Created on: 2024
 *      Author : Laksan Thirukumaran
 *
 */


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

#include <LidarFunct.h>
#include "main.h"
#include "usart.h"
#include "tim.h"
#include "gpio.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

static lidar_t lidar;


static inline HAL_StatusTypeDef LidarUarTx(uint8_t *address, uint8_t *p_data, uint16_t size) {
	HAL_StatusTypeDef status;
	status = HAL_UART_Transmit_DMA(&huart1, address, 2);
	return status;
}


static inline HAL_StatusTypeDef LidarUartRx(uint8_t *p_data, uint16_t size) {
	return HAL_UART_Receive_DMA(&huart1, p_data, size);
}


void LidarInit(void) {
	printf("Lidar Init \n");
	lidar.uart.tx = LidarUarTx;
	lidar.uart.rx = LidarUartRx;
	LIDAR_DEV_ENABLE();
	LIDAR_MOTOR_DISABLE();
	LidarSetSpeed(0);
	printf("Init ok \n");
}


void LidarSetSpeed(uint8_t speed) {
	speed += 49;
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1 ,speed);
}


void  LidarGetInformation(lidar_devEUI_t *devEUI) {
	HAL_StatusTypeDef status;
	uint8_t command[COMMAND_SIZE] = {(LIDAR_GET_DEV_ID & 0xFF00) >> 8, (LIDAR_GET_DEV_ID & 0x00FF)};
	status = lidar.uart.tx(command, NULL, 0);
	if(status != HAL_OK) {
		printf("transmission_failed\n");
		return 0;
	}

	uint8_t version[DEVEUI_SIZE + HEADER_SIZE];

	status = lidar.uart.rx(version, (DEVEUI_SIZE + HEADER_SIZE));
	if(status == HAL_OK) {
		if(strncmp(version, (uint8_t[]){0xA5, 0x5A}, 2) == 0) {
			memcpy(&(devEUI->ModelNumber), &(version[HEADER_SIZE]), MODEL_NUMBER_SIZE);
			memcpy(&(devEUI->FirmWareVersion), &(version[HEADER_SIZE+1]), FIRMWARE_VERSION_SIZE);
			memcpy(&(devEUI->HardWareVersion), &(version[HEADER_SIZE+3]), HARDWARE_VERSION_SIZE);
			memcpy(&(devEUI->SerialNumber), &(version[HEADER_SIZE+4]), SERIAL_NUMBER_SIZE);
		}
		else {
			printf("wrongparams\n");
		}
	}
	else {
		printf("transmission_failed\n");
	}
}



void LidarScanStart(void){

	printf("SCAN START \n");
	HAL_StatusTypeDef status;
	LIDAR_DEV_ENABLE();
	LIDAR_MOTOR_ENABLE();
	uint8_t command[COMMAND_SIZE] = {(LIDAR_SCAN_START & 0xFF00) >> 8, (LIDAR_SCAN_START & 0x00FF)};
	status = lidar.uart.tx(command, NULL, 0);
	if(status != HAL_OK) {
		printf("transmission_failed\n");
	}
}

void  LidarScanStop(void) {
	LIDAR_MOTOR_DISABLE();
	HAL_StatusTypeDef status;
	uint8_t command[COMMAND_SIZE] = {(LIDAR_SCAN_STOP & 0xFF00) >> 8, (LIDAR_SCAN_STOP & 0x00FF)};
	status = lidar.uart.tx(command, NULL, 0);
	if(status != HAL_OK) {
		printf("transmission_failed\n");
	}
}




void  lidarRestart(void) {
	HAL_StatusTypeDef status;
	uint8_t command[COMMAND_SIZE] = {(LIDAR_RESTART & 0xFF00) >> 8, (LIDAR_RESTART & 0x00FF)};
	status = lidar.uart.tx(command, NULL, 0);
	if(status != HAL_OK) {
		printf("Transmit error\n");
	}
}

int  checkCS(lidar_scan_t *lidarData) {
	int check=0;
	uint16_t calculatedCS = ((lidarData->PH[0] << 8) + lidarData->PH[1])^((lidarData->FSA[0] << 8) + lidarData->FSA[1]);
	for (int i = 0; i < lidarData->LSN; i += 2) {
		calculatedCS ^= ((lidarData->SI[i] << 8) + lidarData->SI[i+1]);
	}
	calculatedCS ^= ((lidarData->CT << 8) + lidarData->LSN)^((lidarData->LSA[0] << 8) + lidarData->LSA[1]);
	if(((lidarData->CS[0] << 8) + lidarData->CS[1]) == calculatedCS) {
		check=1;
		return check;
	}
	else {
		return check;
	}
}

void  getLidarScanData(lidar_scan_t *lidarScanData) {
	HAL_StatusTypeDef status = 0;
	int size = 0;
	uint8_t lidarScanRawData[256];
	status = lidar.uart.rx(lidarScanRawData, 256);
	if(status == HAL_OK) {
		//if (xSemaphoreTake(sem_uart_read, 100) == pdTRUE) {	// Wait for DMA Notification
		//LidarScanStop();
		//printf(COLOR_MAGENTA"*** %s\n", CONVERT_TO_STRING(lidarScanRawData, 256));

		int j = 0;
		for(j = 0; j < 256; j++) {
			if(strncmp(&(lidarScanRawData[j]), (uint8_t[]){0xA5, 0x5A}, 2) == 0) {
				printf("main header detected @ %d\n", j);
				memcpy(lidarScanData->PH, &(lidarScanRawData[HEADER_SIZE]), PH_SIZE);
				memcpy(&(lidarScanData->CT), &(lidarScanRawData[HEADER_SIZE + 2]), CT_SIZE);
				memcpy(&(lidarScanData->LSN), &(lidarScanRawData[HEADER_SIZE + 3]), LSN_SIZE);
				memcpy(lidarScanData->FSA, &(lidarScanRawData[HEADER_SIZE + 4]), FSA_SIZE);
				memcpy(lidarScanData->LSA, &(lidarScanRawData[HEADER_SIZE + 6]), LSA_SIZE);
				memcpy(lidarScanData->CS, &(lidarScanRawData[HEADER_SIZE + 8]), CS_SIZE);
				//memcpy(lidarScanData->SI, &(lidarScanRawData[HEADER_SIZE + j + 10]), lidarScanData->LSN);
				for(int k = 0; k < lidarScanData->LSN; k++) {
					lidarScanData->SI[k] = lidarScanRawData[HEADER_SIZE + k + 10];
				}
				break;
			}
			else if(strncmp(&(lidarScanRawData[j]), (uint8_t[]){0xAA, 0x55}, 2) == 0) {
				//LOG_LIDAR_DEBUG("header detected @ %d", j);
				memcpy(lidarScanData->PH, &(lidarScanRawData[j]), PH_SIZE);
				memcpy(&(lidarScanData->CT), &(lidarScanRawData[j + 2]), CT_SIZE);
				memcpy(&(lidarScanData->LSN), &(lidarScanRawData[j + 3]), LSN_SIZE);
				memcpy(lidarScanData->FSA, &(lidarScanRawData[j + 4]), FSA_SIZE);
				memcpy(lidarScanData->LSA, &(lidarScanRawData[j + 6]), LSA_SIZE);
				memcpy(lidarScanData->CS, &(lidarScanRawData[j  + 8]), CS_SIZE);
				//memcpy(lidarScanData->SI, &(lidarScanRawData[j + 10]), lidarScanData->LSN);
				for(int k = 0; k < lidarScanData->LSN; k++) {
					lidarScanData->SI[k] = lidarScanRawData[k + 10];
				}
				break;
			}
		}
		if(j > 255) {
			printf("wrong_size\n");

		}
	}
	else {
		printf("transmission_failed\n");


	}

}
void convertSample(lidar_scan_t *lidarData) { //TODO
	/******************** convert distance ***********************/
	uint16_t sampleSize = lidarData->LSN/Si_SIZE;
	//uint16_t distance[sampleSize];
	position_t position[sampleSize];
	int i = 0;
	for(i = 0; i < sampleSize; i++) {
		//distance[i] = ((lidarData->SI[0] << 8) + lidarData->SI[1])/4;
		position[i].distance = ((lidarData->SI[i] << 8) + lidarData->SI[i+1])/4; // distance in mm
	}

	/******************** angle ***********************/
	float angleFSA, angleLSA, deltaAngle;
	angleFSA = ((((lidarData->FSA[1]) << 8) + lidarData->FSA[0]) >> 1)/64;
	angleLSA = ((((lidarData->LSA[1]) << 8) + lidarData->LSA[0]) >> 1)/64;
	if(angleLSA < angleFSA) {
		angleLSA += 360;
	}
	deltaAngle = angleLSA - angleFSA;

	/******************** angle correction & convertion ***********************/
	//float correctAngle[sampleSize];
	for(i = 0; i < sampleSize; i++) {
		if(position[i].distance>0) {
			//correctAngle[i] = deltaAngle/sampleSize * i + angleFSA + (atan(21.8 * (155.3 - distance[i]) / (155.3 * distance[i])) * 180/3.14);
			position[i].angle = deltaAngle/sampleSize * i + angleFSA + (atan(21.8 * (155.3 - position[i].distance) / (155.3 * position[i].distance)) * 180/3.14);
			if(position[i].angle >= 360.0f) {
				position[i].angle -= 360.0f;
			}
		}
	}
	memset(lidar.position, 0, 100);
	memcpy(lidar.position, position, sampleSize);
	printf("Conversion ok\n");
}

void LidarDist(lidar_scan_t *lidarScanData) {
	uint16_t sampleSize = lidarScanData->LSN / Si_SIZE;
	position_t position[sampleSize];

	for (int i = 0; i < sampleSize; i++) {
		uint16_t sample = (lidarScanData->SI[i * Si_SIZE] << 8) + lidarScanData->SI[i * Si_SIZE + 1];
		position[i].distance = sample / 4; // distance en mm
		printf("Distance pour l'échantillon %d : %u mm\n", i, position[i].distance);
	}
}

