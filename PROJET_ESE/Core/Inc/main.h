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
#define TIM15_CH2_REV_MR_Pin GPIO_PIN_2
#define TIM15_CH2_REV_MR_GPIO_Port GPIOC
#define ADC1_IN0_BAT_LVL_Pin GPIO_PIN_0
#define ADC1_IN0_BAT_LVL_GPIO_Port GPIOA
#define TIM15_CH1_REV_ML_Pin GPIO_PIN_2
#define TIM15_CH1_REV_ML_GPIO_Port GPIOA
#define TIM14_CH1_M_SCTR_Pin GPIO_PIN_4
#define TIM14_CH1_M_SCTR_GPIO_Port GPIOA
#define TIM3_CH1_ENCODER_LA_Pin GPIO_PIN_6
#define TIM3_CH1_ENCODER_LA_GPIO_Port GPIOA
#define TIM3_CH2_ENCODER_LB_Pin GPIO_PIN_7
#define TIM3_CH2_ENCODER_LB_GPIO_Port GPIOA
#define USART1_TX_VCP_TX_Pin GPIO_PIN_4
#define USART1_TX_VCP_TX_GPIO_Port GPIOC
#define USART1_RX_VCP_RX_Pin GPIO_PIN_5
#define USART1_RX_VCP_RX_GPIO_Port GPIOC
#define V_BORDURE_R_Pin GPIO_PIN_0
#define V_BORDURE_R_GPIO_Port GPIOB
#define DEV_EN_Pin GPIO_PIN_1
#define DEV_EN_GPIO_Port GPIOB
#define M_EN_Pin GPIO_PIN_2
#define M_EN_GPIO_Port GPIOB
#define TIM1_CH1_ENCODER_RB_Pin GPIO_PIN_8
#define TIM1_CH1_ENCODER_RB_GPIO_Port GPIOC
#define TIM1_CH2_ENCODER_RA_Pin GPIO_PIN_9
#define TIM1_CH2_ENCODER_RA_GPIO_Port GPIOC
#define TIM16_CH1_FWD_ML_Pin GPIO_PIN_0
#define TIM16_CH1_FWD_ML_GPIO_Port GPIOD
#define TIM17_CH1_FWD_MR_Pin GPIO_PIN_1
#define TIM17_CH1_FWD_MR_GPIO_Port GPIOD
#define USART2_TX_LIDAR_TX_Pin GPIO_PIN_5
#define USART2_TX_LIDAR_TX_GPIO_Port GPIOD
#define USART2_RX_LIDAR_RX_Pin GPIO_PIN_6
#define USART2_RX_LIDAR_RX_GPIO_Port GPIOD
#define LED_R_Pin GPIO_PIN_3
#define LED_R_GPIO_Port GPIOB
#define LED_B_Pin GPIO_PIN_4
#define LED_B_GPIO_Port GPIOB
#define BUMPER_F_Pin GPIO_PIN_5
#define BUMPER_F_GPIO_Port GPIOB
#define BUMPER_B_Pin GPIO_PIN_6
#define BUMPER_B_GPIO_Port GPIOB
#define BUMPER_R_Pin GPIO_PIN_7
#define BUMPER_R_GPIO_Port GPIOB
#define BUMPER_L_Pin GPIO_PIN_8
#define BUMPER_L_GPIO_Port GPIOB
#define V_BORDURE_F_Pin GPIO_PIN_9
#define V_BORDURE_F_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
