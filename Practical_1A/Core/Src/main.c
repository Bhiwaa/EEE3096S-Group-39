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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdint.h>
#include "stm32f0xx.h"
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
TIM_HandleTypeDef htim16;

/* USER CODE BEGIN PV */
// TODO: Define input variables
// Creating an Array of LED pins for ease of configuration.
uint16_t ledPins[8] = { GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3,
GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7 };

int currentDelay = 0; // Tracks the current TIM16 ARR value.

// Variables related to Mode 1 and Mode 2 display
int direction = 1;
int index = 0;
int displayMode = 0;

// Variables related to Mode 3 display. Uninitialized variables are random and set within the interrupt handler.
uint8_t ledPattern_Mode3;
int Mode3_displaydelay;
int Mode3_turnoffdelay;

// Mode3_displaying and count used to switch between showing pattern and systematically removing it.
int Mode3_displaying = 0;
int count = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM16_Init(void);
/* USER CODE BEGIN PFP */
void TIM16_IRQHandler(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

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
	MX_TIM16_Init();
	/* USER CODE BEGIN 2 */

	// TODO: Start timer
	HAL_TIM_Base_Start_IT(&htim16); // starts Tim16 in interrupt Mode

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {

		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */

		// TODO: Check pushbuttons to change timer delay
		/* The code checks whether button PA0 is pressed and switches between a 500ms and 1000ms trigger.
		 * Implemented by changing the ARR value for TIM16
		 */
		if (HAL_GPIO_ReadPin(Button0_GPIO_Port, Button0_Pin) == 0) {
			if (currentDelay == 0) {
				__HAL_TIM_SET_AUTORELOAD(&htim16, 499);
				currentDelay = 1;
			} else {
				__HAL_TIM_SET_AUTORELOAD(&htim16, 999);
				currentDelay = 0;
			}
		}

	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
	while (LL_FLASH_GetLatency() != LL_FLASH_LATENCY_0) {
	}
	LL_RCC_HSI_Enable();

	/* Wait till HSI is ready */
	while (LL_RCC_HSI_IsReady() != 1) {

	}
	LL_RCC_HSI_SetCalibTrimming(16);
	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);

	/* Wait till System clock is ready */
	while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI) {

	}
	LL_SetSystemCoreClock(8000000);

	/* Update the time base */
	if (HAL_InitTick(TICK_INT_PRIORITY) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief TIM16 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM16_Init(void) {

	/* USER CODE BEGIN TIM16_Init 0 */

	/* USER CODE END TIM16_Init 0 */

	/* USER CODE BEGIN TIM16_Init 1 */

	/* USER CODE END TIM16_Init 1 */
	htim16.Instance = TIM16;
	htim16.Init.Prescaler = 8000 - 1;
	htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim16.Init.Period = 1000 - 1;
	htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim16.Init.RepetitionCounter = 0;
	htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_Base_Init(&htim16) != HAL_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN TIM16_Init 2 */
	NVIC_EnableIRQ(TIM16_IRQn);
	/* USER CODE END TIM16_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOF);
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

	//
	LL_GPIO_ResetOutputPin(LED0_GPIO_Port, LED0_Pin);

	//
	LL_GPIO_ResetOutputPin(LED1_GPIO_Port, LED1_Pin);

	//
	LL_GPIO_ResetOutputPin(LED2_GPIO_Port, LED2_Pin);

	//
	LL_GPIO_ResetOutputPin(LED3_GPIO_Port, LED3_Pin);

	//
	LL_GPIO_ResetOutputPin(LED4_GPIO_Port, LED4_Pin);

	//
	LL_GPIO_ResetOutputPin(LED5_GPIO_Port, LED5_Pin);

	//
	LL_GPIO_ResetOutputPin(LED6_GPIO_Port, LED6_Pin);

	//
	LL_GPIO_ResetOutputPin(LED7_GPIO_Port, LED7_Pin);

	//
	GPIO_InitStruct.Pin = Button0_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	LL_GPIO_Init(Button0_GPIO_Port, &GPIO_InitStruct);

	//
	GPIO_InitStruct.Pin = Button1_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	LL_GPIO_Init(Button1_GPIO_Port, &GPIO_InitStruct);

	//
	GPIO_InitStruct.Pin = Button2_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	LL_GPIO_Init(Button2_GPIO_Port, &GPIO_InitStruct);

	//
	GPIO_InitStruct.Pin = Button3_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	LL_GPIO_Init(Button3_GPIO_Port, &GPIO_InitStruct);

	//
	GPIO_InitStruct.Pin = LED0_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(LED0_GPIO_Port, &GPIO_InitStruct);

	//
	GPIO_InitStruct.Pin = LED1_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(LED1_GPIO_Port, &GPIO_InitStruct);

	//
	GPIO_InitStruct.Pin = LED2_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(LED2_GPIO_Port, &GPIO_InitStruct);

	//
	GPIO_InitStruct.Pin = LED3_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(LED3_GPIO_Port, &GPIO_InitStruct);

	//
	GPIO_InitStruct.Pin = LED4_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(LED4_GPIO_Port, &GPIO_InitStruct);

	//
	GPIO_InitStruct.Pin = LED5_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(LED5_GPIO_Port, &GPIO_InitStruct);

	//
	GPIO_InitStruct.Pin = LED6_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(LED6_GPIO_Port, &GPIO_InitStruct);

	//
	GPIO_InitStruct.Pin = LED7_Pin;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	LL_GPIO_Init(LED7_GPIO_Port, &GPIO_InitStruct);

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void TIM16_IRQHandler(void) {
	// Acknowledge interrupt
	HAL_TIM_IRQHandler(&htim16);

	// Resetting the TIM16 to the default of 500ms/1000ms in the event it changed (Mode 3).
	if (HAL_GPIO_ReadPin(Button1_GPIO_Port, Button1_Pin) == 0
			|| HAL_GPIO_ReadPin(Button2_GPIO_Port, Button2_Pin) == 0
			|| HAL_GPIO_ReadPin(Button3_GPIO_Port, Button3_Pin) == 0) {
		if (!currentDelay) {
			__HAL_TIM_SET_AUTORELOAD(&htim16, 999);
		} else {
			__HAL_TIM_SET_AUTORELOAD(&htim16, 499);
		}
		__HAL_TIM_SET_COUNTER(&htim16, 0);
	}

	// TODO: Change LED pattern

	// Selecting a display Mode based on the button Pressed. Resetting any variables in case a Mode was reselected.
	if (HAL_GPIO_ReadPin(Button1_GPIO_Port, Button1_Pin) == 0) {
		displayMode = 1;
		index = 0;
		direction = 1;
	} else if (HAL_GPIO_ReadPin(Button2_GPIO_Port, Button2_Pin) == 0) {
		displayMode = 2;
		index = 0;
		direction = 1;
	} else if (HAL_GPIO_ReadPin(Button3_GPIO_Port, Button3_Pin) == 0) {
		displayMode = 3;
	}

	// switch case to handle the three display modes.
	switch (displayMode) {
	case 1:
		// Code for LED display Mode 1.

		// Turn off all LEDS
		for (int i = 0; i < 8; i++) {
			// No reason to change LED0_GPIO_Port since they map to the same thing
			HAL_GPIO_WritePin(LED0_GPIO_Port, ledPins[i], GPIO_PIN_RESET);
		}

		HAL_GPIO_WritePin(LED0_GPIO_Port, ledPins[index], GPIO_PIN_SET);
		index += direction;

		// Code to check and initiate direction shift
		if (index > 7 || index < 0) {
			direction *= -1;
			index += 2 * direction;
		}
		break;

	case 2:
		// Code for LED display Mode 2

		// Turn off all LEDS
		for (int i = 0; i < 8; i++) {
			// No reason to change LED0_GPIO_Port since they map to the same thing.
			HAL_GPIO_WritePin(LED0_GPIO_Port, ledPins[i], GPIO_PIN_RESET);
		}
		for (int i = 0; i < 8; i++) {
			if (i != index) {
				HAL_GPIO_WritePin(LED0_GPIO_Port, ledPins[i], GPIO_PIN_SET);
			}
		}

		index += direction;

		// Code to check and initiate direction shift.
		if (index > 7 || index < 0) {
			direction *= -1;
			index += 2 * direction;
		}
		break;

	case 3:
		// Code for LED display Mode 3

		if (!Mode3_displaying) // Only set LEDS if not already displaying
		{
			//setting random numbers
			ledPattern_Mode3 = rand() % 256; // Generate a random LED pattern
			Mode3_displaydelay = 100 + rand() % 1401;
			for (int i = 0; i < 8; i++) {
				int state = ledPattern_Mode3 & (1 << i);
				if (state) {
					HAL_GPIO_WritePin(LED0_GPIO_Port, ledPins[i], GPIO_PIN_SET);
				} else {
					HAL_GPIO_WritePin(LED0_GPIO_Port, ledPins[i],GPIO_PIN_RESET);
				}
			}
			// Implement display delay
			__HAL_TIM_SET_AUTORELOAD(&htim16, Mode3_displaydelay);
			__HAL_TIM_SET_COUNTER(&htim16, 0);
			Mode3_displaying = 1;
		}
		else
		{
			HAL_GPIO_WritePin(LED0_GPIO_Port, ledPins[count], GPIO_PIN_RESET);
			Mode3_turnoffdelay = 100 + rand() % 51;
			//Implementing Turn off delay
			__HAL_TIM_SET_AUTORELOAD(&htim16, Mode3_turnoffdelay); // set ARR to default 1 second
			__HAL_TIM_SET_COUNTER(&htim16, 0);
			count++;

			if (count >= 8) // Check to end Turn off Phase.
			{
				count = 0;
				Mode3_displaying = 0;
			}
		}
		break;

	default:
		break;
	}
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
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
