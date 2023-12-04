/**
 *     _______    _____     __     ______    _______        ____
 *    |   ____|  |     \   |  |   / _____)  |   ____|      /    \
 *    |  |__     |  |\  \  |  |  ( (____    |  |__        /  /\  \
 *    |   __|    |  | \  \ |  |   \____ \   |   __|      /  ____  \
 *    |  |____   |  |  \  \|  |   _____) )  |  |____    /  /    \  \
 *    |_______|  |__|   \_____|  (______/   |_______|  /__/      \__\
 *
 * @file    mainTask.c
 * @Author  Dorian Dalbin, Clement Du, Laksan Thirukumaran, Gael Gourdin
 * @Created	2023-10-28
 * @brief
 *
 **/

#include "main.h"
#include "cmsis_os.h"
#include "log/logger.h"
#include "types.h"

#include "mainTask.h"

#define TASK_MAIN_STACK_DEPTH 512
#define TASK_MAIN_PRIORITY 99

typedef struct {
	mainState_t state;
	mainState_t lastState;
	TimerHandle_t timer;
}mainHandle_t;

TaskHandle_t h_task_main = NULL;
static mainHandle_t mainHandle;

mainState_t getmainState(void) {
	return mainHandle.state;
}


void mainTask(void) {
	returncode_t status;

	mainHandle.state = MAIN_INIT;
	mainHandle.lastState = mainHandle.state;
	mainHandle.timer = xTimerCreate("Main Timer", 100, pdTRUE, NULL, NULL);

	while(1) {
		switch (mainHandle.state) {
			
			case MAIN_INIT: {
				if(xTimerIsTimerActive(mainHandle.timer) == pdFALSE) {
					xTimerStart(mainHandle.timer, 0);
					//TODO create task
				}
				mainHandle.state = MAIN_MOUSE;
				break;
			}
			case MAIN_CAT: {
				if(xTimerIsTimerActive(mainHandle.timer) == pdFALSE) {
					xTimerStart(mainHandle.timer, 0);
					
				}
				
			}
			case MAIN_MOUSE: {
				if(xTimerIsTimerActive(mainHandle.timer) == pdFALSE) {
					xTimerStart(mainHandle.timer, 0);
					
				}
				break;
			}
			case MAIN_ERROR : {
				if(xTimerIsTimerActive(mainHandle.timer) == pdFALSE) {
					xTimerStart(mainHandle.timer, 0);
					
				}
				break;
			}
			default:
				mainHandle.state = MAIN_INIT;
				break;
		}
		if(mainHandle.lastState != mainHandle.state) {
			xTimerStop(mainHandle.timer, 0);
			mainHandle.lastState = mainHandle.state;
		}
		
		//vTaskDelay(1);
	}
}

void createMainTask(void) {
	if (xTaskCreate(mainTask, "Main", TASK_MAIN_STACK_DEPTH, NULL, TASK_MAIN_PRIORITY, &h_task_main) != pdPASS) {
		LOG_RTOS_ERROR("Error creating task Main");
	}
	else {
		LOG_RTOS_INFO(COLOR_GREEN"Main task create");
	}
}