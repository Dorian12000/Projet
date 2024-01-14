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

#include "mainTask.h"

#include "main.h"
#include "cmsis_os.h"
#include "logger.h"
#include "types.h"
#include <limits.h>
#include "lidarTask.h"
#include "task_motor.h"

#define TASK_MAIN_STACK_DEPTH 512
#define TASK_MAIN_PRIORITY 99

typedef struct {
	mainState_t state;
	mainState_t lastState;
	TimerHandle_t timer;
}mainHandle_t;

TaskHandle_t h_task_main;
mainHandle_t mainHandle;

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin) {
	LOG_MAIN_ENTER();
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	if(GPIO_Pin == BUMPER_F_Pin) {
		xTaskNotifyFromISR(h_task_main, BUMPER_F_NOTIFY, eSetBits, &xHigherPriorityTaskWoken);
	} else if(GPIO_Pin == BUMPER_B_Pin) {
		xTaskNotifyFromISR(h_task_main, BUMPER_B_NOTIFY, eSetBits, &xHigherPriorityTaskWoken );
	} else if(GPIO_Pin == BUMPER_R_Pin) {
		xTaskNotifyFromISR(h_task_main, BUMPER_R_NOTIFY, eSetBits, &xHigherPriorityTaskWoken );
	} else if(GPIO_Pin == BUMPER_L_Pin) {
		xTaskNotifyFromISR(h_task_main, BUMPER_L_NOTIFY, eSetBits, &xHigherPriorityTaskWoken );
	} else if(GPIO_Pin == BORDER_FRONT_Pin) {
		HAL_GPIO_TogglePin(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin);
		xTaskNotifyFromISR(getPositionMotorTaskHandle(), BORDER_F_NOTIFY, eSetBits, &xHigherPriorityTaskWoken );
	} else if(GPIO_Pin == BORDER_BACK_Pin) {
		HAL_GPIO_TogglePin(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin);
		xTaskNotifyFromISR(getPositionMotorTaskHandle(), BORDER_B_NOTIFY, eSetBits, &xHigherPriorityTaskWoken );
	}
	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}

/**
 * @brief Gets the current state of the main system.
 *
 * This function retrieves and returns the current state of the main system.
 *
 * @return The current state of the main system.
 */
mainState_t getMainState(void) {
	return mainHandle.state;
}

/**
 * @brief Toggles the main system state between CAT and MOUSE.
 *
 * This function toggles the main system state between CAT and MOUSE. If the
 * current state is CAT, it switches to MOUSE, and vice versa.
 */
void setMainState(void) {
	if(mainHandle.state == MAIN_CAT) {
		mainHandle.state = MAIN_MOUSE;
	} else if(mainHandle.state == MAIN_MOUSE) {
		mainHandle.state = MAIN_CAT;
	}
}

/**
 * @brief Main task that controls the overall system behavior.
 *
 * This task is responsible for managing the overall behavior of the system.
 * It transitions between different states and performs actions based on the
 * current state. Additionally, it toggles LEDs, processes notifications, and
 * handles timeouts using a timer.
 */
void mainTask(void *param) {
	returncode_t status;
	uint32_t ulNotifiedValue;

	mainHandle.state = MAIN_INIT;
	mainHandle.lastState = mainHandle.state;
	mainHandle.timer = xTimerCreate("Main Timer", 100, pdTRUE, NULL, NULL);

	while(1) {
		switch (mainHandle.state) {
			case MAIN_INIT: {
				if(xTimerIsTimerActive(mainHandle.timer) == pdFALSE) {
					xTimerStart(mainHandle.timer, 0);
					positionMotorTaskCreate();
					asservMotorTaskCreate();
					createLidarTask();
				}
				mainHandle.state = MAIN_MOUSE;
				break;
			}
			case MAIN_CAT: {
				if(xTimerIsTimerActive(mainHandle.timer) == pdFALSE) {
					xTimerStart(mainHandle.timer, 0);
					HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_SET);
				}
				break;
			}
			case MAIN_MOUSE: {
				if(xTimerIsTimerActive(mainHandle.timer) == pdFALSE) {
					xTimerStart(mainHandle.timer, 0);
					HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);
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
		if(xTaskNotifyWait(0, ULONG_MAX, &ulNotifiedValue, 5000) == pdTRUE) {
			setMainState();
//			HAL_GPIO_TogglePin(LED_B_GPIO_Port, LED_B_Pin);
//			HAL_GPIO_TogglePin(LED_R_GPIO_Port, LED_R_Pin);
			//xTaskNotify(lidarHandle, &ulNotifiedValue, eSetBits); //TODO notify side that has beed affected
			taskENTER_CRITICAL();
			vTaskDelay(2000);
			taskEXIT_CRITICAL();
		}
		//vTaskDelay(1);
	}
}

/**
 * @brief Creates and initializes the main task.
 *
 * This function creates and initializes the main task, which is responsible
 * for managing the overall behavior of the system.
 *
 * @return Returns true if the task creation is successful; otherwise, returns false.
 */
uint8_t createMainTask(void) {
	if (xTaskCreate(mainTask, "Main", TASK_MAIN_STACK_DEPTH, NULL, TASK_MAIN_PRIORITY, &h_task_main) != pdPASS) {
		LOG_RTOS_ERROR("Error creating task Main");
		return false;
	}
	else {
		LOG_RTOS_INFO(COLOR_GREEN"Main task create");
		return true;
	}
}
