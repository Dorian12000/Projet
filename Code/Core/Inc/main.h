/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LIDAR_M_EN_Pin GPIO_PIN_4
#define LIDAR_M_EN_GPIO_Port GPIOC
#define LIDAR_DEV_EN_Pin GPIO_PIN_5
#define LIDAR_DEV_EN_GPIO_Port GPIOC
#define LIDAR_M_SCTR_Pin GPIO_PIN_1
#define LIDAR_M_SCTR_GPIO_Port GPIOB
#define LED_RED_Pin GPIO_PIN_11
#define LED_RED_GPIO_Port GPIOB
#define LED_ORANGE_Pin GPIO_PIN_12
#define LED_ORANGE_GPIO_Port GPIOB
#define LED_GREEN_Pin GPIO_PIN_13
#define LED_GREEN_GPIO_Port GPIOB
#define MR_REV_Pin GPIO_PIN_14
#define MR_REV_GPIO_Port GPIOB
#define MR_FWD_Pin GPIO_PIN_15
#define MR_FWD_GPIO_Port GPIOB
#define MR_ENC_B_Pin GPIO_PIN_8
#define MR_ENC_B_GPIO_Port GPIOA
#define MR_ENC_A_Pin GPIO_PIN_9
#define MR_ENC_A_GPIO_Port GPIOA
#define ML_ENC_B_Pin GPIO_PIN_6
#define ML_ENC_B_GPIO_Port GPIOC
#define ML_ENC_A_Pin GPIO_PIN_7
#define ML_ENC_A_GPIO_Port GPIOC
#define ML_REV_Pin GPIO_PIN_0
#define ML_REV_GPIO_Port GPIOD
#define ML_FWD_Pin GPIO_PIN_1
#define ML_FWD_GPIO_Port GPIOD
#define BORDER_BACK_Pin GPIO_PIN_2
#define BORDER_BACK_GPIO_Port GPIOD
#define BORDER_BACK_EXTI_IRQn EXTI2_3_IRQn
#define BORDER_FRONT_Pin GPIO_PIN_3
#define BORDER_FRONT_GPIO_Port GPIOD
#define BORDER_FRONT_EXTI_IRQn EXTI2_3_IRQn
#define PC_TX_Pin GPIO_PIN_5
#define PC_TX_GPIO_Port GPIOD
#define PC_RX_Pin GPIO_PIN_6
#define PC_RX_GPIO_Port GPIOD
#define BUMPER_L_Pin GPIO_PIN_6
#define BUMPER_L_GPIO_Port GPIOB
#define BUMPER_R_Pin GPIO_PIN_7
#define BUMPER_R_GPIO_Port GPIOB
#define BUMPER_B_Pin GPIO_PIN_8
#define BUMPER_B_GPIO_Port GPIOB
#define BUMPER_F_Pin GPIO_PIN_9
#define BUMPER_F_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
