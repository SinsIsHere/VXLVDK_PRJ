/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#define NORMAL_STATE GPIO_PIN_SET
#define PRESSED_STATE GPIO_PIN_RESET
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int timer0_counter = 0;
int timer0_flag = 0;
void setTimer0(int duration) {
	timer0_counter = duration / 10;
	timer0_flag = 0;
}
void timerRun() {
	if (timer0_counter > 0) {
		timer0_counter--;
		if (timer0_counter <= 0) {
			timer0_flag = 1;
		}
	}
}

//********************************************************************************************//
int KeyGen0 = NORMAL_STATE;
int KeyGen1 = NORMAL_STATE;
int KeyGen2 = NORMAL_STATE;
int KeyGen3 = NORMAL_STATE;

int BUTTON_flag = 0;

int BUTTON_PRESSED() {
	if (BUTTON_flag == 1) {
		//BUTTON_flag = 0;
		return 1;
	}
	return 0;
}

void getKeyInput() {
	KeyGen0 = KeyGen1;
	KeyGen1 = KeyGen2;
	KeyGen2 = HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin);

	if ((KeyGen0 == KeyGen1) && (KeyGen1 == KeyGen2)) {
		if (KeyGen3 != KeyGen2) {
			KeyGen3 = KeyGen2;
			if (KeyGen2 == NORMAL_STATE) {
				//TODO
				BUTTON_flag = 1;
			}
		}
	}
}

//********************************************************************************************//


int fsm_status = 0;
int counter = 0;
int red_length = 5;
int yellow_length = 2;
int green_length = 3;
int total_length = 10;







