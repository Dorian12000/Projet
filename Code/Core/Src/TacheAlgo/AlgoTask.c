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

<<<<<<< HEAD
#include "AlgoTask.h"


TaskFunction_t AlgoTask(void) {
	mainState_t robotState;
=======

// TODO faire les fonctions callbacks du bumper
// Envoyer une notifications pour savoir si c'est une souris ou un chat
#define TASK_ALGO_STACK_DEPTH 512
#define TASK_ALGO_PRIORITY 50
#include "AlgoTask.h"
#include "../Inc/AlgoTask.h"
typedef struct {
	algoState_t state;
	algoState_t lastState;
	TimerHandle_t timer;
}algoHandle_t;
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
>>>>>>> 8c4ad9ef68b97d3093480e5f02b79854a4f53d2a
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


		//TODO Diriger le robot
	}
	else {

		printf("The state doesn't match");

<<<<<<< HEAD
	}


=======

	//TODO calculer la distance la plus éloigné



	//TODO Renvoyer position pour s'échapper du robot

}
void createAlgoTask(void) {
	if (xTaskCreate(algoTask, "Algo", TASK_ALGO_STACK_DEPTH, NULL, TASK_ALGO_PRIORITY, &h_task_algo) != pdPASS) {
		LOG_LIDAR_ERROR("Error creating task algo\r\n");
	}
	else {
		LOG_LIDAR_DEBUG(COLOR_GREEN"Algo task create");
	}
>>>>>>> 8c4ad9ef68b97d3093480e5f02b79854a4f53d2a
}


