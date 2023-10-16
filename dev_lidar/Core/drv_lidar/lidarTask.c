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

TaskHandle_t h_task_lidar = NULL;

void lidarTask(void) {
	while(LidarInit() != success) {
		vTaskDelay(10);
	}
	while(1) {
		LOG_LIDAR_DEBUG("**************************");
		vTaskDelay(2000);
	}
}

void createLidarTask(void) {
	if (xTaskCreate(lidarTask, "Lidar", TASK_LIDAR_STACK_DEPTH, NULL, TASK_LIDAR_PRIORITY, &h_task_lidar) != pdPASS) {
		LOG_LIDAR_ERROR("Error creating task lidar\r\n");
		Error_Handler();
	}
	else {
		LOG_LIDAR_INFO(COLOR_GREEN"Lidar task create");
	}
}