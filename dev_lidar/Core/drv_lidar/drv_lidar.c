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
#include <math.h>

#include "log/logger.h"
#include "log/types.h"

#include "lidarTask.h"
#include "drv_lidar.h"

#define RAYON 0.0637 //rayon du lidar en m

static lidar_t lidar;
static SemaphoreHandle_t sem_uart_read = NULL;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART1)
	{
		BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
		xSemaphoreGiveFromISR(sem_uart_read, &pxHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
	}
}

/**
 * @brief Transmit data through UART for Lidar communication.
 *
 * This function uses Direct Memory Access (DMA) to transmit data through UART.
 *
 * @param[in] address Pointer to the destination address.
 * @param[in] p_data Pointer to the data to be transmitted.
 * @param[in] size Size of the data to be transmitted.
 *
 * @return
 *   - #HAL_OK: Transmission was successful.
 *   - #HAL_ERROR: An error occurred during transmission.
 *
 * @note This function is specific to the UART communication for Lidar devices.
 * It uses DMA for efficient and non-blocking data transmission.
 */
static inline HAL_StatusTypeDef LidarUarTx(uint8_t *address, uint8_t *p_data, uint16_t size) {
	HAL_StatusTypeDef status;
	status = HAL_UART_Transmit_DMA(&huart1, address, 2);
	return status;
}

/**
 * @brief Receive data through UART for Lidar communication.
 *
 * This function uses Direct Memory Access (DMA) to receive data through UART.
 *
 * @param[out] p_data Pointer to the buffer where received data will be stored.
 * @param[in] size Size of the data to be received.
 *
 * @return
 *   - #HAL_OK: Reception was successful.
 *   - #HAL_ERROR: An error occurred during reception.
 *
 * @note This function is specific to the UART communication for Lidar devices.
 * It uses DMA for efficient and non-blocking data reception.
 */
static inline HAL_StatusTypeDef LidarUartRx(uint8_t *p_data, uint16_t size) {
	return HAL_UART_Receive_DMA(&huart1, p_data, size);
}

/**
 * @brief Initialize the Lidar device.
 *
 * This function initializes the Lidar device, configuring UART communication,
 * creating a semaphore for UART read synchronization, enabling the Lidar device and motor,
 * and setting the Lidar speed to 0.
 *
 * @return
 *   - #success: Lidar initialization was successful.
 *   - #memoryfail: Failed to create a semaphore for UART read synchronization.
 *
 * @note This function sets up the necessary configurations for the Lidar device to operate.
 * It also creates a semaphore for synchronization during UART reads and enables the Lidar device and motor.
 * The Lidar speed is set to 0 during initialization.
 */
