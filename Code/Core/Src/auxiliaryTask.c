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
#include "adc.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "log/logger.h"
#include "log/types.h"


SemaphoreHandle_t xBatterySemaphore = NULL;
TaskHandle_t h_task_batteryLevel = NULL;

void auxiliaryInit(){
	xBatterySemaphore=xSemaphoreCreateBinary();
}
void auxiliaryTask(){
	float batteryLevel;

	while (1) {
		batteryLevel = getBatteryLevel();
		HAL_UART_Transmit(&huart1, "battery ok\n",13, 1);
		LOG_LED_DEBUG("Battery OK : %f ", batteryLevel);
		if (batteryLevel < BATTERY_THRESHOLD) {
			HAL_UART_Transmit(&huart1, "battery no ok\n",16, 1);
			xSemaphoreTake(xBatterySemaphore, portMAX_DELAY);
			LOG_LED_DEBUG("Low battery");
			xSemaphoreGive(xBatterySemaphore);
		}
		vTaskDelay(1);
	}
}
float getBatteryLevel() {
	HAL_ADC_Start(&hadc1);
	float batteryLevel;
	if(HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK){
		uint16_t adcValue = HAL_ADC_GetValue(&hadc1);
		HAL_ADC_Stop(&hadc1);
		batteryLevel = (float)adcValue / RESOLUTION_ADC * BATTERY_SUPPLY;
	}
	else{
		LOG_LED_ERROR("Get battery error");
	}
	return batteryLevel;
}

void createAuxiliaryTask() {
	if 	(xTaskCreate(auxiliaryTask, "Auxiliary", TASK_AUXILIARY_STACK_DEPTH, NULL,
			TASK_AUXILIARY_PRIORITY, &h_task_batteryLevel)!= pdPASS){
		LOG_LED_ERROR("Error creating task auxiliary");

	}
	else
		LOG_LED_DEBUG(COLOR_GREEN"Auxiliary task create");
}



