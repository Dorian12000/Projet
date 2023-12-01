/**
 *     _______    _____     __     ______    _______        ____
 *    |   ____|  |     \   |  |   / _____)  |   ____|      /    \
 *    |  |__     |  |\  \  |  |  ( (____    |  |__        /  /\  \
 *    |   __|    |  | \  \ |  |   \____ \   |   __|      /  ____  \
 *    |  |____   |  |  \  \|  |   _____) )  |  |____    /  /    \  \
 *    |_______|  |__|   \_____|  (______/   |_______|  /__/      \__\
 *
 * @file    auxiliaryTask.c
 * @Author  Dorian Dalbin, Clement Du, Laksan Thirukumaran, Gael Gourdin
 * @Created	2023-10-28
 * @brief
 *
 **/

#include <auxiliaryTask.h>
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



#define BATTERY_VOLTAGE 7.2
#define RESOLUTION_ADC 4096

TaskHandle_t h_task_batteryLevel = NULL;

float readBatteryLevel() {
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
	uint16_t adcValue = HAL_ADC_GetValue(&hadc1);
	HAL_ADC_Stop(&hadc1);

	float batteryLevel = (float)adcValue / RESOLUTION_ADC * BATTERY_VOLTAGE;

	return batteryLevel;
}



// Tâche pour surveiller l'état de la batterie

void createBatteryLevelTask(void *pvParameters) {
	if 	(xTaskCreate(batteryTask, "BatteryTask", TASK_BATTERY_STACK_DEPTH, NULL,
			TASK_BATTERY_PRIORITY, h_task_batteryLevel)!= pdPASS){
		LOG_BATTERY_ERROR("Error creating task lidar\r\n");

	}
	else
		LOG_LIDAR_DEBUG(COLOR_GREEN"BatteryLevel task create");
}



