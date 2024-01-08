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

#include "../Inc/logger.h"
#include "../Inc/types.h"

#include "../Inc/lidarTask.h"
#include "../Inc/drv_lidar.h"

#define RAYON 0.0637 //rayon du lidar en m

static lidar_t lidar;
static SemaphoreHandle_t sem_uart_read = NULL;
static position_t robotPosition = {0, 0};

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
	status = HAL_UART_Transmit_DMA(&huart4, address, 2);
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
	return HAL_UART_Receive_DMA(&huart4, p_data, size);
}

/**
 * @brief Clears the nearest robot position.
 *
 * This function resets the angle and distance of the nearest robot position.
 * After calling this function, the robot's angle will be set to 0, and the
 * distance will be set to 0.
 */
static void clearNearestRobotPosition(void) {
	robotPosition.angle = 0;
	robotPosition.distance = 0;
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

	uint8_t version[DEVEUI_SIZE + HEADER_SIZE];

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
			return wrongparameter;
		}
	}
	else {
		LOG_LIDAR_ERROR("received error");
		return transmission_failed;
	}
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
	uint8_t buf[HEADER_SIZE + HEALTH_STATUS_SIZE];

	status = lidar.uart.rx(buf,  (HEADER_SIZE + HEALTH_STATUS_SIZE));
	if(status == HAL_OK) {
		xSemaphoreTake(sem_uart_read, portMAX_DELAY);	// Wait for DMA Notification
		if(strncmp(buf, (uint8_t[]){0xA5, 0x5A}, 2) == 0) {
			memcpy(&(healthStatus->StatusCode), &(buf[HEADER_SIZE]), STATUS_CODE_SIZE);
			memcpy(&(healthStatus->ErrorCode), &(buf[HEADER_SIZE + 1]), ERROR_CODE_SIZE);
		}
		else {
			LOG_LIDAR_ERROR("Header error 0x%02X %02X", buf[0], buf[1]);
			return wrongparameter;
		}
	}
	else {
		LOG_LIDAR_ERROR("reception error");
		return transmission_failed;
	}
	LOG_LIDAR_DEBUG("healthStatus: -status = %s, -error: 0x[%02X %02X]", 
					(healthStatus->StatusCode == 0) ? "running normaly" : (healthStatus->StatusCode == 1) ? "running warning" : "running incorrectly",
					healthStatus->ErrorCode[0], healthStatus->ErrorCode[1]);
	return success;
}

/**
 * @brief Restarts the LIDAR device.
 *
 * This function sends a restart command to the LIDAR device over UART and
 * checks the transmission status. If the transmission is successful, the
 * function returns success; otherwise, it returns transmission_failed.
 *
 * @return Return code indicating the success or failure of the LIDAR restart.
 *         - success: The LIDAR restart command was successfully transmitted.
 *         - transmission_failed: An error occurred during the transmission.
 */
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

/**
 * @brief Check the checksum (CS) of lidar scan data.
 *
 * This function calculates the checksum based on the provided lidar scan data
 * and compares it with the checksum value in the data. If the calculated
 * checksum matches the one in the data, the function returns true; otherwise,
 * it returns false.
 *
 * @param lidarData Pointer to the lidar scan data structure.
 * @return Returns true if the checksum is valid, false otherwise.
 */
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

/**
 * @brief Converts lidar scan data to distance and angle values.
 *
 * This function takes lidar scan data and converts it into distance and angle
 * values. The calculated values are then stored in the global position array
 * of the lidar structure.
 *
 * @param lidarData Pointer to the lidar scan data structure.
 * @return Returns success if the conversion is successful.
 */
returncode_t convertSample(lidar_scan_t *lidarData) {
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
	return success;
}

/**
 * @brief Determines the position of the nearest robot based on lidar data.
 *
 * This function analyzes lidar data to determine the position of the nearest
 * robot. It checks for patterns indicating the presence of a robot and updates
 * the global robot position accordingly. Additionally, it resets the robot
 * position if no update is received after 20 seconds.
 *
 * @return Pointer to the structure representing the position of the nearest robot.
 */
position_t *whereIsNearestRobot(void) {
	/* reset nearest robot position if no update after 20s */ 
	TimerHandle_t robotTimer = xTimerCreate("Other Robot Timer", pdMS_TO_TICKS(20000), pdFALSE, NULL, clearNearestRobotPosition);
	uint8_t offsetAngle = lidar.position[1].angle - lidar.position[0].angle;
	for(int n = 0; n < strlen(lidar.position); n++)
	{
		/*check if 5 samples form an arc of a circle*/
		double expectedDistance = calculateExpectedDistance(lidar.position[n].distance, offsetAngle);
		if((lidar.position[n+1].distance < expectedDistance + 0.3) && (lidar.position[n+1].distance > expectedDistance - 0.3) && (lidar.position[n-1].distance < expectedDistance + 0.3) && (lidar.position[n-1].distance > expectedDistance - 0.3)) {
			expectedDistance = calculateExpectedDistance(lidar.position[n].distance, 2*offsetAngle);
			if((lidar.position[n+2].distance < expectedDistance + 0.3) && (lidar.position[n+2].distance > expectedDistance - 0.3) && (lidar.position[n-2].distance < expectedDistance + 0.3) && (lidar.position[n-2].distance > expectedDistance - 0.3)) {
				if(!robotPosition.distance) { // first robot detected
					xTimerStart(robotTimer, 0);
					robotPosition.distance = lidar.position[n].distance;
					robotPosition.angle = n;
				}
				else if(robotPosition.distance > lidar.position[n].distance) { // a robot is closer than previous
					xTimerReset(robotTimer, 0);
					robotPosition.distance = lidar.position[n].distance;
					robotPosition.angle = n;
				}
			}
		}
	}
	return &robotPosition;
}

/**
 * @brief Calculates the expected distance for a point with angular displacement.
 *
 * This function calculates the expected distance for a point given its initial
 * distance and an angular displacement. The calculation is based on the
 * law of cosines, and the result represents the distance to the point after
 * applying the angular displacement.
 *
 * @param distanceInitiale Initial distance to the point.
 * @param angleDecalage Angular displacement in degrees.
 * @return The calculated distance after applying the angular displacement.
 */
double calculateExpectedDistance(double distanceInitiale, double angleDecalage) {
    double angleEnRadians = angleDecalage * 3.14 / 180.0; // Conversion de l'angle en radians, car les fonctions trigonométriques en C utilisent des radians
    double nouvelleDistance = sqrt(pow(distanceInitiale, 2) + pow(RAYON, 2) - 
								2 * distanceInitiale * RAYON * cos(angleEnRadians));// Calcul de la distance à laquelle devrait etre le prochain point pour etre un robot
																					// en utilisant le théorème des cosinus
	return nouvelleDistance;
}

/**
 * @brief Retrieves lidar scan data from the LIDAR device.
 *
 * This function reads raw lidar scan data from the LIDAR device through UART
 * and processes it to extract relevant information, such as headers, angles,
 * distances, and checksum. The extracted data is then stored in the provided
 * lidar scan data structure.
 *
 * @param lidarScanData Pointer to the lidar scan data structure where the data will be stored.
 * @return Returns success if the operation is successful; otherwise, returns an error code.
 */
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
