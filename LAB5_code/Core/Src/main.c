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
#include "stdio.h"
#include "string.h"
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
UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart2_tx;

/* USER CODE BEGIN PV */
uint8_t RxBuffer[20]; //start_bit+massage_bit+(stop_bit x2) //add another 1 bit for make sure that have enough space for bits
uint8_t TxBuffer[60];
uint8_t text1[200];
uint8_t text2[200];
uint8_t text3[60];
uint8_t text4[21];
uint8_t text5[55];
//float time_to_trick ;
int count_hz = 0;
int x;
int j = 1;
//int count_hz ;
//int x;
//uint32_t count_hz;
//int time_to_trick ;
//int time_sec_hz;
//int time_to_trick ;
//int time_sec_hz ;
//int x;


//int mode = 0;
//uint32_t control_hz;
int8_t p;
int8_t on_off = 1;
uint32_t time_to_trick;
uint8_t state;
//uint8_t count_hz = 0;
//uint32_t time_to_trick;

enum Status{select_mode,LED_Control,button_status};

enum Status status = select_mode;



//uint32_t mode = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void Dummytask();
void uartdmaconfig();
void mode();
void button_loop();
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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
//uint8_t text[] = "HELLO BEST";
//HAL_UART_Transmit(&huart2, text, 11, 10);
//sprintf((char*)TxBuffer,"  Choose your menu\r\n-----------------\r\n{0} LED Control\r\n{1} Button Status\r\n-------------------\r\n\r\n\r\n\r\n\r\n");
//HAL_UART_Transmit_IT(&huart2, TxBuffer, strlen((char*)TxBuffer));
//uartdmaconfig();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  sprintf((char*)text1,"Choose your menu\n\r*******************\r\n(0) LED Control\r\n(1) Button Status\r\n*******************\r\nAnd Don't Forget it!!!!\r\n\r\n\r\n\r\n");
  HAL_UART_Transmit_IT( &huart2, text1, strlen((char*)text1));
  //uartdmaconfig();
  while (1)
  {
	  //mode();
	  Dummytask();
//	  uartdmaconfig();
	  uartdmaconfig();
	  HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);
//	  mode();
	  //uartdmaconfig();
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
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
  huart2.Init.BaudRate = 230400;
  huart2.Init.WordLength = UART_WORDLENGTH_9B;
  huart2.Init.StopBits = UART_STOPBITS_2;
  huart2.Init.Parity = UART_PARITY_EVEN;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  /* DMA1_Stream6_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream6_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream6_IRQn);

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
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PC13 */
  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PA0 LD2_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_0|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void Dummytask()
{
//		if (x<=0){
//			x = 0;
//			count_hz = 0;
//		}
		uint32_t time_sec_hz ;
		uint32_t time_to_trick;
		time_sec_hz = 1000/count_hz;
		time_to_trick = time_sec_hz/2;
		static uint32_t timestamp=0;
			if(HAL_GetTick()>=timestamp && on_off == 1 ){
				timestamp = HAL_GetTick()+ time_to_trick; //+100 = 5hz because on&off used 200ms ->1000/200 s = 5 cycle = 5hz
				//timestamp = HAL_GetTick()+ 500;
				HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
//				if(p != 1 ){
//					sprintf((char*)TxBuffer,"On LED now.. \r\n ");
//					//p = on_off;
//					HAL_UART_Transmit_DMA(&huart2, TxBuffer, strlen((char*)TxBuffer));
//				}
				//p = on_off;
			}
			else if (on_off != 1) {
//				p = on_off;
				HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5 , GPIO_PIN_RESET);
				sprintf((char*)TxBuffer,"Sorry LED out of service right now... \r\n ");
//				p = on_off;
				HAL_UART_Transmit_DMA(&huart2, TxBuffer, strlen((char*)TxBuffer));
				//p = on_off;
			}
//			p = on_off;
	}

//void uartdmaconfig(){
//	HAL_UART_Receive_DMA(&huart2, RxBuffer, 1);
//
//}

