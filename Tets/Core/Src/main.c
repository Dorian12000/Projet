/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "motor_drv.h"
#include "encoder.h"
#include "pwm.h"
#include "shell.h"
#include "pid.h"
#include <math.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

#define FE 100
#define TE 0.01
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
  uint32_t last_encoder_value = 0;
  uint32_t current_encoder_value = 0;
  float inputs_pid[3] = {0};
  float outputs_pid[3] = {0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int __io_putchar(int chr)
{
	HAL_UART_Transmit(&hlpuart1, (uint8_t*)&chr, 1, HAL_MAX_DELAY);
	return chr;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM3)
	{
		// Lecture encoder
		current_encoder_value = READ_ENCODER(htim2);
		//printf("Current encoder value = %ld\r\n", current_encoder_value);

		// Distance entre les deux valeurs d'encoder (en tour)
		float distance = (current_encoder_value - last_encoder_value) / RES_ENCODER;
		//printf("Distance = %f\r\n", distance);

		// Conversion en vitesse (tour par minute)
		float rpm = 60.0 * distance / TE ;
		//printf("Vitesse en tour par minute = %f\r\n", rpm);

		// Conversion de la vitesse entre 0 et 100
		float speed = 100.0 * rpm / (float)MAX_RPM;
		//printf("Vitesse (0-100) = %f\r\n", speed);

		// Erreur entre la consigne et la mesure
		error(&pid_motor_left, inputs_pid, motor_left.speed, speed);

		// Corection
		correcteur(&pid_motor_left, inputs_pid, outputs_pid);

		if(outputs_pid[pid_motor_left.index] != 0)
		{
			printf("%f\r\n", fabs(outputs_pid[pid_motor_left.index]));
			//forwardMotor(&motor_left, outputs_pid[pid_motor_left.index]);
		}
		last_encoder_value = current_encoder_value;
	}
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_LPUART1_UART_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  if(HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL) != HAL_OK) // Encodeur
  {
	  printf("Error start timer encoder\r\n");
	  Error_Handler();
  }
  if(HAL_TIM_Base_Start_IT(&htim3) != HAL_OK) // Interruption asservissement moteur
  {
	  printf("Error start timer 3\r\n");
	  Error_Handler();
  }

  // Pwm motor init
  PWM fwd = {&htim1, TIM_CHANNEL_1, 0.0};
  PWM rev = {&htim1, TIM_CHANNEL_2, 0.0};

  // Motor init
  initMotor(&motor_left, &fwd, &rev);

  // PID init
  initPID(&pid_motor_left, 0.0, 0.0, 0.0, TE);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  forwardMotor(&motor_left, 15);
  HAL_Delay(10000);
  //stopMotor(&motor_left);
  //HAL_TIM_Base_Stop_IT(&htim3);
//  START_PWM(*motor_left.fwd);
//  setPwmDutyCycle(motor_left.fwd, 0);


  while (1)
  {

//	  while(i < 15)
//	  {
//		  current_encoder_value = READ_ENCODER(htim2);
//		  end_ms = HAL_GetTick();
//
//		  time_ms = end_ms - start_ms;
//
//		  printf("i = %d\n\r", i);
//		  printf("%ld\n\r", end_ms);
//		  printf("Time = %ld\r\n", time_ms);
//		  printf("Encoder = %ld\r\n", current_encoder_value);
//		  printf("Speed = %f\r\n", convertEncoderToSpeed(prev_encoder_value, current_encoder_value, time_ms));
//		  printf("\n\n");
//		  prev_encoder_value = current_encoder_value;
//		  start_ms = end_ms;
//
//		  HAL_Delay(1000);
//		  i++;
//
//		  if(i == 7)
//		  {
//			  setPwmDutyCycle(&fwd, 0.3f);
//		  }
//	  }
//	  STOP_PWM(fwd);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV4;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