void fsm_run() {
	switch (fsm_status) {
		case 0:
			setTimer0(10);
			counter = total_length;
			HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, 1);
			HAL_GPIO_WritePin(LED_RED_0_GPIO_Port, LED_RED_0_Pin, 1);
			HAL_GPIO_WritePin(LED_YELLOW_0_GPIO_Port, LED_YELLOW_0_Pin, 1);
			HAL_GPIO_WritePin(LED_GREEN_0_GPIO_Port, LED_GREEN_0_Pin, 1);
			HAL_GPIO_WritePin(LED_RED_1_GPIO_Port, LED_RED_1_Pin, 1);
			HAL_GPIO_WritePin(LED_YELLOW_1_GPIO_Port, LED_YELLOW_1_Pin, 1);
			HAL_GPIO_WritePin(LED_GREEN_1_GPIO_Port, LED_GREEN_1_Pin, 1);
			HAL_GPIO_WritePin(PED_RED_0_GPIO_Port, PED_RED_0_Pin, 1);
			HAL_GPIO_WritePin(PED_GREEN_0_GPIO_Port, PED_GREEN_0_Pin, 1);
			HAL_GPIO_WritePin(PED_RED_1_GPIO_Port, PED_RED_1_Pin, 1);
			HAL_GPIO_WritePin(PED_GREEN_1_GPIO_Port, PED_GREEN_1_Pin, 1);
			fsm_status = 1;
			break;
		case 1:
			if (green_length + yellow_length < counter && counter <= total_length) {
				HAL_GPIO_WritePin(LED_RED_0_GPIO_Port, LED_RED_0_Pin, 0);
				HAL_GPIO_WritePin(LED_YELLOW_0_GPIO_Port, LED_YELLOW_0_Pin, 1);
				HAL_GPIO_WritePin(LED_GREEN_0_GPIO_Port, LED_GREEN_0_Pin, 1);
				HAL_GPIO_WritePin(PED_RED_0_GPIO_Port, PED_RED_0_Pin, 1);
				HAL_GPIO_WritePin(PED_GREEN_0_GPIO_Port, PED_GREEN_0_Pin, 0);
				if (BUTTON_PRESSED() == 1) HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, 0);
				//counter_0--;
			}

			if (red_length + yellow_length < counter && counter <= total_length) {
				HAL_GPIO_WritePin(LED_RED_1_GPIO_Port, LED_RED_1_Pin, 1);
				HAL_GPIO_WritePin(LED_YELLOW_1_GPIO_Port, LED_YELLOW_1_Pin, 1);
				HAL_GPIO_WritePin(LED_GREEN_1_GPIO_Port, LED_GREEN_1_Pin, 0);
				HAL_GPIO_WritePin(PED_RED_1_GPIO_Port, PED_RED_1_Pin, 0);
				HAL_GPIO_WritePin(PED_GREEN_1_GPIO_Port, PED_GREEN_1_Pin, 1);
				//counter_1--;
			}

			if (red_length < counter && counter <= total_length - green_length) {
				HAL_GPIO_WritePin(LED_RED_1_GPIO_Port, LED_RED_1_Pin, 1);
				HAL_GPIO_WritePin(LED_YELLOW_1_GPIO_Port, LED_YELLOW_1_Pin, 0);
				HAL_GPIO_WritePin(LED_GREEN_1_GPIO_Port, LED_GREEN_1_Pin, 1);
				HAL_GPIO_WritePin(PED_RED_1_GPIO_Port, PED_RED_1_Pin, 0);
				HAL_GPIO_WritePin(PED_GREEN_1_GPIO_Port, PED_GREEN_1_Pin, 1);
				//counter_1--;
			}

			if (timer0_flag == 1) {
				counter--;
				setTimer0(1000);
			}

			if (counter == green_length + yellow_length) {
				fsm_status = 2;
				BUTTON_flag = 0;
				break;
			}

			fsm_status = 1;

			break;

		case 2:
			if (yellow_length < counter && counter <= green_length + yellow_length) {
				HAL_GPIO_WritePin(LED_RED_0_GPIO_Port, LED_RED_0_Pin, 1);
				HAL_GPIO_WritePin(LED_YELLOW_0_GPIO_Port, LED_YELLOW_0_Pin, 1);
				HAL_GPIO_WritePin(LED_GREEN_0_GPIO_Port, LED_GREEN_0_Pin, 0);
				HAL_GPIO_WritePin(PED_RED_0_GPIO_Port, PED_RED_0_Pin, 0);
				HAL_GPIO_WritePin(PED_GREEN_0_GPIO_Port, PED_GREEN_0_Pin, 1);
				if (BUTTON_PRESSED() == 0) HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, 1);
				//counter_0--;
			}

			if (0 < counter && counter <= yellow_length) {
				HAL_GPIO_WritePin(LED_RED_0_GPIO_Port, LED_RED_0_Pin, 1);
				HAL_GPIO_WritePin(LED_YELLOW_0_GPIO_Port, LED_YELLOW_0_Pin, 0);
				HAL_GPIO_WritePin(LED_GREEN_0_GPIO_Port, LED_GREEN_0_Pin, 1);
				HAL_GPIO_WritePin(PED_RED_0_GPIO_Port, PED_RED_0_Pin, 0);
				HAL_GPIO_WritePin(PED_GREEN_0_GPIO_Port, PED_GREEN_0_Pin, 1);
				if (BUTTON_PRESSED() == 0) HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, 1);
				//counter_0--;
			}

			if (0 < counter && counter <= red_length) {
				HAL_GPIO_WritePin(LED_RED_1_GPIO_Port, LED_RED_1_Pin, 0);
				HAL_GPIO_WritePin(LED_YELLOW_1_GPIO_Port, LED_YELLOW_1_Pin, 1);
				HAL_GPIO_WritePin(LED_GREEN_1_GPIO_Port, LED_GREEN_1_Pin, 1);
				HAL_GPIO_WritePin(PED_RED_1_GPIO_Port, PED_RED_1_Pin, 1);
				HAL_GPIO_WritePin(PED_GREEN_1_GPIO_Port, PED_GREEN_1_Pin, 0);
				//counter_1--;
			}

			if (timer0_flag == 1) {
				counter--;
				setTimer0(1000);
			}

			if (counter == 0) {
				fsm_status = 0;
				break;
			}

			/*if (//buttonpressed) {
			 * fsm_status = 3;
			 * setTimer0()
			 * }*/

			fsm_status = 2;
			break;

		default:
			break;
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
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  fsm_run();
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED_RED_0_Pin|LED_YELLOW_0_Pin|LED_GREEN_0_Pin|LED_RED_Pin
                          |PED_RED_0_Pin|PED_GREEN_0_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LED_RED_1_Pin|LED_YELLOW_1_Pin|LED_GREEN_1_Pin|PED_RED_1_Pin
                          |PED_GREEN_1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : BUTTON_Pin */
  GPIO_InitStruct.Pin = BUTTON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(BUTTON_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_RED_0_Pin LED_YELLOW_0_Pin LED_GREEN_0_Pin LED_RED_Pin
                           PED_RED_0_Pin PED_GREEN_0_Pin */
  GPIO_InitStruct.Pin = LED_RED_0_Pin|LED_YELLOW_0_Pin|LED_GREEN_0_Pin|LED_RED_Pin
                          |PED_RED_0_Pin|PED_GREEN_0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LED_RED_1_Pin LED_YELLOW_1_Pin LED_GREEN_1_Pin PED_RED_1_Pin
                           PED_GREEN_1_Pin */
  GPIO_InitStruct.Pin = LED_RED_1_Pin|LED_YELLOW_1_Pin|LED_GREEN_1_Pin|PED_RED_1_Pin
                          |PED_GREEN_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	getKeyInput();
	timerRun();
}
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
