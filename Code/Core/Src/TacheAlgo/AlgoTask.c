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


TaskFunction_t AlgoTask(void) {
	mainState_t robotState;
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

	}


}

