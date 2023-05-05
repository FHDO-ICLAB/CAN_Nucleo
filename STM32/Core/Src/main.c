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
#include<stdlib.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
CAN_HandleTypeDef hcan1;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

void CAN_Mode(uint8_t);
void CAN_SJW(uint8_t);
void CAN_TSeg1(uint8_t);
void CAN_TSeg2(uint8_t);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
CAN_TxHeaderTypeDef TxHeader; //for transmitting data
CAN_RxHeaderTypeDef RxHeader; // checking data after receive

uint32_t TxMailbox;//creating a mailbox to send data

//creating buffer for transmit and receive
//uint8_t UART_TxData[8];
//uint8_t CAN_TxData[8];
uint8_t UART_RxData[14];
//uint8_t CAN_RxData[8];
//uint8_t *Data;
//uint8_t count=0;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{


	//specifying the header

	if (UART_RxData[0]==2){
		//specifying the header

		TxHeader.DLC=UART_RxData[2];
		TxHeader.IDE=(UART_RxData[13]==1)?CAN_ID_STD:CAN_ID_EXT;
		if (TxHeader.IDE==CAN_ID_STD)
		{
			TxHeader.StdId=UART_RxData[1];
		}else{
			TxHeader.ExtId=UART_RxData[1];
		}
		TxHeader.RTR=(UART_RxData[11]==1)?CAN_RTR_DATA:CAN_RTR_REMOTE;
		TxHeader.TransmitGlobalTime=(UART_RxData[12]==1)?ENABLE:DISABLE;

		uint8_t *Data=malloc(UART_RxData[2]*sizeof(uint8_t));
		for (int i=0;i<UART_RxData[2];i++)
		{
			Data[i]=UART_RxData[i+3];
		}
		HAL_CAN_AddTxMessage(&hcan1, &TxHeader, Data, &TxMailbox);
	} else if (UART_RxData[0]==1){
		hcan1.Init.Prescaler = UART_RxData[1];
		/*
		hcan1.Init.Mode = CAN_MODE_LOOPBACK;
		hcan1.Init.SyncJumpWidth = CAN_SJW_3TQ;
		hcan1.Init.TimeSeg1 = CAN_BS1_4TQ;
		hcan1.Init.TimeSeg2 = CAN_BS2_5TQ;
		*/
		CAN_Mode(UART_RxData[2]);
		CAN_SJW(UART_RxData[3]);
		CAN_TSeg1(UART_RxData[4]);
		CAN_TSeg2(UART_RxData[5]);

		HAL_UART_Receive_IT(&huart2, UART_RxData, sizeof(UART_RxData));
	}


}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	//uint8_t *CAN_RxData;
	uint8_t *CAN_RxData=malloc((UART_RxData[1])*sizeof(uint8_t));
	//uint8_t CAN_RxData[8];
	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, CAN_RxData);
	//UART_TxData=CAN_RxData;
	//CAN_RxData[7]+=1;
	CAN_RxData[3]=0xa1;
	HAL_UART_Transmit(&huart2,CAN_RxData,8,1000);//sizeof(CAN_RxData)
	HAL_UART_Receive_IT(&huart2, UART_RxData, sizeof(UART_RxData)); //restarting the interrupt reception mode
	//RxData[7]+=1;
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
  MX_CAN1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  HAL_CAN_Start(&hcan1);

  //activating UART Receive Interrupt
  HAL_UART_Receive_IT(&huart2, UART_RxData, sizeof(UART_RxData));

  HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING); //activating CAN Receive interrupt

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

void CAN_Mode(uint8_t ModeID)
{
	switch(ModeID)
	{
	case 0:
		hcan1.Init.Mode =CAN_MODE_NORMAL;
		break;
	case 1:
		hcan1.Init.Mode =CAN_MODE_LOOPBACK;
		break;
	case 2:
		hcan1.Init.Mode =CAN_MODE_SILENT;
		break;
	case 3:
		hcan1.Init.Mode =CAN_MODE_SILENT_LOOPBACK;
		break;
	//default:
	//	printf("Error CAN Mode !!");
	}
}

