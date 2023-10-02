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
#define GET_DEV_ID 			0xA590
#define GET_HEALTH_STATUS	0XA591

#define LIDAR_MOTOR_ENABLE()	HAL_GPIO_WritePin(M_EN_GPIO_Port, M_EN_Pin, GPIO_PIN_SET)
#define LIDAR_MOTOR_DISABLE()	HAL_GPIO_WritePin(M_EN_GPIO_Port, M_EN_Pin, GPIO_PIN_RESET)
#define LIDAR_DEV_ENABLE()	HAL_GPIO_WritePin(DEV_EN_GPIO_Port, DEV_EN_Pin, GPIO_PIN_SET)
#define LIDAR_DEV_DISABLE()	HAL_GPIO_WritePin(DEV_EN_GPIO_Port, DEV_EN_Pin, GPIO_PIN_RESET)

typedef int (* uart_tx_t)(uint8_t address, uint8_t *p_data, uint16_t size);
typedef int (* uart_rx_t)(uint8_t address, uint8_t *p_data, uint16_t size);

typedef struct uart_s{
	uart_tx_t tx;
	uart_rx_t rx;
}uart_t;

typedef struct lidar_s{
	uart_t uart;
	uint8_t SampleQuantity;
	uint16_t StartAngle;
	uint16_t StopAngle;
	uint16_t CS; // check code
}lidar_t;

typedef struct lidar_devEUI_s {
	uint8_t ModelNumber;
	uint16_t FirmWareVersion;
	uint8_t HardWareVersion;
	int SerialNumber;
}lidar_devEUI_t;

void LidarInit(void);
void LidarSetSpeed(uint8_t speed);
void LidarGetInformation(lidar_devEUI_t *devEUI);