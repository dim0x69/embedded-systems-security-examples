/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include <stdio.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* Non-secure Vector table to jump to (internal Flash Bank2 here)             */
/* Caution: address must correspond to non-secure internal Flash where is     */
/*          mapped in the non-secure vector table                             */
#define VTOR_TABLE_NS_START_ADDR  0x08040000UL
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

UART_HandleTypeDef hlpuart1;

RNG_HandleTypeDef hrng;

RTC_HandleTypeDef hrtc;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void NonSecure_Init(void);
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_GTZC_Init(void);
static void MX_RTC_Init(void);
static void MX_LPUART1_UART_Init(void);
static void MX_RNG_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

CMSE_NS_ENTRY uint32_t SECURE_GenerateRandom(){
	uint32_t random32bit;
	HAL_StatusTypeDef status = HAL_ERROR;
	status = HAL_RNG_GenerateRandomNumber(&hrng, &random32bit);
	while (status == HAL_ERROR){}
	return random32bit;
}

CMSE_NS_ENTRY void SECURE_TransmitRandomUart(uint32_t random32bit){
	char buff[50];
	sprintf(buff, "Random: %lu\r\n", random32bit);
	HAL_StatusTypeDef status = HAL_UART_Transmit(&hlpuart1, (uint8_t*) buff, strlen(buff), 1000);
	if (status != HAL_OK){
		Error_Handler();
	}
}

void TransmitCharPUart(char* text){
	 HAL_StatusTypeDef status = HAL_UART_Transmit(&hlpuart1, (uint8_t*) text, strlen(text), 1000);
	 if (status != HAL_OK){
		 Error_Handler();
	 }
}

void HAL_GTZC_TZIC_Callback(uint32_t periph){
		if(periph == GTZC_PERIPH_RNG){
			HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, 1);
			TransmitCharPUart("Security Violation in RNG\r\nReseting System now.\r\n");
			HAL_NVIC_SystemReset();
		}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* SAU/IDAU, FPU and interrupts secure/non-secure allocation setup done */
  /* in SystemInit() based on partition_stm32l552xx.h file's definitions. */
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* GTZC initialisation */
  MX_GTZC_Init();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_RTC_Init();
  MX_LPUART1_UART_Init();
  MX_RNG_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /*************** Setup and jump to non-secure *******************************/

  NonSecure_Init();

  /* Non-secure software does not return, this code is not executed */
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
  * @brief  Non-secure call function
  *         This function is responsible for Non-secure initialization and switch 
  *         to non-secure state
  * @retval None
  */
static void NonSecure_Init(void)
{
  funcptr_NS NonSecure_ResetHandler;

  SCB_NS->VTOR = VTOR_TABLE_NS_START_ADDR;

  /* Set non-secure main stack (MSP_NS) */
  __TZ_set_MSP_NS((*(uint32_t *)VTOR_TABLE_NS_START_ADDR));

  /* Get non-secure reset handler */
  NonSecure_ResetHandler = (funcptr_NS)(*((uint32_t *)((VTOR_TABLE_NS_START_ADDR) + 4U)));

  /* Start non-secure state software application */
  NonSecure_ResetHandler();
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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE0) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure LSE Drive Capability 
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_LSE
                              |RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 55;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GTZC Initialization Function
  * @param None
  * @retval None
  */
