/**
 *     _______    _____     __     ______    _______        ____
 *    |   ____|  |     \   |  |   / _____)  |   ____|      /    \
 *    |  |__     |  |\  \  |  |  ( (____    |  |__        /  /\  \
 *    |   __|    |  | \  \ |  |   \____ \   |   __|      /  ____  \
 *    |  |____   |  |  \  \|  |   _____) )  |  |____    /  /    \  \
 *    |_______|  |__|   \_____|  (______/   |_______|  /__/      \__\
 *
 * @file    drv_lidar.h
 * @Author  Dorian Dalbin, Clement Du, Laksan Thirukumaran, Gael Gourdin
 * @Created	2023-10-28
 * @brief
 *
 **/

#include "main.h"
#include "usart.h"
#include "tim.h"
#include "gpio.h"

#define LIDAR_START 		0xA55A
#define LIDAR_SCAN_START 	0xA560
#define LIDAR_SCAN_STOP 	0xA565
#define LIDAR_RESTART 		0xA580
#define GET_DEV_ID 			0xA591

#define LIDAR_MOTOR_ENABLE()	HAL_GPIO_WritePin(M_EN_GPIO_Port, M_EN_Pin, GPIO_PIN_SET)
#define LIDAR_MOTOR_DISABLE()	HAL_GPIO_WritePin(M_EN_GPIO_Port, M_EN_Pin, GPIO_PIN_RESET)

typedef int (* lidar_tx_t)(uint8_t address, uint8_t *p_data, uint16_t size);
typedef int (* lidar_rx_t)(uint8_t address, uint8_t *p_data, uint16_t size);

typedef struct LidarUart_s{
	lidar_rx_t Rx;
	lidar_tx_t Tx;
}LidarUart_t;

void LidarInit(void);
void LidarSetSpeed(uint8_t speed);