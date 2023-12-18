/**
 *     _______    _____     __     ______    _______        ____
 *    |   ____|  |     \   |  |   / _____)  |   ____|      /    \
 *    |  |__     |  |\  \  |  |  ( (____    |  |__        /  /\  \
 *    |   __|    |  | \  \ |  |   \____ \   |   __|      /  ____  \
 *    |  |____   |  |  \  \|  |   _____) )  |  |____    /  /    \  \
 *    |_______|  |__|   \_____|  (______/   |_______|  /__/      \__\
 *
 * @file    mainTask.h
 * @Author  Dorian Dalbin, Clement Du, Laksan Thirukumaran, Gael Gourdin
 * @Created	2023-10-28
 * @brief
 *
 **/
#ifndef INC_MAIN_TASK_H_
#define INC_MAIN_TASK_H_

typedef enum {
	MAIN_INIT,
	MAIN_CAT,
	MAIN_MOUSE,
	MAIN_ERROR,
}mainState_t;

typedef enum {
	BUMPER_F_NOTIFY,
	BUMPER_B_NOTIFY,
	BUMPER_R_NOTIFY,
	BUMPER_L_NOTIFY,
	BORDER_F_NOTIFY,
	BORDER_B_NOTIFY,
}ISRnotify_t;

mainState_t getMainState(void);
void setMainState(void);

uint8_t createMainTask(void);
void mainTask(void);

#endif