static void MX_GTZC_Init(void)
{

  /* USER CODE BEGIN GTZC_Init 0 */

  /* USER CODE END GTZC_Init 0 */

  MPCBB_ConfigTypeDef MPCBB1_NonSecureArea_Desc = {0};
  MPCBB_ConfigTypeDef MPCBB2_NonSecureArea_Desc = {0};

  /* USER CODE BEGIN GTZC_Init 1 */

  /* USER CODE END GTZC_Init 1 */
  if (HAL_GTZC_TZSC_ConfigPeriphAttributes(GTZC_PERIPH_LPUART1, GTZC_TZSC_PERIPH_SEC|GTZC_TZSC_PERIPH_NPRIV) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_GTZC_TZSC_ConfigPeriphAttributes(GTZC_PERIPH_VREFBUF, GTZC_TZSC_PERIPH_SEC|GTZC_TZSC_PERIPH_NPRIV) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_GTZC_TZSC_ConfigPeriphAttributes(GTZC_PERIPH_RNG, GTZC_TZSC_PERIPH_SEC|GTZC_TZSC_PERIPH_NPRIV) != HAL_OK)
  {
    Error_Handler();
  }
  MPCBB1_NonSecureArea_Desc.SecureRWIllegalMode = GTZC_MPCBB_SRWILADIS_ENABLE;
  MPCBB1_NonSecureArea_Desc.InvertSecureState = GTZC_MPCBB_INVSECSTATE_NOT_INVERTED;
  MPCBB1_NonSecureArea_Desc.AttributeConfig.MPCBB_SecConfig_array[0] =   0xFFFFFFFF;
  MPCBB1_NonSecureArea_Desc.AttributeConfig.MPCBB_SecConfig_array[1] =   0xFFFFFFFF;
  MPCBB1_NonSecureArea_Desc.AttributeConfig.MPCBB_SecConfig_array[2] =   0xFFFFFFFF;
  MPCBB1_NonSecureArea_Desc.AttributeConfig.MPCBB_SecConfig_array[3] =   0xFFFFFFFF;
  MPCBB1_NonSecureArea_Desc.AttributeConfig.MPCBB_SecConfig_array[4] =   0xFFFFFFFF;
  MPCBB1_NonSecureArea_Desc.AttributeConfig.MPCBB_SecConfig_array[5] =   0xFFFFFFFF;
  MPCBB1_NonSecureArea_Desc.AttributeConfig.MPCBB_SecConfig_array[6] =   0xFFFFFFFF;
  MPCBB1_NonSecureArea_Desc.AttributeConfig.MPCBB_SecConfig_array[7] =   0xFFFFFFFF;
  MPCBB1_NonSecureArea_Desc.AttributeConfig.MPCBB_SecConfig_array[8] =   0xFFFFFFFF;
  MPCBB1_NonSecureArea_Desc.AttributeConfig.MPCBB_SecConfig_array[9] =   0xFFFFFFFF;
  MPCBB1_NonSecureArea_Desc.AttributeConfig.MPCBB_SecConfig_array[10] =   0xFFFFFFFF;
  MPCBB1_NonSecureArea_Desc.AttributeConfig.MPCBB_SecConfig_array[11] =   0xFFFFFFFF;
  MPCBB1_NonSecureArea_Desc.AttributeConfig.MPCBB_SecConfig_array[12] =   0x00000000;
  MPCBB1_NonSecureArea_Desc.AttributeConfig.MPCBB_SecConfig_array[13] =   0x00000000;
  MPCBB1_NonSecureArea_Desc.AttributeConfig.MPCBB_SecConfig_array[14] =   0x00000000;
  MPCBB1_NonSecureArea_Desc.AttributeConfig.MPCBB_SecConfig_array[15] =   0x00000000;
  MPCBB1_NonSecureArea_Desc.AttributeConfig.MPCBB_SecConfig_array[16] =   0x00000000;
  MPCBB1_NonSecureArea_Desc.AttributeConfig.MPCBB_SecConfig_array[17] =   0x00000000;
  MPCBB1_NonSecureArea_Desc.AttributeConfig.MPCBB_SecConfig_array[18] =   0x00000000;
  MPCBB1_NonSecureArea_Desc.AttributeConfig.MPCBB_SecConfig_array[19] =   0x00000000;
  MPCBB1_NonSecureArea_Desc.AttributeConfig.MPCBB_SecConfig_array[20] =   0x00000000;
  MPCBB1_NonSecureArea_Desc.AttributeConfig.MPCBB_SecConfig_array[21] =   0x00000000;
  MPCBB1_NonSecureArea_Desc.AttributeConfig.MPCBB_SecConfig_array[22] =   0x00000000;
  MPCBB1_NonSecureArea_Desc.AttributeConfig.MPCBB_SecConfig_array[23] =   0x00000000;
  MPCBB1_NonSecureArea_Desc.AttributeConfig.MPCBB_LockConfig_array[0] =   0x00000000;
  if (HAL_GTZC_MPCBB_ConfigMem(SRAM1_BASE, &MPCBB1_NonSecureArea_Desc) != HAL_OK)
  {
    Error_Handler();
  }
  MPCBB2_NonSecureArea_Desc.SecureRWIllegalMode = GTZC_MPCBB_SRWILADIS_ENABLE;
  MPCBB2_NonSecureArea_Desc.InvertSecureState = GTZC_MPCBB_INVSECSTATE_NOT_INVERTED;
  MPCBB2_NonSecureArea_Desc.AttributeConfig.MPCBB_SecConfig_array[0] =   0x00000000;
  MPCBB2_NonSecureArea_Desc.AttributeConfig.MPCBB_SecConfig_array[1] =   0x00000000;
  MPCBB2_NonSecureArea_Desc.AttributeConfig.MPCBB_SecConfig_array[2] =   0x00000000;
  MPCBB2_NonSecureArea_Desc.AttributeConfig.MPCBB_SecConfig_array[3] =   0x00000000;
  MPCBB2_NonSecureArea_Desc.AttributeConfig.MPCBB_SecConfig_array[4] =   0x00000000;
  MPCBB2_NonSecureArea_Desc.AttributeConfig.MPCBB_SecConfig_array[5] =   0x00000000;
  MPCBB2_NonSecureArea_Desc.AttributeConfig.MPCBB_SecConfig_array[6] =   0x00000000;
  MPCBB2_NonSecureArea_Desc.AttributeConfig.MPCBB_SecConfig_array[7] =   0x00000000;
  MPCBB2_NonSecureArea_Desc.AttributeConfig.MPCBB_LockConfig_array[0] =   0x00000000;
  if (HAL_GTZC_MPCBB_ConfigMem(SRAM2_BASE, &MPCBB2_NonSecureArea_Desc) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_GTZC_TZIC_EnableIT(GTZC_PERIPH_RNG) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_GTZC_TZIC_EnableIT(GTZC_PERIPH_FLASH) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN GTZC_Init 2 */

  /* USER CODE END GTZC_Init 2 */

}

/**
  * @brief LPUART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_LPUART1_UART_Init(void)
{

  /* USER CODE BEGIN LPUART1_Init 0 */

  /* USER CODE END LPUART1_Init 0 */

  /* USER CODE BEGIN LPUART1_Init 1 */

  /* USER CODE END LPUART1_Init 1 */
  hlpuart1.Instance = LPUART1;
  hlpuart1.Init.BaudRate = 209700;
  hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
  hlpuart1.Init.StopBits = UART_STOPBITS_1;
  hlpuart1.Init.Parity = UART_PARITY_NONE;
  hlpuart1.Init.Mode = UART_MODE_TX_RX;
  hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  hlpuart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  hlpuart1.FifoMode = UART_FIFOMODE_DISABLE;
  if (HAL_UART_Init(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&hlpuart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&hlpuart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LPUART1_Init 2 */

  /* USER CODE END LPUART1_Init 2 */

}

/**
  * @brief RNG Initialization Function
  * @param None
  * @retval None
  */
static void MX_RNG_Init(void)
{

  /* USER CODE BEGIN RNG_Init 0 */

  /* USER CODE END RNG_Init 0 */

  /* USER CODE BEGIN RNG_Init 1 */

  /* USER CODE END RNG_Init 1 */
  hrng.Instance = RNG;
  hrng.Init.ClockErrorDetection = RNG_CED_ENABLE;
  if (HAL_RNG_Init(&hrng) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RNG_Init 2 */

  /* USER CODE END RNG_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_PrivilegeStateTypeDef privilegeState = {0};
  RTC_SecureStateTypeDef secureState = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only 
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  hrtc.Init.OutPutPullUp = RTC_OUTPUT_PULLUP_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  privilegeState.rtcPrivilegeFull = RTC_PRIVILEGE_FULL_NO;
  privilegeState.backupRegisterPrivZone = RTC_PRIVILEGE_BKUP_ZONE_NONE;
  privilegeState.backupRegisterStartZone2 = RTC_BKP_DR0;
  privilegeState.backupRegisterStartZone3 = RTC_BKP_DR0;
  if (HAL_RTCEx_PrivilegeModeSet(&hrtc, &privilegeState) != HAL_OK)
  {
    Error_Handler();
  }
  secureState.rtcSecureFull = RTC_SECURE_FULL_YES;
  secureState.backupRegisterStartZone2 = RTC_BKP_DR0;
  secureState.backupRegisterStartZone3 = RTC_BKP_DR0;
  if (HAL_RTCEx_SecureModeSet(&hrtc, &secureState) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

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
  __HAL_RCC_GPIOG_CLK_ENABLE();
  HAL_PWREx_EnableVddIO2();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_RESET);

  /*IO attributes management functions */
  HAL_GPIO_ConfigPinAttributes(GPIOC, GPIO_PIN_13|VBUS_SENSE_Pin, GPIO_PIN_NSEC);

  /*Configure GPIO pin : LED_GREEN_Pin */
  GPIO_InitStruct.Pin = LED_GREEN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GREEN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LED_RED_Pin */
  GPIO_InitStruct.Pin = LED_RED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_RED_GPIO_Port, &GPIO_InitStruct);

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
