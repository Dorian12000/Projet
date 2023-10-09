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

#define COMMAND_SIZE			0x02
#define HEADER_SIZE				0x07
#define PH_SIZE					0x02
#define	CT_SIZE					0x01
#define LSN_SIZE				0x01
#define FSA_SIZE				0x02
#define	LSA_SIZE				0x02
#define CS_SIZE					0x02
#define Si_SIZE					0x02
#define MODEL_NUMBER_SIZE		0x01
#define FIRMWARE_VERSION_SIZE	0x02
#define HARDWARE_VERSION_SIZE	0x01
#define	SERIAL_NUMBER_SIZE		0x10
#define STATUS_CODE_SIZE		0x01
#define ERROR_CODE_SIZE			0x02


#define LIDAR_MOTOR_ENABLE()	HAL_GPIO_WritePin(M_EN_GPIO_Port, M_EN_Pin, GPIO_PIN_SET)
#define LIDAR_MOTOR_DISABLE()	HAL_GPIO_WritePin(M_EN_GPIO_Port, M_EN_Pin, GPIO_PIN_RESET)
#define LIDAR_DEV_ENABLE()	HAL_GPIO_WritePin(DEV_EN_GPIO_Port, DEV_EN_Pin, GPIO_PIN_SET)
#define LIDAR_DEV_DISABLE()	HAL_GPIO_WritePin(DEV_EN_GPIO_Port, DEV_EN_Pin, GPIO_PIN_RESET)

typedef int (* uart_tx_t)(uint8_t *address, uint8_t *p_data, uint16_t size);
typedef int (* uart_rx_t)(uint8_t *address, uint8_t *p_data, uint16_t size);

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


typedef struct Lidar_sc{
	uint16_t PH; // PACKET HEADER
	uint8_t CT; //Package Type
	uint8_t LSN; // Sample quantity
	uint16_t FSA; // Start Angle
	uint16_t LSA; // Stop Angle
	uint16_t CS; // Check code
	uint16_t SI; // sample data
}lidar_scan_t;


typedef struct lidar_devEUI_s {
	uint8_t ModelNumber;
	uint16_t FirmWareVersion;
	uint8_t HardWareVersion;
	uint8_t SerialNumber[16];
}lidar_devEUI_t;

typedef struct lidar_healthStatus_s{
	uint8_t StatusCode;
	uint8_t ErrorCode;
}lidar_healthStatus_t;

void LidarInit(void);
void LidarSetSpeed(uint8_t speed);
void LidarGetInformation(lidar_devEUI_t *devEUI);
void LidarScanStart(lidar_scan_t *lidscan);
void LidarHealthStatus(lidar_healthStatus_t *healthStatus);

