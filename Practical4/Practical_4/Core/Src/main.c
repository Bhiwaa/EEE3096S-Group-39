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
#include <stdio.h>
#include "stm32f4xx.h"
#include "lcd_stm32f4.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
// TODO: Add values for below variables
#define NS  128      // Number of samples in LUT
#define TIM2CLK 16000000  // STM Clock frequency: Hint You might want to check the ioc file
#define F_SIGNAL  44100	// Frequency of output analog signal

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
DMA_HandleTypeDef hdma_tim2_ch1;

/* USER CODE BEGIN PV */
// TODO: Add code for global variables, including LUTs
uint32_t Sin_LUT[NS] =      {0x0800, 0x0865, 0x08CA, 0x092E, 0x0992, 0x09F5, 0x0A56, 0x0AB6, 0x0B15, 0x0B71, 0x0BCC, 0x0C24, 0x0C79, 0x0CCB, 0x0D1B, 0x0D67, 0x0DB0, 0x0DF6, 0x0E37, 0x0E75, 0x0EAF, 0x0EE4, 0x0F15, 0x0F42, 0x0F6A, 0x0F8E, 0x0FAD, 0x0FC7, 0x0FDC, 0x0FEC, 0x0FF7, 0x0FFE, 0x0FFF, 0x0FFB, 0x0FF2, 0x0FE5, 0x0FD2, 0x0FBA, 0x0F9E, 0x0F7D, 0x0F57, 0x0F2C, 0x0EFD, 0x0ECA, 0x0E92, 0x0E57, 0x0E17, 0x0DD3, 0x0D8C, 0x0D42, 0x0CF4, 0x0CA3, 0x0C4F, 0x0BF8, 0x0B9F, 0x0B43, 0x0AE6, 0x0A87, 0x0A26, 0x09C4, 0x0960, 0x08FC, 0x0897, 0x0832, 0x07CD, 0x0768, 0x0703, 0x069F, 0x063B, 0x05D9, 0x0578, 0x0519, 0x04BC, 0x0460, 0x0407, 0x03B0, 0x035C, 0x030B, 0x02BD, 0x0273, 0x022C, 0x01E8, 0x01A8, 0x016D, 0x0135, 0x0102, 0x00D3, 0x00A8, 0x0082, 0x0061, 0x0045, 0x002D, 0x001A, 0x000D, 0x0004, 0x0000, 0x0001, 0x0008, 0x0013, 0x0023, 0x0038, 0x0052, 0x0071, 0x0095, 0x00BD, 0x00EA, 0x011B, 0x0150, 0x018A, 0x01C8, 0x0209, 0x024F, 0x0298, 0x02E4, 0x0334, 0x0386, 0x03DB, 0x0433, 0x048E, 0x04EA, 0x0549, 0x05A9, 0x060A, 0x066D, 0x06D1, 0x0735, 0x079A, 0x07FF};
uint32_t Saw_LUT[NS] =      {0x0000, 0x0020, 0x0040, 0x0061, 0x0081, 0x00A1, 0x00C1, 0x00E2, 0x0102, 0x0122, 0x0142, 0x0163, 0x0183, 0x01A3, 0x01C3, 0x01E4, 0x0204, 0x0224, 0x0244, 0x0265, 0x0285, 0x02A5, 0x02C5, 0x02E6, 0x0306, 0x0326, 0x0346, 0x0367, 0x0387, 0x03A7, 0x03C7, 0x03E8, 0x0408, 0x0428, 0x0448, 0x0469, 0x0489, 0x04A9, 0x04C9, 0x04EA, 0x050A, 0x052A, 0x054A, 0x056A, 0x058B, 0x05AB, 0x05CB, 0x05EB, 0x060C, 0x062C, 0x064C, 0x066C, 0x068D, 0x06AD, 0x06CD, 0x06ED, 0x070E, 0x072E, 0x074E, 0x076E, 0x078F, 0x07AF, 0x07CF, 0x07EF, 0x0810, 0x0830, 0x0850, 0x0870, 0x0891, 0x08B1, 0x08D1, 0x08F1, 0x0912, 0x0932, 0x0952, 0x0972, 0x0993, 0x09B3, 0x09D3, 0x09F3, 0x0A14, 0x0A34, 0x0A54, 0x0A74, 0x0A95, 0x0AB5, 0x0AD5, 0x0AF5, 0x0B15, 0x0B36, 0x0B56, 0x0B76, 0x0B96, 0x0BB7, 0x0BD7, 0x0BF7, 0x0C17, 0x0C38, 0x0C58, 0x0C78, 0x0C98, 0x0CB9, 0x0CD9, 0x0CF9, 0x0D19, 0x0D3A, 0x0D5A, 0x0D7A, 0x0D9A, 0x0DBB, 0x0DDB, 0x0DFB, 0x0E1B, 0x0E3C, 0x0E5C, 0x0E7C, 0x0E9C, 0x0EBD, 0x0EDD, 0x0EFD, 0x0F1D, 0x0F3E, 0x0F5E, 0x0F7E, 0x0F9E, 0x0FBF, 0x0FDF, 0x0000};
uint32_t Triangle_LUT[NS] = {0x0000, 0x0040, 0x0081, 0x00C1, 0x0102, 0x0142, 0x0183, 0x01C3, 0x0204, 0x0244, 0x0285, 0x02C5, 0x0306, 0x0346, 0x0387, 0x03C7, 0x0408, 0x0448, 0x0489, 0x04C9, 0x050A, 0x054A, 0x058B, 0x05CB, 0x060C, 0x064C, 0x068D, 0x06CD, 0x070E, 0x074E, 0x078F, 0x07CF, 0x0810, 0x0850, 0x0891, 0x08D1, 0x0912, 0x0952, 0x0993, 0x09D3, 0x0A14, 0x0A54, 0x0A95, 0x0AD5, 0x0B15, 0x0B56, 0x0B96, 0x0BD7, 0x0C17, 0x0C58, 0x0C98, 0x0CD9, 0x0D19, 0x0D5A, 0x0D9A, 0x0DDB, 0x0E1B, 0x0E5C, 0x0E9C, 0x0EDD, 0x0F1D, 0x0F5E, 0x0F9E, 0x0FDF, 0x0FDF, 0x0F9E, 0x0F5E, 0x0F1D, 0x0EDD, 0x0E9C, 0x0E5C, 0x0E1B, 0x0DDB, 0x0D9A, 0x0D5A, 0x0D19, 0x0CD9, 0x0C98, 0x0C58, 0x0C17, 0x0BD7, 0x0B96, 0x0B56, 0x0B15, 0x0AD5, 0x0A95, 0x0A54, 0x0A14, 0x09D3, 0x0993, 0x0952, 0x0912, 0x08D1, 0x0891, 0x0850, 0x0810, 0x07CF, 0x078F, 0x074E, 0x070E, 0x06CD, 0x068D, 0x064C, 0x060C, 0x05CB, 0x058B, 0x054A, 0x050A, 0x04C9, 0x0489, 0x0448, 0x0408, 0x03C7, 0x0387, 0x0346, 0x0306, 0x02C5, 0x0285, 0x0244, 0x0204, 0x01C3, 0x0183, 0x0142, 0x0102, 0x00C1, 0x0081, 0x0040, 0x0000};
uint32_t Piano_LUT =        {0x0800, 0x0880, 0x07C4, 0x0830, 0x080C, 0x0829, 0x07F8, 0x0869, 0x082F, 0x0864, 0x0821, 0x07F7, 0x07F3, 0x090F, 0x080B, 0x06F4, 0x07EE, 0x07FE, 0x07C2, 0x0806, 0x080F, 0x0803, 0x0647, 0x07EA, 0x08EB, 0x0804, 0x0B19, 0x06AF, 0x06EE, 0x073A, 0x08E8, 0x088A, 0x0616, 0x086E, 0x07B5, 0x080E, 0x07C4, 0x0827, 0x06D6, 0x07A5, 0x0794, 0x0833, 0x089C, 0x0792, 0x08C9, 0x0790, 0x0A1B, 0x07EF, 0x082A, 0x080D, 0x07D0, 0x080F, 0x07EA, 0x07F6, 0x0AA1, 0x07A7, 0x0923, 0x074E, 0x07D3, 0x07A2, 0x05DF, 0x0789, 0x06B0, 0x07F5, 0x0701, 0x0815, 0x085A, 0x07FF, 0x07E5, 0x07E5, 0x06C8, 0x0864, 0x0776, 0x0824, 0x0707, 0x09A1, 0x0882, 0x07F2, 0x0928, 0x0879, 0x07F1, 0x07FA, 0x07D4, 0x0817, 0x07F3, 0x07FA, 0x0881, 0x0805, 0x0835, 0x078F, 0x073E, 0x07F3, 0x07DD, 0x08AA, 0x07D4, 0x0836, 0x091B, 0x0804, 0x081C, 0x07F8, 0x0818, 0x07E9, 0x06EA, 0x0789, 0x07CC, 0x07D0, 0x087E, 0x0818, 0x07F5, 0x07D2, 0x0738, 0x0861, 0x07DC, 0x081E, 0x07CD, 0x0817, 0x07F0, 0x080C, 0x079D, 0x075D, 0x0662, 0x0702, 0x081C, 0x0808, 0x07D8, 0x07B1, 0x0811, 0x0800};
uint32_t Guitar_LUT = {0x0800, 0x088E, 0x088B, 0x0821, 0x0819, 0x07AD, 0x07A1, 0x0795, 0x0710, 0x0814, 0x080F, 0x0802, 0x0811, 0x080C, 0x07C0, 0x07FB, 0x07FE, 0x07F7, 0x0768, 0x08C9, 0x07C0, 0x07B8, 0x08BA, 0x083F, 0x0799, 0x0802, 0x07F1, 0x0806, 0x07FB, 0x07E8, 0x07D9, 0x0802, 0x0802, 0x0817, 0x07AE, 0x082D, 0x07AD, 0x0824, 0x07EC, 0x077B, 0x0851, 0x07F5, 0x0804, 0x0801, 0x0807, 0x07CD, 0x0829, 0x07FD, 0x07B4, 0x07E7, 0x0840, 0x07EF, 0x083F, 0x082A, 0x07BA, 0x07ED, 0x081B, 0x0828, 0x0815, 0x07A4, 0x084F, 0x0816, 0x0927, 0x085A, 0x081A, 0x0851, 0x0866, 0x0858, 0x0828, 0x0824, 0x0708, 0x075F, 0x0809, 0x07FE, 0x080E, 0x0807, 0x086B, 0x07FE, 0x07F9, 0x0803, 0x0834, 0x083B, 0x0795, 0x082B, 0x080E, 0x0773, 0x080F, 0x07B6, 0x0811, 0x0817, 0x07FF, 0x07F9, 0x07CB, 0x07B9, 0x081E, 0x07FF, 0x0864, 0x07DE, 0x0858, 0x07E4, 0x07F8, 0x07EB, 0x08DB, 0x08A7, 0x07F0, 0x0801, 0x07FA, 0x07FD, 0x081D, 0x07CC, 0x07FF, 0x0801, 0x07CA, 0x07F7, 0x082D, 0x07BE, 0x086F, 0x07D2, 0x0829, 0x0751, 0x0819, 0x07A3, 0x0897, 0x07C1, 0x074E, 0x08FE, 0x07CD, 0x0800};
uint32_t Drum_LUT = {0x0800, 0x0D33, 0x0749, 0x083E, 0x0810, 0x0D62, 0x0765, 0x078E, 0x0826, 0x0852, 0x07F4, 0x07EF, 0x07F7, 0x0791, 0x0586, 0x0739, 0x082F, 0x07E5, 0x07FD, 0x080B, 0x0800, 0x0818, 0x07FC, 0x07E5, 0x0850, 0x0719, 0x0819, 0x07FD, 0x0853, 0x07FA, 0x0CA4, 0x04FC, 0x059D, 0x088E, 0x0984, 0x0839, 0x06EE, 0x0822, 0x07ED, 0x07F0, 0x0FFE, 0x07AB, 0x07F4, 0x07FA, 0x0986, 0x08E1, 0x071C, 0x07CC, 0x0838, 0x0817, 0x07E2, 0x0804, 0x0319, 0x0973, 0x081B, 0x0815, 0x0037, 0x08C0, 0x07FB, 0x07E8, 0x0870, 0x0801, 0x0B57, 0x0953, 0x0FE3, 0x07DD, 0x08EE, 0x07F7, 0x09E8, 0x0A96, 0x0800, 0x0830, 0x06E0, 0x07D3, 0x0826, 0x07FB, 0x0A4E, 0x0791, 0x080E, 0x081C, 0x07DE, 0x0809, 0x07FB, 0x0801, 0x08A4, 0x0800, 0x0812, 0x0800, 0x0B69, 0x0817, 0x07E2, 0x0821, 0x07D7, 0x0801, 0x0727, 0x07DE, 0x0357, 0x08FE, 0x07BA, 0x07F9, 0x07F5, 0x07F5, 0x083B, 0x07FB, 0x0669, 0x0809, 0x0813, 0x07F0, 0x07FD, 0x0987, 0x0861, 0x08AA, 0x0846, 0x080D, 0x07F7, 0x06F6, 0x0876, 0x097B, 0x0854, 0x0FFF, 0x086A, 0x07DB, 0x07F4, 0x064F, 0x07A6, 0x0129, 0x09DD, 0x0800};
char LCD_CHARS[6][50] = {"Sine Wave","Sawtooth Wave","Triangle Wave","Guitar Wave","Piano Wave","Drums Wave"};