void uartdmaconfig(){
	//start  UART in DMA Mode
	HAL_UART_Receive_DMA(&huart2, RxBuffer, 1);
	state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);

	if (RxBuffer[0] == '1' ){
			state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
			if(!state){
				//sprintf((char*)TxBuffer,"Received : Press \r\n");
				//HAL_UART_Transmit_IT(&huart2, TxBuffer, strlen((char*)TxBuffer));
				sprintf((char*)TxBuffer,"Received :   press \r\n");
				HAL_UART_Transmit_DMA(&huart2, TxBuffer, strlen((char*)TxBuffer));
			}
			else if(state){
				sprintf((char*)TxBuffer,"Received : Unpress \r\n");
				HAL_UART_Transmit_DMA(&huart2, TxBuffer, strlen((char*)TxBuffer));
			}if(RxBuffer[0] == 'x'){
				if( TxBuffer[20] == '\n' && TxBuffer[11]=='M'){
//				sprintf((char*)TxBuffer,"Back                                            ");
//				HAL_UART_Transmit_IT(&huart2, TxBuffer, strlen((char*)TxBuffer));
//				sprintf((char*)TxBuffer,"Select Menu 1/0 \r\n");
//				HAL_UART_Transmit_IT(&huart2, TxBuffer, strlen((char*)TxBuffer));
					TxBuffer[0] = '\000';TxBuffer[1] = '\000';TxBuffer[2] = '\000';TxBuffer[3] = '\000';TxBuffer[4] = '\000';TxBuffer[5] = '\000';TxBuffer[6] = '\000';TxBuffer[7] = '\000';TxBuffer[8] = '\000';TxBuffer[9] = '\000';TxBuffer[10] = '\000';TxBuffer[11] = '\000';TxBuffer[12] = '\000';TxBuffer[13] = '\000';TxBuffer[14] = '\000';TxBuffer[15] = '\000';TxBuffer[16] = '\000';TxBuffer[17] = '\000';TxBuffer[18] = '\000';TxBuffer[19] = '\000';TxBuffer[20] = '\000';TxBuffer[21] = '\000';TxBuffer[22] = '\000';TxBuffer[23] = '\000';TxBuffer[24] = '\000';TxBuffer[25] = '\000';TxBuffer[26] = '\000';TxBuffer[27] = '\000';TxBuffer[28] = '\000';TxBuffer[29] = '\000';TxBuffer[30]  = '\000';
					//HAL_UART_Transmit_IT(&huart2, TxBuffer, strlen((char*)TxBuffer));}
//					sprintf((char*)text2,"Choose your menu\n\r-----------------\r\n{0} LED Control\r\n{1} Button Status\r\n-------------------\r\nAnd Don't Forget it!!!!\r\n\r\n\r\n\r\n");
//					HAL_UART_Transmit_IT( &huart2, text2, strlen((char*)text2));
					status = select_mode;}
			}
//				sprintf((char*)TxBuffer,"Back                                            ");
//				HAL_UART_Transmit_IT(&huart2, TxBuffer, strlen((char*)TxBuffer));}

//			}
		}

	}




