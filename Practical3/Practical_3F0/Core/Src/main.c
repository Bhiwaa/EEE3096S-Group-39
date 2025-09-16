/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include <stdlib.h>
#include "stm32f0xx.h"
#include <stdbool.h> // Added for preference
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct {
    int overflow_square;
    int overflow_cross;
} Task7_Variables;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
//TODO: Define variables you think you might need
  Task7_Variables task7_variables;
  uint32_t start_time;
  uint32_t end_time;
  uint32_t execution_time;
  uint64_t checksum;
  int Test_ITER = 100;
  int image_dimension[5] = {128, 160, 192, 224, 256};
  int run_Count = 0;
  bool state;
  int MAX_iter[5] = {100,250,500,750,1000};
  int Dimension_val = 0;
  int Iter_val = 0;
  int Scale_val = 0;
  int Scales[3] = {1000,10000,1000000};

// - Performance timing variables (e.g execution time, throughput, pixels per second, clock cycles)

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
//TODO: Define any function prototypes you might need such as the calculate Mandelbrot function among others
uint64_t calculate_mandelbrot_fixed_point_arithmetic(int width, int height, int max_iterations);
uint64_t calculate_mandelbrot_double(int width, int height, int max_iterations);
uint64_t calculate_mandelbrot_var_fixed_point_arithmetic(int width, int height, int max_iterations, int Scale);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    /*Base Mandelbrot Code Implementation*/

		if (!run_Count) {
			// Added to distinguish between which mode is running
			state = true; //  true implies fixed point arithmetic is in use.
			run_Count = 1;
		}

//		for (int i = 0;
//				i < (sizeof(image_dimension) / sizeof(image_dimension[0]));
//				i++) {
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); // Turn on LED0 to signify the start of a computation
//			start_time = HAL_GetTick(); //Record start time
//			checksum = calculate_mandelbrot_fixed_point_arithmetic(
//					image_dimension[i], image_dimension[i], Test_ITER); // Compute Mandelbrot
//
//			/*Retrieve end time and compute execution time*/
//			end_time = HAL_GetTick();
//			execution_time = end_time - start_time;
//
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); // Turn on LED1 to signify end of computation
//			HAL_Delay(1000);
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_RESET); // reset LEDS for next cycle
//		}

		if (run_Count) {
			// Added to distinguish between which mode is running
			state = false; //False implies double method is in use
			run_Count = 0;
		}

		for (int i = 0;
				i < (sizeof(image_dimension) / sizeof(image_dimension[0]));
				i++) {
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); // Turn on LED0 to signify the start of a computation
			start_time = HAL_GetTick(); //Record start time
			checksum = calculate_mandelbrot_double(image_dimension[i],
					image_dimension[i], Test_ITER); // Compute Mandelbrot

			/*Retrieve end time and compute execution time*/
			end_time = HAL_GetTick();
			execution_time = end_time - start_time;

			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); // Turn on LED1 to signify end of computation
			HAL_Delay(1000);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_RESET); // reset LEDS for next cycle
		}
	/*Code for Task 2 execution - Remove comments from lines 120 to 174 to run */
//		if (!run_Count) {
//			// Added to distinguish between which mode is running
//			state = true; //  true implies fixed point arithmetic is in use.
//			run_Count = 1;
//		}
//		for (int j = 0; j < (sizeof(MAX_iter) / sizeof(MAX_iter[0])); j++) {
//			for (int i = 0;
//					i < (sizeof(image_dimension) / sizeof(image_dimension[0]));
//					i++) {
//				Iter_val = MAX_iter[j];
//				Dimension_val = image_dimension[i];
//				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); // Turn on LED0 to signify the start of a computation
//				start_time = HAL_GetTick(); //Record start time
//				checksum = calculate_mandelbrot_fixed_point_arithmetic(
//						image_dimension[i], image_dimension[i], MAX_iter[j]); // Compute Mandelbrot
//
//				/*Retrieve end time and compute execution time*/
//				end_time = HAL_GetTick();
//				execution_time = end_time - start_time;
//
//				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); // Turn on LED1 to signify end of computation
//				HAL_Delay(1000);
//				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1,
//						GPIO_PIN_RESET); // reset LEDS for next cycle
//			}
//		}
//		if (run_Count) {
//			// Added to distinguish between which mode is running
//			state = false; //False implies double method is in use
//			run_Count = 0;
//		}
//		for (int j = 0; j < (sizeof(MAX_iter) / sizeof(MAX_iter[0])); j++) {
//			for (int i = 0;
//					i < (sizeof(image_dimension) / sizeof(image_dimension[0]));
//					i++) {
//				Iter_val = MAX_iter[j];
//				Dimension_val = image_dimension[i];
//				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); // Turn on LED0 to signify the start of a computation
//				start_time = HAL_GetTick(); //Record start time
//				checksum = calculate_mandelbrot_double(image_dimension[i],
//						image_dimension[i], MAX_iter[j]); // Compute Mandelbrot
//
//				/*Retrieve end time and compute execution time*/
//				end_time = HAL_GetTick();
//				execution_time = end_time - start_time;
//
//				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); // Turn on LED1 to signify end of computation
//				HAL_Delay(1000);
//				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1,
//						GPIO_PIN_RESET); // reset LEDS for next cycle
//			}
//		}
//
//  /*Code for Task 7 */
//		for (int j = 0; j < (sizeof(Scales) / sizeof(Scales[0])); j++) {
//			for (int i = 0;
//					i < (sizeof(image_dimension) / sizeof(image_dimension[0]));
//					i++) {
//				Scale_val = Scales[j];
//				Dimension_val = image_dimension[i];
//				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); // Turn on LED0 to signify the start of a computation
//				start_time = HAL_GetTick(); //Record start time
//				checksum = calculate_mandelbrot_var_fixed_point_arithmetic(
//						image_dimension[i], image_dimension[i], Test_ITER,Scales[j]); // Compute Mandelbrot
//
//				/*Retrieve end time and compute execution time*/
//				end_time = HAL_GetTick();
//				execution_time = end_time - start_time;
//
//				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET); // Turn on LED1 to signify end of computation
//				HAL_Delay(1000);
//				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_1,
//						GPIO_PIN_RESET); // reset LEDS for next cycle
//			}
//  	  }
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
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pins : PB0 PB1 PB2 PB3
                           PB4 PB5 PB6 PB7 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
