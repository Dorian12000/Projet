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

/***************************************commands**************************************/
#define LIDAR_START 				0xA55A
#define LIDAR_SCAN_START 			0xA560
#define LIDAR_SCAN_STOP 			0xA565
#define LIDAR_RESTART 				0xA580
#define LIDAR_GET_DEV_ID 			0xA590
#define LIDAR_GET_HEALTH_STATUS		0XA591

#define COMMAND_SIZE			0x02
#define HEADER_SIZE				0x07
/***************************************scan size************************************/
#define PH_SIZE					0x02
#define	CT_SIZE					0x01
#define LSN_SIZE				0x01
#define FSA_SIZE				0x02
#define	LSA_SIZE				0x02
#define CS_SIZE					0x02
#define Si_SIZE					0x02
#define SCAN_RESPONSE_SIZE		(PH_SIZE + CT_SIZE + LSN_SIZE + FSA_SIZE + LSA_SIZE + CS_SIZE)
/********************************device information size********************************/
#define MODEL_NUMBER_SIZE		0x01
#define FIRMWARE_VERSION_SIZE	0x02
#define HARDWARE_VERSION_SIZE	0x01
#define	SERIAL_NUMBER_SIZE		0x10
#define DEVEUI_SIZE				(MODEL_NUMBER_SIZE + FIRMWARE_VERSION_SIZE + HARDWARE_VERSION_SIZE + SERIAL_NUMBER_SIZE)
/************************************health status size********************************/
#define STATUS_CODE_SIZE		0x01
#define ERROR_CODE_SIZE			0x02
#define HEALTH_STATUS_SIZE		(STATUS_CODE_SIZE + ERROR_CODE_SIZE)


#define LIDAR_MOTOR_ENABLE()	HAL_GPIO_WritePin(M_EN_GPIO_Port, M_EN_Pin, GPIO_PIN_SET)
#define LIDAR_MOTOR_DISABLE()	HAL_GPIO_WritePin(M_EN_GPIO_Port, M_EN_Pin, GPIO_PIN_RESET)
#define LIDAR_DEV_ENABLE()	HAL_GPIO_WritePin(DEV_EN_GPIO_Port, DEV_EN_Pin, GPIO_PIN_SET)
#define LIDAR_DEV_DISABLE()	HAL_GPIO_WritePin(DEV_EN_GPIO_Port, DEV_EN_Pin, GPIO_PIN_RESET)

typedef int (* uart_tx_t)(uint8_t *address, uint8_t *p_data, uint16_t size);
typedef int (* uart_rx_t)(uint8_t *p_data, uint16_t size);

typedef struct uart_s{
	uart_tx_t tx;
	uart_rx_t rx;
}uart_t;

typedef struct position_s{
	float angle;
	uint16_t distance;
}position_t;


typedef struct lidar_s{
	uart_t uart;
	position_t position[100];
}lidar_t;

typedef struct Lidar_sc{
	uint8_t PH[2]; // PACKET HEADER
	uint8_t CT; //Package Type
	uint8_t LSN; // Sample quantity
	uint8_t FSA[2]; // Start Angle
	uint8_t LSA[2]; // Stop Angle
	uint8_t CS[2]; // Check code
	uint8_t SI[100]; // sample data
}lidar_scan_t;


typedef struct lidar_devEUI_s {
	uint8_t ModelNumber;
	uint16_t FirmWareVersion;
	uint8_t HardWareVersion;
	uint8_t SerialNumber[16];
}lidar_devEUI_t;

typedef struct lidar_healthStatus_s{
	uint8_t StatusCode;
	uint8_t ErrorCode[2];
}lidar_healthStatus_t;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void LidarInit(void);

void LidarSetSpeed(uint8_t speed);
void LidarGetInformation(lidar_devEUI_t *devEUI);
void LidarScanStart(void);
void LidarScanStop(void);

void  lidarRestart(void);

int checkCS(lidar_scan_t *lidarData);
void convertSample(lidar_scan_t *lidarData);
void getLidarScanData(lidar_scan_t *lidarScanData);
void LidarDist(lidar_scan_t *lidarScanData);
void convertSamplenonfloat(lidar_scan_t *lidarData);
