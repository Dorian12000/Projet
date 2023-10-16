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
#include "log/logger.h"
#include "log/types.h"

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

bool isLidarScanning(void) {
	return (lidarHandle.state == LIDAR_SCANNING);
}

void lidarTask(void) {
	lidar_devEUI_t devEUI;
	lidar_scan_t lidarSc;

	lidarHandle.state = LIDAR_INIT;
	lidarHandle.lastState = lidarHandle.state;
	lidarHandle.timer = xTimerCreate("Lidar Timer", 100, pdTRUE, NULL, NULL);

	while(1) {
		switch (lidarHandle.state) {
			case LIDAR_INIT: {
				if(xTimerIsTimerActive(lidarHandle.timer) == pdFALSE) {
					xTimerStart(lidarHandle.timer, 0);
					LidarInit();
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
				}
				if(LidarScanStart() == success) {
					lidarHandle.state = LIDAR_PROCESS;
				}
				break;
			}
			case LIDAR_PROCESS: {
				lidarDataProcess();
				lidarHandle.state = LIDAR_SCANNING;
				break;
			}
			case LIDAR_STANDBY: {
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
		
		vTaskDelay(1);
	}
}

void createLidarTask(void) {
	if (xTaskCreate(lidarTask, "Lidar", TASK_LIDAR_STACK_DEPTH, NULL, TASK_LIDAR_PRIORITY, &h_task_lidar) != pdPASS) {
		LOG_LIDAR_ERROR("Error creating task lidar\r\n");
	}
	else {
		LOG_LIDAR_INFO(COLOR_GREEN"Lidar task create");
	}
}