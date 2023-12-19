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


// TODO faire les fonctions callbacks du bumper
// Envoyer une notifications pour savoir si c'est une souris ou un chat
#include "AlgoTask.h"
typedef struct {
	AlgoState_t state;
	AlgoState_t lastState;
	TimerHandle_t timer;
}algoHandle_t;

algoHandle_t algoHandle;

void ConfigAlgoState(algoState_t state){
	algoHandle_t state = state
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
	lidar_healthStatus_t healthStatus;
	//TODO  la distance du lidar
	status = getLidarScanData(&lidarScanData);





	//TODO calculer la distance la plus éloigné



	//TODO Renvoyer position pour s'échapper du robot



}

