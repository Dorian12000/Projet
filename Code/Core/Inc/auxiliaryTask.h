/**
 *     _______    _____     __     ______    _______        ____
 *    |   ____|  |     \   |  |   / _____)  |   ____|      /    \
 *    |  |__     |  |\  \  |  |  ( (____    |  |__        /  /\  \
 *    |   __|    |  | \  \ |  |   \____ \   |   __|      /  ____  \
 *    |  |____   |  |  \  \|  |   _____) )  |  |____    /  /    \  \
 *    |_______|  |__|   \_____|  (______/   |_______|  /__/      \__\
 *
 * @file    auxiliaryTask.h
 * @Author  Dorian Dalbin, Clement Du, Laksan Thirukumaran, Gael Gourdin
 * @Created	2023-10-28
 * @brief
 *
 **/

#include "main.h"
#include "tim.h"
#include "gpio.h"
#include "log/types.h"
#include "cmsis_os.h"
/***************************************macros**************************************/

#define TASK_AUXILIARY_STACK_DEPTH 512
#define TASK_AUXILIARY_PRIORITY 1
#define BATTERY_SUPPLY 7.2
#define BATTERY_THRESHOLD 6.4
#define RESOLUTION_ADC 4096
/***************************************functions**************************************/
float getBatteryLevel();
void createAuxiliaryTask();
void auxiliaryTask();
void auxiliaryInit();