returncode_t LidarInit(void) {
	returncode_t ret = success;
	lidar.uart.tx = LidarUarTx;
	lidar.uart.rx = LidarUartRx;
	
	sem_uart_read = xSemaphoreCreateBinary();
	if(sem_uart_read == NULL) {
		return memoryfail;
	}
	LIDAR_DEV_ENABLE();
	LIDAR_MOTOR_DISABLE();
	LidarSetSpeed(0);
	//ret = LidarScanStop();
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

/**
 * @brief Get information from the Lidar device.
 *
 * This function sends a command to the Lidar device to retrieve device information,
 * including model number, firmware version, hardware version, and serial number.
 *
 * @param[out] devEUI Pointer to a structure where the Lidar device information will be stored.
 *
 * @return
 *   - #success: Lidar information retrieval was successful.
 *   - #mallocfail: Failed to allocate memory for data reception.
 *   - #transmission_failed: Failed to transmit or receive data.
 *   - #wrongparameter: Received data has an unexpected header or format.
 *
 * @note This function uses UART communication with DMA for efficient data transmission and reception.
 * It checks the header of the received data to ensure it matches the expected format before extracting device information.
 */
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
	status = lidar.uart.rx(version, (DEVEUI_SIZE + HEADER_SIZE));
	if(status == HAL_OK) {
		xSemaphoreTake(sem_uart_read, portMAX_DELAY);	// Wait for DMA Notification
		if(strncmp(version, (uint8_t[]){0xA5, 0x5A}, 2) == 0) {
			//LOG_LIDAR_DEBUG(COLOR_GREEN"header OK");
			memcpy(&(devEUI->ModelNumber), &(version[HEADER_SIZE]), MODEL_NUMBER_SIZE);
			memcpy(&(devEUI->FirmWareVersion), &(version[HEADER_SIZE+1]), FIRMWARE_VERSION_SIZE);
			memcpy(&(devEUI->HardWareVersion), &(version[HEADER_SIZE+3]), HARDWARE_VERSION_SIZE);
			memcpy(&(devEUI->SerialNumber), &(version[HEADER_SIZE+4]), SERIAL_NUMBER_SIZE);
		}
		else {
			LOG_LIDAR_ERROR("Header error 0x%02X %02X", version[0], version[1]);
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

/**
 * @brief Start a Lidar scanning process.
 *
 * This function sends a command to the Lidar device to initiate the scanning process.
 *
 * @return
 *   - #success: Lidar scanning process was successfully initiated.
 *   - #transmission_failed: Failed to transmit the scanning command.
 *
 * @note This function uses UART communication with DMA for efficient data transmission.
 */
returncode_t LidarScanStart(void){
	LOG_LIDAR_ENTER();
	HAL_StatusTypeDef status;

	LIDAR_DEV_ENABLE();
	LIDAR_MOTOR_ENABLE();

	uint8_t command[COMMAND_SIZE] = {(LIDAR_SCAN_START & 0xFF00) >> 8, (LIDAR_SCAN_START & 0x00FF)};
	//uint8_t command[COMMAND_SIZE] = {0x60, 0xA5};
	status = lidar.uart.tx(command, NULL, 0);
	if(status != HAL_OK) {
		LOG_LIDAR_ERROR("transmit error: %d", status);
		return transmission_failed;
	}
	return success;
}

/**
 * @brief Stop the Lidar scanning process.
 *
 * This function sends a command to the Lidar device to stop the scanning process.
 *
 * @return
 *   - #success: Lidar scanning process was successfully stopped.
 *   - #transmission_failed: Failed to transmit the command to stop scanning.
 *
 * @note This function uses UART communication with DMA for efficient data transmission.
 */
returncode_t LidarScanStop(void) {
	//LOG_LIDAR_ENTER();
	LIDAR_MOTOR_DISABLE();
	HAL_StatusTypeDef status;
	uint8_t command[COMMAND_SIZE] = {(LIDAR_SCAN_STOP & 0xFF00) >> 8, (LIDAR_SCAN_STOP & 0x00FF)};
	status = lidar.uart.tx(command, NULL, 0);
	if(status != HAL_OK) {
		LOG_LIDAR_ERROR("transmit error: %d", status);
		return transmission_failed;
	}
	return success;
}

/**
 * @brief Get the health status of the Lidar device.
 *
 * This function sends a command to the Lidar device to retrieve its health status,
 * including the status code and error code.
 *
 * @param[out] healthStatus Pointer to a structure where the Lidar health status will be stored.
 *
 * @return
 *   - #success: Lidar health status retrieval was successful.
 *   - #mallocfail: Failed to allocate memory for data reception.
 *   - #transmission_failed: Failed to transmit or receive data.
 *   - #wrongparameter: Received data has an unexpected header or format.
 *
 * @note This function uses UART communication with DMA for efficient data transmission and reception.
 * It checks the header of the received data to ensure it matches the expected format before extracting health status information.
 */
returncode_t LidarHealthStatus(lidar_healthStatus_t *healthStatus) {
	LOG_LIDAR_ENTER();
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
	status = lidar.uart.rx(buf,  (HEADER_SIZE + HEALTH_STATUS_SIZE));
	if(status == HAL_OK) {
		xSemaphoreTake(sem_uart_read, portMAX_DELAY);	// Wait for DMA Notification
		if(strncmp(buf, (uint8_t[]){0xA5, 0x5A}, 2) == 0) {
			memcpy(&(healthStatus->StatusCode), &(buf[HEADER_SIZE]), STATUS_CODE_SIZE);
			memcpy(&(healthStatus->ErrorCode), &(buf[HEADER_SIZE + 1]), ERROR_CODE_SIZE);
		}
		else {
			LOG_LIDAR_ERROR("Header error 0x%02X %02X", buf[0], buf[1]);
			free(buf);
			return wrongparameter;
		}
	}
	else {
		LOG_LIDAR_ERROR("reception error");
		free(buf);
		return transmission_failed;
	}
	LOG_LIDAR_DEBUG("healthStatus: -status = %s, -error: 0x[%02X %02X]", 
					(healthStatus->StatusCode == 0) ? "running normaly" : (healthStatus->StatusCode == 1) ? "running warning" : "running incorrectly",
					healthStatus->ErrorCode[0], healthStatus->ErrorCode[1]);
	free(buf);
	return success;
}

returncode_t lidarRestart(void) {
	LOG_LIDAR_ENTER();
	HAL_StatusTypeDef status;
	uint8_t command[COMMAND_SIZE] = {(LIDAR_RESTART & 0xFF00) >> 8, (LIDAR_RESTART & 0x00FF)};
	status = lidar.uart.tx(command, NULL, 0);
	if(status != HAL_OK) {
		LOG_LIDAR_ERROR("transmit error: %d", status);
		return transmission_failed;
	}
	return success;
}

bool checkCS(lidar_scan_t *lidarData) {
	uint16_t calculatedCS = ((lidarData->PH[0] << 8) + lidarData->PH[1])^((lidarData->FSA[0] << 8) + lidarData->FSA[1]);
	for (int i = 0; i < lidarData->LSN; i += 2) {
		calculatedCS ^= ((lidarData->SI[i] << 8) + lidarData->SI[i+1]);
	}
	calculatedCS ^= ((lidarData->CT << 8) + lidarData->LSN)^((lidarData->LSA[0] << 8) + lidarData->LSA[1]);
	if(((lidarData->CS[0] << 8) + lidarData->CS[1]) == calculatedCS) {
		return true;
	}
	else {
		return false;
	}
}

returncode_t convertSample(lidar_scan_t *lidarData) { //TODO
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
	lidar.position = malloc(sampleSize * sizeof(position_t));
	if (lidar.position == NULL) {
		return mallocfail;
	}	
	memcpy(&(lidar.position), position, sampleSize);
	free(lidar.position); // TODO
	return success;
}
/*
returncode_t calculateOptimalDirection(void) {
	double sumDistance = 0.0;
    double avgAngle = 0.0;

    // Calculez la somme des distances et des angles de tous les obstacles
    for (int i = 0; i < numObstacles; i++) {
        sumDistance += obstaclePolar[i].distance;
        avgAngle += obstaclePolar[i].angle;
    }

    // Calculez la distance moyenne en divisant la somme par le nombre d'obstacles
    double avgDistance = sumDistance / numObstacles;

    // Calculez l'angle moyen en divisant la somme par le nombre d'obstacles
    avgAngle /= numObstacles;

    PolarCoordinate optimalPosition;
    optimalPosition.distance = avgDistance;
    optimalPosition.angle = avgAngle;

    return optimalPosition;
}*/
/*
int16_t *whereIsNearestRobot(void) {
	int16_t robotPosition[2] = {0, 0}; // {distance, angle}
	//TODO convert distance
	//TODO for boucle

	/*check if 5 samples form an arc of a circle*/
/*	double expectedDistance = calculateExpectedDistance(convertedDistance[n], offsetAngle);
	if((convertedDistance[n+1] < expectedDistance + 0.3) && (convertedDistance[n+1] > expectedDistance - 0.3) && (convertedDistance[n-1] < expectedDistance + 0.3) && (convertedDistance[n-1] > expectedDistance - 0.3)) {
		expectedDistance = calculateExpectedDistance(convertedDistance[n], 2*offsetAngle);
		if((convertedDistance[n+2] < expectedDistance + 0.3) && (convertedDistance[n+2] > expectedDistance - 0.3) && (convertedDistance[n-2] < expectedDistance + 0.3) && (convertedDistance[n-2] > expectedDistance - 0.3)) {
			if(!robotPosition[0]) { // first robot detected
				robotPosition[0] = convertedDistance[n];
				robotPosition[1] = n;
			}
			else if(robotPosition[0] > convertedDistance[n]) { // a robot is closer than previous
				robotPosition[0] = convertedDistance[n];
				robotPosition[1] = n;
			}
		}
	}
	// end for
	return robotPosition;
}*/
/*
double calculateExpectedDistance(double distanceInitiale, double angleDecalage) {
    double angleEnRadians = angleDecalage * M_PI / 180.0; // Conversion de l'angle en radians, car les fonctions trigonométriques en C utilisent des radians
    double nouvelleDistance = sqrt(pow(distanceInitiale, 2) + pow(RAYON, 2) - 
								2 * distanceInitiale * RAYON * cos(angleEnRadians));// Calcul de la distance à laquelle devrait etre le prochain point pour etre un robot
																					// en utilisant le théorème des cosinus
	return nouvelleDistance;
}*/

returncode_t getLidarScanData(lidar_scan_t *lidarScanData) {
	HAL_StatusTypeDef status = 0;
	int size = 0;
	uint8_t lidarScanRawData[256];
	status = lidar.uart.rx(lidarScanRawData, 256);
	if(status == HAL_OK) {
		if (xSemaphoreTake(sem_uart_read, 100) == pdTRUE) {	// Wait for DMA Notification
			//LidarScanStop();
			//printf(COLOR_MAGENTA"*** %s\n", CONVERT_TO_STRING(lidarScanRawData, 256));
			
			int j = 0;
			for(j = 0; j < 256; j++) {
				if(strncmp(&(lidarScanRawData[j]), (uint8_t[]){0xA5, 0x5A}, 2) == 0) {
					printf("main header detected @ %d\n", j);
					//LOG_LIDAR_DEBUG("main header detected @ %d", j);
					memcpy(lidarScanData->PH, &(lidarScanRawData[HEADER_SIZE]), PH_SIZE);
					memcpy(&(lidarScanData->CT), &(lidarScanRawData[HEADER_SIZE + 2]), CT_SIZE);
					memcpy(&(lidarScanData->LSN), &(lidarScanRawData[HEADER_SIZE + 3]), LSN_SIZE);
					memcpy(lidarScanData->FSA, &(lidarScanRawData[HEADER_SIZE + 4]), FSA_SIZE);
					memcpy(lidarScanData->LSA, &(lidarScanRawData[HEADER_SIZE + 6]), LSA_SIZE);
					memcpy(lidarScanData->CS, &(lidarScanRawData[HEADER_SIZE + 8]), CS_SIZE);
					//lidarScanData->SI = malloc((lidarScanData->LSN) * sizeof(uint8_t));
					if (lidarScanData->SI == NULL) {
						return mallocfail;
					} else {
						//memcpy(lidarScanData->SI, &(lidarScanRawData[HEADER_SIZE + j + 10]), lidarScanData->LSN);
						for(int k = 0; k < lidarScanData->LSN; k++) {
							lidarScanData->SI[k] = lidarScanRawData[HEADER_SIZE + k + 10];
						}
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
					//lidarScanData->SI = malloc((lidarScanData->LSN) * sizeof(uint8_t));
					if (lidarScanData->SI == NULL) {
						return mallocfail;
					} else {
						//memcpy(lidarScanData->SI, &(lidarScanRawData[j + 10]), lidarScanData->LSN);
						for(int k = 0; k < lidarScanData->LSN; k++) {
							lidarScanData->SI[k] = lidarScanRawData[k + 10];
						}
					}
					break;
				}
			}
			if(j > 255) {
				LOG_LIDAR_WARN("no header detected");
				return wrongparameter;
			}
		}
		else {
			LOG_LIDAR_WARN("semaphore timeout");
			return transmission_no_response;
		}
	}
	else {
		LOG_LIDAR_ERROR("uart Rx error: %d", status);
		return transmission_no_response;
	}
	return success;
}