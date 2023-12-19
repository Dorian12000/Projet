/**
 *     _______    _____     __     ______    _______        ____
 *    |   ____|  |     \   |  |   / _____)  |   ____|      /    \
 *    |  |__     |  |\  \  |  |  ( (____    |  |__        /  /\  \
 *    |   __|    |  | \  \ |  |   \____ \   |   __|      /  ____  \
 *    |  |____   |  |  \  \|  |   _____) )  |  |____    /  /    \  \
 *    |_______|  |__|   \_____|  (______/   |_______|  /__/      \__\
 *
 * @file    logger.h
 * @Author  Dorian Dalbin
 * @Created	2023-08-03
 * @brief
 *
 **/
#ifndef INC_LOGGER_H_
#define INC_LOGGER_H_

#include <stdlib.h>
#include <stdio.h>
#include "colors.h"

#include "main.h"

#define LOG_ASSERT(message, x...) printf("ASSERT: %s:%s - " message, __FILE__, __func__, ## x)

#define ASSERT(x) if((x)==0) { LOG_ASSERT("line %u [%s]", __LINE__, #x); }

#define LOG_TASK_ENTER() printf(COLOR_WHITE "TASK\t ENTER\n")
#define LOG_TASK_EXIT() printf(COLOR_WHITE "TASK\t EXIT\n")

#define LOG_MAIN_ENTER() 				printf(COLOR_WHITE "MAIN\t ENTER: %s:\n\r", __func__)
#define LOG_MAIN_EXIT() 				printf(COLOR_WHITE "MAIN\t EXIT: %s:\n\r", __func__)
#define LOG_MAIN_DEBUG(message, x...) 	printf(COLOR_WHITE "MAIN\t INFO: %s : %s - " message "\n\r", __func__, ## x)
#define LOG_MAIN_INFO(message, x...) 	printf(COLOR_CYAN"MAIN\t INFO: %s : %s - " message "\n\r", __func__, ## x)
#define LOG_MAIN_ERROR(message, x...) 	printf(COLOR_RED"MAIN\t ERROR: %s : %s - " message "\n\r", __func__, ## x)
#define LOG_MAIN_WARN(message, x...) 	printf(COLOR_YELLOW"MAIN \t WARN: %s : %s - " message "\n\r", __func__, ## x)

#define LOG_LIDAR_ENTER() 				printf(COLOR_WHITE "LIDAR\t ENTER: %s:\n\r", __func__)
#define LOG_LIDAR_EXIT() 				printf(COLOR_WHITE "LIDAR\t EXIT: %s:\n\r", __func__)
#define LOG_LIDAR_DEBUG(message, x...) 	printf(COLOR_WHITE "LIDAR\t INFO: %s : %s - " message "\n\r", __func__, ## x)
#define LOG_LIDAR_INFO(message, x...) 	printf(COLOR_CYAN"LIDAR\t INFO: %s : %s - " message "\n\r", __func__, ## x)
#define LOG_LIDAR_ERROR(message, x...) 	printf(COLOR_RED"LIDAR\t ERROR: %s : %s - " message "\n\r", __func__, ## x)
#define LOG_LIDAR_WARN(message, x...) 	printf(COLOR_YELLOW"LIDAR\t WARN: %s : %s - " message "\n\r", __func__, ## x)

#define LOG_TIMER_ENTER() 				printf(COLOR_WHITE "TIMER\t ENTER: %s:\n\r", __func__)
#define LOG_TIMER_EXIT() 				printf(COLOR_WHITE "TIMER\t EXIT: %s:\n\r", __func__)
#define LOG_TIMER_DEBUG(message, x...) 	printf(COLOR_WHITE "TIMER\t INFO: %s : %s - " message "\n\r", __func__, ## x)
#define LOG_TIMER_INFO(message, x...) 	printf(COLOR_CYAN"TIMER\t INFO: %s : %s - " message "\n\r", __func__, ## x)
#define LOG_TIMER_ERROR(message, x...) 	printf(COLOR_RED"TIMER\t ERROR: %s : %s - " message "\n\r", __func__, ## x)
#define LOG_TIMER_WARN(message, x...) 	printf(COLOR_YELLOW"TIMER\t WARN: %s : %s - " message "\n\r", __func__, ## x)

#define LOG_LED_ENTER() 				printf(COLOR_WHITE "LED\t ENTER: %s:\n\r", __func__)
#define LOG_LED_EXIT() 					printf(COLOR_WHITE "LED\t EXIT: %s:\n\r", __func__)
#define LOG_LED_DEBUG(message, x...) 	printf(COLOR_WHITE "LED\t INFO: %s : %s - " message "\n\r", __func__, ## x)
#define LOG_LED_INFO(message, x...) 	printf(COLOR_CYAN"LED\t INFO: %s : %s - " message "\n\r", __func__, ## x)
#define LOG_LED_ERROR(message, x...) 	printf(COLOR_RED"LED\t ERROR: %s : %s - " message "\n\r", __func__, ## x)
#define LOG_LED_WARN(message, x...) 	printf(COLOR_YELLOW"LED\t WARN: %s : %s - " message "\n\r", __func__, ## x)

#define LOG_CONSOLE_ENTER() 				printf(COLOR_WHITE "CONSOLE\t ENTER: %s:\n\r", __func__)
#define LOG_CONSOLE_EXIT() 					printf(COLOR_WHITE "CONSOLE\t EXIT: %s:\n\r", __func__)
#define LOG_CONSOLE_DEBUG(message, x...) 	printf(COLOR_WHITE "CONSOLE\t INFO: %s : %s - " message "\n\r", __func__, ## x)
#define LOG_CONSOLE_INFO(message, x...) 	printf(COLOR_CYAN"CONSOLE\t INFO: %s : %s - " message "\n\r", __func__, ## x)
#define LOG_CONSOLE_ERROR(message, x...) 	printf(COLOR_RED"CONSOLE\t ERROR: %s : %s - " message "\n\r", __func__, ## x)
#define LOG_CONSOLE_WARN(message, x...) 	printf(COLOR_YELLOW"CONSOLE\t WARN: %s : %s - " message "\n\r", __func__, ## x)

#define LOG_RTOS_ENTER() 				printf(COLOR_WHITE "RTOS\t ENTER: %s:\n\r", __func__)
#define LOG_RTOS_EXIT() 				printf(COLOR_WHITE "RTOS\t EXIT: %s:\n\r", __func__)
#define LOG_RTOS_DEBUG(message, x...) 	printf(COLOR_WHITE "RTOS\t INFO: %s : %s - " message "\n\r", __func__, ## x)
#define LOG_RTOS_INFO(message, x...) 	printf(COLOR_CYAN"RTOS\t INFO: %s : %s - " message "\n\r", __func__, ## x)
#define LOG_RTOS_ERROR(message, x...) 	printf(COLOR_RED"RTOS\t ERROR: %s : %s - " message "\n\r", __func__, ## x)
#define LOG_RTOS_WARN(message, x...) 	printf(COLOR_YELLOW"RTOS\t WARN: %s : %s - " message "\n\r", __func__, ## x)

#endif /* INC_LOGGER_H_ */
