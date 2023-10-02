/**
 *     _______    _____     __     ______    _______        ____
 *    |   ____|  |     \   |  |   / _____)  |   ____|      /    \
 *    |  |__     |  |\  \  |  |  ( (____    |  |__        /  /\  \
 *    |   __|    |  | \  \ |  |   \____ \   |   __|      /  ____  \
 *    |  |____   |  |  \  \|  |   _____) )  |  |____    /  /    \  \
 *    |_______|  |__|   \_____|  (______/   |_______|  /__/      \__\
 *
 * @file    drv_lidar.c
 * @Author  Dorian Dalbin, Clement Du, Laksan Thirukumaran, Gael Gourdin
 * @Created	2023-10-28
 * @brief
 *
 **/

#include "main.h"
#include "usart.h"
#include "tim.h"
#include "gpio.h"

#include "log/logger.h"

#include "drv_lidar.h"

lidar_t lidar;

static inline HAL_StatusTypeDef LidarUarTx(uint8_t address, uint8_t *p_data, uint16_t size) {
	HAL_StatusTypeDef status;
	HAL_UART_Transmit(&huart1, &address, 2, HAL_MAX_DELAY);
	HAL_UART_Transmit(&huart1, p_data, size, HAL_MAX_DELAY);
	return status;
}

static inline HAL_StatusTypeDef LidarUartRx(uint8_t address, uint8_t *p_data, uint16_t size) {
	HAL_StatusTypeDef status;
	HAL_UART_Receive(&huart1, p_data, size, HAL_MAX_DELAY);
	return status;
}

void LidarInit(void) {
	lidar.uart.tx = LidarUarTx;
	lidar.uart.rx = LidarUartRx;
}

/**
 * @brief Définit la vitesse du Lidar.
 *
 * Cette fonction permet de régler la vitesse du Lidar.
 *
 * @param speed La nouvelle valeur de vitesse à définir.
 *
 * @note 0: vitesse minimale
 * @note 50: vitesse maximale.
 */
void LidarSetSpeed(uint8_t speed) {
	speed += 49;
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1 ,speed);
}

void LidarGetInformation(void) {
	lidar.uart.tx(GET_DEV_ID, NULL, 0);
	uint8_t buf[20];
	lidar.uart.rx(NULL, buf, 20);
	LOG_MAIN_INFO("%b", buf, 20);
}