//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//	if(huart == &huart2)
//	{
//		RxBuffer[2] = '\0';
//
//		HAL_UART_Transmit_IT(&huart2, TxBuffer, strlen((char*)TxBuffer));
//
//		HAL_UART_Receive_IT(&huart2, RxBuffer, 1);
//	}
//}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
//	 sprintf((char*)text1,"  Choose your menu\r\n-----------------\r\n{0} LED Control\r\n{1} Button Status\r\n-------------------\r\n\r\n\r\n\r\n\r\n");
//	 HAL_UART_Transmit_IT( &huart2, text1, strlen((char*)text1));

		switch(status){
//		sprintf((char*)text2,"Choose your menu\n\r-----------------\r\n{0} LED Control\r\n{1} Button Status\r\n-------------------\r\nAnd Don't Forget it!!!!\r\n\r\n\r\n\r\n");
//						HAL_UART_Transmit_IT( &huart2, text2, strlen((char*)text2));
			case select_mode:
//				sprintf((char*)TxBuffer,"Received : Menu    \r\n");
//				HAL_UART_Transmit_IT(&huart2, TxBuffer, strlen((char*)TxBuffer));
//				sprintf((char*)text2,"Choose your menu\n\r-----------------\r\n{0} LED Control\r\n{1} Button Status\r\n-------------------\r\nAnd Don't Forget it!!!!\r\n\r\n\r\n\r\n");
//				HAL_UART_Transmit_IT( &huart2, text2, strlen((char*)text2));
				j=0;
				TxBuffer[0] = 0;TxBuffer[1] = '\000';TxBuffer[2] = '\000';TxBuffer[3] = '\000';TxBuffer[4] = '\000';TxBuffer[5] = '\000';TxBuffer[6] = '\000';TxBuffer[7] = '\000';TxBuffer[8] = '\000';TxBuffer[9] = '\000';TxBuffer[10] = '\000';TxBuffer[11] = '\000';TxBuffer[12] = '\000';TxBuffer[13] = '\000';TxBuffer[14] = '\000';TxBuffer[15] = '\000';TxBuffer[16] = '\000';TxBuffer[17] = '\000';TxBuffer[18] = '\000';TxBuffer[19] = '\000';TxBuffer[20] = '\000';TxBuffer[21] = '\000';TxBuffer[22] = '\000';TxBuffer[23] = '\000';TxBuffer[24] = '\000';TxBuffer[25] = '\000';TxBuffer[26] = '\000';TxBuffer[27] = '\000';TxBuffer[28] = '\000';TxBuffer[29] = '\000';TxBuffer[30]  = '\000';
				if (j == 0 && RxBuffer[0]=='x' ){
					//TxBuffer = [];
					TxBuffer[0] = '\000';TxBuffer[1] = '\000';TxBuffer[2] = '\000';TxBuffer[3] = '\000';TxBuffer[4] = '\000';TxBuffer[5] = '\000';TxBuffer[6] = '\000';TxBuffer[7] = '\000';TxBuffer[8] = '\000';TxBuffer[9] = '\000';TxBuffer[10] = '\000';TxBuffer[11] = '\000';TxBuffer[12] = '\000';TxBuffer[13] = '\000';TxBuffer[14] = '\000';TxBuffer[15] = '\000';TxBuffer[16] = '\000';TxBuffer[17] = '\000';TxBuffer[18] = '\000';TxBuffer[19] = '\000';TxBuffer[20] = '\000';TxBuffer[21] = '\000';TxBuffer[22] = '\000';TxBuffer[23] = '\000';TxBuffer[24] = '\000';TxBuffer[25] = '\000';TxBuffer[26] = '\000';TxBuffer[27] = '\000';TxBuffer[28] = '\000';TxBuffer[29] = '\000';TxBuffer[30]  = '\000';
//					sprintf((char*)TxBuffer,"Received : Menu    \r\n");
//					HAL_UART_Transmit_IT(&huart2, TxBuffer, strlen((char*)TxBuffer));
					sprintf((char*)text2,"\r\n\r\n\r\nChoose your menu\n\r*******************\r\n(0) LED Control\r\n(1) Button Status\r\n*******************\r\nAnd Don't Forget it!!!!\r\n\r\n\r\n\r\n");
					HAL_UART_Transmit_IT( &huart2, text2, strlen((char*)text2));
					RxBuffer[0] = '\000';
				}

				if(RxBuffer[0] == '0' ){
					status = LED_Control;
					sprintf((char*)TxBuffer,"Received : LED_CONTROL \r\n");
					HAL_UART_Transmit_DMA(&huart2, TxBuffer, strlen((char*)TxBuffer));
					RxBuffer[0] = '\000';
//				}else if(RxBuffer[0] == '1' ){
//					status = button_status;
//					sprintf((char*)TxBuffer,"Received : BUTTON_STATUS \r\n");
//					HAL_UART_Transmit_IT(&huart2, TxBuffer, strlen((char*)TxBuffer));
//					RxBuffer[0] = '\000';
				}else if(RxBuffer[0] != '0' && RxBuffer[0] != '1' && RxBuffer[0] != '\000' && RxBuffer[0] != '\0'  ){
					RxBuffer[0] = '\000';
					sprintf((char*)TxBuffer,"\n\rPlease Select menu again\r\n");//please type your selection again!!!
					HAL_UART_Transmit_IT(&huart2, TxBuffer, strlen((char*)TxBuffer));
				}
				break;

			case LED_Control:

				if(RxBuffer[0] == 'a'){
					count_hz += 1;
					x = count_hz;
//					if (x>=0){
//						x = 30;
//					}
//					x = count_hz;

					sprintf((char*)TxBuffer,"Received speed right Now!!! LED_ON %d\r\n",x);
					HAL_UART_Transmit_DMA(&huart2, TxBuffer, strlen((char*)TxBuffer));
				}else if(RxBuffer[0] == 's'){
					count_hz -= 1;
					x = count_hz;
					if (x<0||count_hz<0){
						x = 0;
						count_hz = 0;
					}
//					if (x<=0){
//						x = 0;
//						count_hz = 0;
//					}
					sprintf((char*)TxBuffer,"Received speed right Now!!! LED_ON %d\r\n",x);
					HAL_UART_Transmit_DMA(&huart2, TxBuffer, strlen((char*)TxBuffer));
				}
					else if(RxBuffer[0] == 'd'){
						on_off *= -1;
						RxBuffer[0] = '\000';
//						if (on_off == 1 && p == 0){
//					}

//						sprintf((char*)TxBuffer,"\r\n\r\n\r\n\r\n\r\n\r\nReceived speed right Now!!! %d\r\n",x);
//						HAL_UART_Transmit_DMA(&huart2, TxBuffer, strlen((char*)TxBuffer));
					}
				else if(RxBuffer[0] == 'x'){
					sprintf((char*)TxBuffer,"Received : Back to Menu \r\n");
					HAL_UART_Transmit_DMA(&huart2, TxBuffer, strlen((char*)TxBuffer));
					status = select_mode;
					RxBuffer[0] = '\000';
				}else if(RxBuffer[0] != 'a' && RxBuffer[0] != 's' && RxBuffer[0] != 'd' && RxBuffer[0] != 'x' && RxBuffer[0] != '\000' && RxBuffer[0] != '\0'){
					RxBuffer[0] = '\000';
					sprintf((char*)TxBuffer,"Received : error  \r\n");
					HAL_UART_Transmit_DMA(&huart2, TxBuffer, strlen((char*)TxBuffer));
				}
				break;
		}
