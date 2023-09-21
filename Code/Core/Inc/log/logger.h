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

#include "console.h"

#define STRING_LEVEL_ENTER		"ENTER"
#define STRING_LEVEL_EXIT		"EXIT"
#define STRING_LEVEL_DEBUG		"DEBUG"
#define STRING_LEVEL_INFO		"INFO"
#define STRING_LEVEL_WARN		"WARN"
#define STRING_LEVEL_ERROR		"ERROR"
#define STRING_LEVEL_UNK		"UNK"
#define STRING_LEVEL_ASSERT     "ASSERT"

#define LOG_ASSERT(message, x...)       console_logger(LOGTYPE_ASSERT, STRING_LEVEL_ASSERT, COLOR_RED, "ASSERT", __FILE__, __func__, message, ## x)

#define ASSERT(x)    if((x)==0){LOG_ASSERT("line %u [%s]", __LINE__, #x);}


#define LOG_TASK_ENTER()		console_logger(LOGTYPE_ENTER, STRING_LEVEL_ENTER, COLOR_WHITE, "Task", __FILE__, __func__, NULL)
#define LOG_TASK_EXIT()			console_logger(LOGTYPE_EXIT, STRING_LEVEL_EXIT, COLOR_WHITE, "Task", __FILE__, __func__, NULL)

#define LOG_TIMER_ENTER()				console_logger(LOGTYPE_ENTER, STRING_LEVEL_ENTER, COLOR_WHITE, "Timer", __FILE__, __func__, NULL)
#define LOG_TIMER_EXIT()				console_logger(LOGTYPE_EXIT, STRING_LEVEL_EXIT, COLOR_WHITE, "Timer", __FILE__, __func__, NULL)
#define LOG_TIMER_DEBUG(message, x...)	console_logger(LOGTYPE_DEBUG, STRING_LEVEL_DEBUG, COLOR_WHITE, "Timer", __FILE__, __func__, message, ## x)
#define LOG_TIMER_INFO(message, x...) 	console_logger(LOGTYPE_INFO, STRING_LEVEL_INFO, COLOR_CYAN, "Timer", __FILE__, __func__, message, ## x)
#define LOG_TIMER_ERROR(message, x...)	console_logger(LOGTYPE_ERROR, STRING_LEVEL_ERROR, COLOR_RED, "Timer", __FILE__, __func__, message, ## x)
#define LOG_TIMER_WARN(message, x...)	console_logger(LOGTYPE_WARN, STRING_LEVEL_WARN, COLOR_YELLOW, "Timer", __FILE__, __func__, message, ## x)

#define LOG_MAIN_ENTER()				console_logger(LOGTYPE_ENTER, STRING_LEVEL_ENTER, COLOR_WHITE, "Main", __FILE__, __func__, NULL)
#define LOG_MAIN_EXIT()					console_logger(LOGTYPE_EXIT, STRING_LEVEL_EXIT, COLOR_WHITE, "Main", __FILE__, __func__, NULL)
#define LOG_MAIN_DEBUG(message, x...)	console_logger(LOGTYPE_DEBUG, STRING_LEVEL_DEBUG, COLOR_WHITE, "Main", __FILE__, __func__, message, ## x)
#define LOG_MAIN_INFO(message, x...) 	console_logger(LOGTYPE_INFO, STRING_LEVEL_INFO, COLOR_CYAN, "Main", __FILE__, __func__, message, ## x)
#define LOG_MAIN_ERROR(message, x...)	console_logger(LOGTYPE_ERROR, STRING_LEVEL_ERROR, COLOR_RED, "Main", __FILE__, __func__, message, ## x)
#define LOG_MAIN_WARN(message, x...)	console_logger(LOGTYPE_WARN, STRING_LEVEL_WARN, COLOR_YELLOW, "Main", __FILE__, __func__, message, ## x)


#define LOG_LED_ENTER()					console_logger(LOGTYPE_ENTER, STRING_LEVEL_ENTER, COLOR_WHITE, "LED", __FILE__, __func__, NULL)
#define LOG_LED_EXIT()					console_logger(LOGTYPE_EXIT, STRING_LEVEL_EXIT, COLOR_WHITE, "LED", __FILE__, __func__, NULL)
#define LOG_LED_DEBUG(message, x...)	console_logger(LOGTYPE_DEBUG, STRING_LEVEL_DEBUG, COLOR_WHITE, "LED", __FILE__, __func__, message, ## x)
#define LOG_LED_INFO(message, x...) 	console_logger(LOGTYPE_INFO, STRING_LEVEL_INFO, COLOR_CYAN, "LED", __FILE__, __func__, message, ## x)
#define LOG_LED_ERROR(message, x...)	console_logger(LOGTYPE_ERROR, STRING_LEVEL_ERROR, COLOR_RED, "LED", __FILE__, __func__, message, ## x)
#define LOG_LED_WARN(message, x...)		console_logger(LOGTYPE_WARN, STRING_LEVEL_WARN, COLOR_YELLOW, "LED", __FILE__, __func__, message, ## x)


#define LOG_CONSOLE_ENTER()					console_logger(LOGTYPE_ENTER, STRING_LEVEL_ENTER, COLOR_WHITE, "Console", __FILE__, __func__, NULL)
#define LOG_CONSOLE_EXIT()					console_logger(LOGTYPE_EXIT, STRING_LEVEL_EXIT, COLOR_WHITE, "Console", __FILE__, __func__, NULL)
#define LOG_CONSOLE_DEBUG(message, x...)	console_logger(LOGTYPE_DEBUG, STRING_LEVEL_DEBUG, COLOR_WHITE, "Console", __FILE__, __func__, message, ## x)
#define LOG_CONSOLE_INFO(message, x...) 	console_logger(LOGTYPE_INFO, STRING_LEVEL_INFO, COLOR_CYAN, "Console", __FILE__, __func__, message, ## x)
#define LOG_CONSOLE_ERROR(message, x...)	console_logger(LOGTYPE_ERROR, STRING_LEVEL_ERROR, COLOR_RED, "Console", __FILE__, __func__, message, ## x)
#define LOG_CONSOLE_WARN(message, x...)		console_logger(LOGTYPE_WARN, STRING_LEVEL_WARN, COLOR_YELLOW, "Console", __FILE__, __func__, message, ## x)

#define LOG_RTOS_ENTER()				console_logger(LOGTYPE_ENTER, STRING_LEVEL_ENTER, COLOR_WHITE, "RTOS", __FILE__, __func__, NULL)
#define LOG_RTOS_EXIT()					console_logger(LOGTYPE_EXIT, STRING_LEVEL_EXIT, COLOR_WHITE, "RTOS", __FILE__, __func__, NULL)
#define LOG_RTOS_DEBUG(message, x...)	console_logger(LOGTYPE_DEBUG, STRING_LEVEL_DEBUG, COLOR_WHITE, "RTOS", __FILE__, __func__, message, ## x)
#define LOG_RTOS_INFO(message, x...) 	console_logger(LOGTYPE_INFO, STRING_LEVEL_INFO, COLOR_CYAN, "RTOS", __FILE__, __func__, message, ## x)
#define LOG_RTOS_ERROR(message, x...)	console_logger(LOGTYPE_ERROR, STRING_LEVEL_ERROR, COLOR_RED, "RTOS", __FILE__, __func__, message, ## x)
#define LOG_RTOS_WARN(message, x...)	console_logger(LOGTYPE_WARN, STRING_LEVEL_WARN, COLOR_YELLOW, "RTOS", __FILE__, __func__, message, ## x)



#endif /* INC_LOGGER_H_ */