void CAN_TSeg1(uint8_t TSeg1ID)
{
	switch(TSeg1ID)
	{
	case 1:
		hcan1.Init.TimeSeg1 = CAN_BS1_1TQ;
		break;
	case 2:
		hcan1.Init.TimeSeg1 = CAN_BS1_2TQ;
		break;
	case 3:
		hcan1.Init.TimeSeg1 = CAN_BS1_3TQ;
		break;
	case 4:
		hcan1.Init.TimeSeg1 = CAN_BS1_4TQ;
		break;
	case 5:
		hcan1.Init.TimeSeg1 = CAN_BS1_5TQ;
		break;
	case 6:
		hcan1.Init.TimeSeg1 = CAN_BS1_6TQ;
		break;
	case 7:
		hcan1.Init.TimeSeg1 = CAN_BS1_7TQ;
		break;
	case 8:
		hcan1.Init.TimeSeg1 = CAN_BS1_8TQ;
		break;
	case 9:
		hcan1.Init.TimeSeg1 = CAN_BS1_9TQ;
		break;
	case 10:
		hcan1.Init.TimeSeg1 = CAN_BS1_10TQ;
		break;
	case 11:
		hcan1.Init.TimeSeg1 = CAN_BS1_11TQ;
		break;
	case 12:
		hcan1.Init.TimeSeg1 = CAN_BS1_12TQ;
		break;
	case 13:
		hcan1.Init.TimeSeg1 = CAN_BS1_13TQ;
		break;
	case 14:
		hcan1.Init.TimeSeg1 = CAN_BS1_14TQ;
		break;
	case 15:
		hcan1.Init.TimeSeg1 = CAN_BS1_15TQ;
		break;
	case 16:
		hcan1.Init.TimeSeg1 = CAN_BS1_16TQ;
		break;
	}

}
void CAN_TSeg2(uint8_t TSeg2ID)
{
	switch(TSeg2ID)
	{
	case 1:
		hcan1.Init.TimeSeg2 = CAN_BS2_1TQ;
		break;
	case 2:
		hcan1.Init.TimeSeg2 = CAN_BS2_2TQ;
		break;
	case 3:
		hcan1.Init.TimeSeg2 = CAN_BS2_3TQ;
		break;
	case 4:
		hcan1.Init.TimeSeg2 = CAN_BS2_4TQ;
		break;
	case 5:
		hcan1.Init.TimeSeg2 = CAN_BS2_5TQ;
		break;
	case 6:
		hcan1.Init.TimeSeg2 = CAN_BS2_6TQ;
		break;
	case 7:
		hcan1.Init.TimeSeg2 = CAN_BS2_7TQ;
		break;
	case 8:
		hcan1.Init.TimeSeg2 = CAN_BS2_8TQ;
		break;
	}

}
void CAN_SJW(uint8_t SJW)
{
	switch(SJW)
	{
	case 1:
		hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
		break;
	case 2:
		hcan1.Init.SyncJumpWidth = CAN_SJW_2TQ;
		break;
	case 3:
		hcan1.Init.SyncJumpWidth = CAN_SJW_3TQ;
		break;
	case 4:
		hcan1.Init.SyncJumpWidth = CAN_SJW_4TQ;
		break;
	}

}
/*
void CAN_TSeg1_test(uint8_t TSeg1ID)
{
	hcan1.Init.TimeSeg1 = "CAN_BS1_" + TSeg1ID + "TQ";
}
*/
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
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 20;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
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
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV4;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief CAN1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN1_Init(void)
{

  /* USER CODE BEGIN CAN1_Init 0 */

  /* USER CODE END CAN1_Init 0 */

  /* USER CODE BEGIN CAN1_Init 1 */

  /* USER CODE END CAN1_Init 1 */
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 10;
  hcan1.Init.Mode = CAN_MODE_LOOPBACK; //CAN_MODE_NORMAL CAN_MODE_LOOPBACK
  hcan1.Init.SyncJumpWidth = CAN_SJW_3TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_4TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_5TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN1_Init 2 */

  /* USER CODE END CAN1_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */
  // CAN Filter Configuration
     CAN_FilterTypeDef canfilterconfig;

     canfilterconfig.FilterActivation=CAN_FILTER_ENABLE;//CAN_FILTER_DISABLE or CAN_FILTER_ENABLE
     canfilterconfig.FilterBank=10;
     canfilterconfig.FilterFIFOAssignment=CAN_RX_FIFO0;
     canfilterconfig.FilterIdHigh=0;//0x100<<5
     canfilterconfig.FilterIdLow=0x0000;
     canfilterconfig.FilterMaskIdHigh=0;//0x100<<5
     canfilterconfig.FilterMaskIdLow=0x0000;
     canfilterconfig.FilterMode=CAN_FILTERMODE_IDMASK;
     canfilterconfig.FilterScale=CAN_FILTERSCALE_32BIT;
     canfilterconfig.SlaveStartFilterBank=0;

     //calling the filter configuration function
     HAL_CAN_ConfigFilter(&hcan1, &canfilterconfig);
  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