//				sprintf((char*)TxBuffer,"Received : hi \r\n");
//				HAL_UART_Transmit_IT(&huart2, TxBuffer, strlen((char*)TxBuffer));}

//			case button_status:
////				uint8_t text2[52];
////				uint8_t text3[46];
////				uint8_t text4[20];
//
//				state = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
//				if(state == 0){
//					sprintf((char*)TxBuffer,"Received : You Press the button right now!!! \r\n");
//					HAL_UART_Transmit_IT(&huart2, TxBuffer, strlen((char*)TxBuffer));
//				}
//				else if(state == 1){
//					sprintf((char*)TxBuffer,"Received : You Un_press the button...  \r\n");
//					HAL_UART_Transmit_IT(&huart2, TxBuffer, strlen((char*)TxBuffer));;
//				}if(RxBuffer[0] == 'x'){
//					sprintf((char*)TxBuffer,"Received : Menu \r\n");
//					HAL_UART_Transmit_IT(&huart2, TxBuffer, strlen((char*)TxBuffer));
//					status = select_mode;
//					RxBuffer[0] = '\000';
//				}else if(RxBuffer[0] != 'x' && RxBuffer[0] != '\000' && RxBuffer[0] != '\0'){
//					RxBuffer[0] = '\000';
//					sprintf((char*)TxBuffer,"Received : error \r\n");
//					HAL_UART_Transmit_IT(&huart2, TxBuffer, strlen((char*)TxBuffer));
//					//TxBuffer=0;
//				}
//				break;
//		}
	}


//}




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