// TODO: Equation to calculate TIM2_Ticks
uint32_t TIM2_Ticks = 0; // How often to write new LUT value
uint32_t DestAddress = (uint32_t) &(TIM3->CCR3); // Write LUT TO TIM3->CCR3 to modify PWM duty cycle


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */
void EXTI0_IRQHandler(void);
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
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  // TODO: Start TIM3 in PWM mode on channel 3
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);

  // TODO: Start TIM2 in Output Compare (OC) mode on channel 1
  HAL_TIM_OC_Start(&htim2, TIM_CHANNEL_1);

  // TODO: Start DMA in IT mode on TIM2->CH1. Source is LUT and Dest is TIM3->CCR3; start with Sine LUT
  HAL_DMA_Start_IT(&hdma_tim2_ch1, Sin_LUT,DestAddress,NS); // Typecast?? Also are we cycling through waveforms.

  // TODO: Write current waveform to LCD(Sine is the first waveform)
  init_LCD();
  lcd_update_line(LCD_CHARS[0]);
  // TODO: Enable DMA (start transfer from LUT to CCR)
  __HAL_TIM_ENABLE_DMA(&htim2, TIM_DMA_CC1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

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
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4294967295;
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
  if (HAL_TIM_OC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_TIMING;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_OC_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */
  /* TIM2_CH1 DMA Init */
  __HAL_RCC_DMA1_CLK_ENABLE();

  hdma_tim2_ch1.Instance = DMA1_Stream5;
  hdma_tim2_ch1.Init.Channel = DMA_CHANNEL_3;         // TIM2_CH1 is on channel 3
  hdma_tim2_ch1.Init.Direction = DMA_MEMORY_TO_PERIPH; // Memory -> TIM3->CCR3
  hdma_tim2_ch1.Init.PeriphInc = DMA_PINC_DISABLE;    // Peripheral address fixed
  hdma_tim2_ch1.Init.MemInc = DMA_MINC_ENABLE;        // Memory address increments
  hdma_tim2_ch1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_tim2_ch1.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
  hdma_tim2_ch1.Init.Mode = DMA_CIRCULAR;            // Repeat LUT automatically
  hdma_tim2_ch1.Init.Priority = DMA_PRIORITY_HIGH;
  hdma_tim2_ch1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;

  if (HAL_DMA_Init(&hdma_tim2_ch1) != HAL_OK)
  {
      Error_Handler();
  }

  /* Link DMA handle to TIM2 handle */
  __HAL_LINKDMA(&htim2, hdma[TIM_DMA_ID_CC1], hdma_tim2_ch1);
  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  // -------------------------------
  // LCD pins configuration
  // -------------------------------
  // Configure PC14 (RS) and PC15 (E) as output push-pull
  GPIO_InitStruct.Pin = GPIO_PIN_14 | GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  // Configure PB8 (D4) and PB9 (D5) as output push-pull
  GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  // Configure PA12 (D6) and PA15 (D7) as output push-pull
  GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // Set all LCD pins LOW initially
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8 | GPIO_PIN_9, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12 | GPIO_PIN_15, GPIO_PIN_RESET);


  // -------------------------------
  // Button0 configuration (PA0)
  // -------------------------------
  GPIO_InitStruct.Pin = Button0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING; // Interrupt on rising edge
  GPIO_InitStruct.Pull = GPIO_PULLUP;         // Use pull-up resistor
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // Enable and set EXTI line 0 interrupt priority
  HAL_NVIC_SetPriority(EXTI0_IRQn, 2, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void EXTI0_IRQHandler(void){

	// TODO: Debounce using HAL_GetTick()


	// TODO: Disable DMA transfer and abort IT, then start DMA in IT mode with new LUT and re-enable transfer
	// HINT: Consider using C's "switch" function to handle LUT changes




	HAL_GPIO_EXTI_IRQHandler(Button0_Pin); // Clear interrupt flags
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
