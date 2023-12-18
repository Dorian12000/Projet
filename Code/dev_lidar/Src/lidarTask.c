/**
 *     _______    _____     __     ______    _______        ____
 *    |   ____|  |     \   |  |   / _____)  |   ____|      /    \
 *    |  |__     |  |\  \  |  |  ( (____    |  |__        /  /\  \
 *    |   __|    |  | \  \ |  |   \____ \   |   __|      /  ____  \
 *    |  |____   |  |  \  \|  |   _____) )  |  |____    /  /    \  \
 *    |_______|  |__|   \_____|  (______/   |_______|  /__/      \__\
 *
 * @file    lidarTask.c
 * @Author  Dorian Dalbin, Clement Du, Laksan Thirukumaran, Gael Gourdin
 * @Created	2023-10-28
 * @brief
 *
 **/

#include "main.h"
#include "cmsis_os.h"
#include "drv_lidar.h"
#include "logger.h"
#include "types.h"

#include "lidarTask.h"

#define TASK_LIDAR_STACK_DEPTH 512
#define TASK_LIDAR_PRIORITY 1

typedef struct {
	lidarState_t state;
	lidarState_t lastState;
	TimerHandle_t timer;
}lidarHandle_t;

TaskHandle_t h_task_lidar = NULL;
static lidarHandle_t lidarHandle;

/**
 * @brief Check if the Lidar is currently in scanning mode.
 *
 * This function returns a boolean indicating whether the Lidar is currently in the scanning state.
 *
 * @return
 *   - \c true: Lidar is in scanning mode.
 *   - \c false: Lidar is not in scanning mode.
 */
uint8_t isLidarScanning(void) {
	return (lidarHandle.state == LIDAR_SCANNING);
}

void setLidarState(lidarState_t state) {
	lidarHandle.state = state;
}

/**
 * @brief Lidar task function.
 *
 * This function implements the main functionality of the Lidar task.
 * It handles the initialization, scanning, data processing, and standby states of the Lidar device.
 *
 * The function uses a state machine to manage the different states of the Lidar task.
 * States:
 * - LIDAR_INIT: Initializes the Lidar device and transitions to LIDAR_SCANNING on success.
 * - LIDAR_SCANNING: Starts Lidar scanning and performs data processing. Transitions to LIDAR_PROCESS.
 * - LIDAR_PROCESS: Placeholder state for future processing if needed.
 * - LIDAR_STANDBY: Placeholder state for standby mode if needed.
 *
 * @note The task runs indefinitely in a loop, continuously monitoring and managing the Lidar device state.
 *
 * @return None.
 */
TaskFunction_t lidarTask(void) {
	returncode_t status;
	lidar_devEUI_t devEUI;
	lidar_scan_t lidarScanData;
	lidar_healthStatus_t healthStatus;


	lidarHandle.state = LIDAR_INIT;
	lidarHandle.lastState = lidarHandle.state;
	lidarHandle.timer = xTimerCreate("Lidar Timer", 100, pdTRUE, NULL, NULL);

	while(1) {
		switch (lidarHandle.state) {
			
			case LIDAR_INIT: {
				if(xTimerIsTimerActive(lidarHandle.timer) == pdFALSE) {
					xTimerStart(lidarHandle.timer, 0);
					LidarInit();
					lidarRestart();
				}
				if(LidarGetInformation(&devEUI) == success) {
					LOG_LIDAR_INFO(COLOR_GREEN"init lidar successful");
					//LOG_LIDAR_INFO("model: %02X, firmware: %04X, hardware: %02X, serial: %d", devEUI.ModelNumber, devEUI.FirmWareVersion, devEUI.HardWareVersion, devEUI.SerialNumber);
					lidarHandle.state = LIDAR_SCANNING;
				}
				else {
					LOG_LIDAR_ERROR("lidar bad information");
				}
				break;
			}

			case LIDAR_SCANNING: {
				if(xTimerIsTimerActive(lidarHandle.timer) == pdFALSE) {
					xTimerStart(lidarHandle.timer, 0);
					while(LidarScanStart() != success) {
						LOG_LIDAR_ERROR("cannot start scan");
					}
					LOG_LIDAR_INFO("scan started");
				}
				status = getLidarScanData(&lidarScanData);
				if(status != success) {
					lidarHandle.state = LIDAR_ERROR;
					break;
				}
				//if(checkCS(&lidarScanData)){
					status = convertSample(&lidarScanData);
					if(status != success) {
						LOG_LIDAR_WARN("convert sample failed: %d", status);
					}
				//}
				/*
				if(robotState() == CAT) {
					findMousePosition();
				}
				else {
					calculateOptimalDirection();
				}
				*/
				//osDelay(1000);
			}
			case LIDAR_PROCESS: {
				
				//lidarHandle.state = LIDAR_SCANNING;
				break;
			}
			case LIDAR_STANDBY: {
				break;
			}
			case LIDAR_ERROR : {
				while(LidarScanStop() != success) {
					osDelay(1);
				}
				status = LidarHealthStatus(&healthStatus);
				if(healthStatus.StatusCode == 2) {
					lidarRestart();
					lidarHandle.state = LIDAR_INIT;
				}
				else {
					lidarHandle.state = LIDAR_SCANNING;
				}
				break;
			}
			default:
				lidarHandle.state = LIDAR_INIT;
				break;
		}
		if(lidarHandle.lastState != lidarHandle.state) {
			xTimerStop(lidarHandle.timer, 0);
			lidarHandle.lastState = lidarHandle.state;
		}
		
		//vTaskDelay(1);
	}
}

/**
 * @brief Creates a task for the Lidar functionality.
 *
 * This function creates a FreeRTOS task named "Lidar" using xTaskCreate.
 * The task is created with the lidarTask function as its entry point.
 *
 * @note The task is created with specified stack depth and priority.
 * If the task creation fails, an error message is logged using LOG_LIDAR_ERROR.
 * If successful, a debug message is logged using LOG_LIDAR_DEBUG.
 *
 * @see lidarTask
 *
 * @return None.
 */
void createLidarTask(void) {
	if (xTaskCreate(lidarTask, "Lidar", TASK_LIDAR_STACK_DEPTH, NULL, TASK_LIDAR_PRIORITY, &h_task_lidar) != pdPASS) {
		LOG_LIDAR_ERROR("Error creating task lidar\r\n");
	}
	else {
		LOG_LIDAR_DEBUG(COLOR_GREEN"Lidar task create");
	}
}