//TODO: Function signatures you defined previously , implement them here
uint64_t calculate_mandelbrot_fixed_point_arithmetic(int width, int height,
		int max_iterations) {
	uint64_t mandelbrot_sum = 0;
	int64_t S = 65536; // Scaling factor for fixed point arithmetic-corresponds to 2^16
	//TODO: Complete the function implementation
	for (int y = 0; y < (height); y++) {
		for (int x = 0; x < (width); x++) {
			int64_t x_0 = ((int64_t) x * (int64_t) (3.5 * S)) / width
					- (int64_t) (2.5 * S);
			int64_t y_0 = ((int64_t) y * (int64_t) (2.0 * S)) / height
					- (int64_t) (1.0 * S);
			int64_t x_i = 0;
			int64_t y_i = 0;
			int iteration = 0;
			while ((iteration < max_iterations)
					&& (((x_i * x_i) / S) + ((y_i * y_i) / S) <= (4 * S))) {
				int64_t temp = ((x_i * x_i) / S) - ((y_i * y_i) / S);
				y_i = (2 * x_i * y_i) / (S) + y_0;
				x_i = temp + x_0;
				iteration++;
			}
			mandelbrot_sum += iteration;
		}
	}
	return mandelbrot_sum;
}

uint64_t calculate_mandelbrot_var_fixed_point_arithmetic(int width, int height,	int max_iterations,int Scale) {
	uint64_t mandelbrot_sum = 0;
	int64_t S = Scale; // Scaling factor for fixed point arithmetic set vary between 10^3,10^4,10^6
	task7_variables.overflow_square = 0;
	task7_variables.overflow_cross  = 0;
	//TODO: Complete the function implementation
	for (int y = 0; y < (height); y++) {
		for (int x = 0; x < (width); x++) {
			int64_t x_0 = ((int64_t) x * (int64_t) (3.5 * S)) / width
					- (int64_t) (2.5 * S);
			int64_t y_0 = ((int64_t) y * (int64_t) (2.0 * S)) / height
					- (int64_t) (1.0 * S);
			int64_t x_i = 0;
			int64_t y_i = 0;
			int iteration = 0;
			while ((iteration < max_iterations)
					&& (((x_i * x_i) / S) + ((y_i * y_i) / S) <= (4 * S))) {
				// Check potential overflow for squares- Alters execution time so comment out
				if (x_i != 0 && abs(x_i) > INT32_MAX / abs(x_i)) {
					task7_variables.overflow_square++;
				}
			    // Check potential overflow for cross-term (2 * x_i * y_i)- Alters execution time so comment out
				if (x_i != 0 && y_i != 0
						&& abs(y_i) > INT32_MAX / (1 * abs(x_i))) {
					task7_variables.overflow_cross++;
				}
				int64_t temp = ((x_i * x_i) / S) - ((y_i * y_i) / S);
				y_i = (2 * x_i * y_i) / (S) + y_0;
				x_i = temp + x_0;
				iteration++;

			}
			mandelbrot_sum += iteration;
		}
	}
	return mandelbrot_sum;
}

uint64_t calculate_mandelbrot_double(int width, int height, int max_iterations) {
	uint64_t mandelbrot_sum = 0;
	//TODO: Complete the function implementation
	for (int y = 0; y < (height); y++) {
		for (int x = 0; x < (width); x++) {
			double x_0 = (((double) x) / width) * 3.5 - 2.5;
			double y_0 = (((double) y) / height) * 2.0 - 1.0;
			double x_i = 0;
			double y_i = 0;
			int iteration = 0;
			while ((iteration < max_iterations)
					&& (x_i * x_i + y_i * y_i) <= 4.0) {
				double temp = x_i * x_i - y_i * y_i;
				y_i = (2.0 * x_i * y_i) + y_0;
				x_i = temp + x_0;
				iteration++;
			}
			mandelbrot_sum += iteration;
		}
	}
	return mandelbrot_sum;
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
#ifdef USE_FULL_ASSERT
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
