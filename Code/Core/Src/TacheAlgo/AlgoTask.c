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
 * @Created	2023-12-28
 * @brief
 *
 **/

#include "AlgoTask.h"
#include "AlgoTask.h"
#include "../Inc/AlgoTask.h"

#define TASK_ALGO_STACK_DEPTH 512
#define TASK_ALGO_PRIORITY 50

TaskHandle h_task_algo = NULL;
static algoHandle_t algoHandle;

void ConfigAlgoState(algoState_t state){
	algoHandle_t state = state;
}
typedef enum {
	ATTENTE,
	SOURIS,
	CHAT,
	STOP
}algoState_t;

TaskFunction_t mouse_cat_AlgoTask(void) {

	returncode_t status;
	lidar_devEUI_t devEUI;
	lidar_scan_t lidarScanData;
	convertSample(&lidarScanData);
	robotState = getMainState();

	if (robotState = MAIN_CAT)
	{


	}
	if (robotState = MAIN_MOUSE)
	{
		//TODO Recuperer les distances


		//TODO Choisir la distance la plus longue


		//TODO calculer la distance la plus éloigné



		//TODO Renvoyer position pour s'échapper du robot


		//TODO Diriger le robot
	}
	else {

		printf("The state doesn't match");

	}


}
void createAlgoTask(void) {
	if (xTaskCreate(algoTask, "Algo", TASK_ALGO_STACK_DEPTH, NULL, TASK_ALGO_PRIORITY, &h_task_algo) != pdPASS) {
		LOG_LIDAR_ERROR("Error creating task algo\r\n");
	}
	else {
		LOG_LIDAR_DEBUG(COLOR_GREEN"Algo task create");
	}
}


