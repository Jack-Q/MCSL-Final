#include "string.h"
#include "stdio.h"

#include "stm32l4xx.h"
#include "main.h"
#include "stm32l4xx_hal.h"
#include "ringbuffer.h"


// Functions
void Error_Handler();
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);

// Global Data
UART_HandleTypeDef huart1, huart2;
RingBuffer pcTxBuf, pcRxBuf, btTxBuf, btRxBuf;
char pcReadBuf[1], btReadBuf[1];
uint8_t pcTxData, btTxData;
__IO ITStatus PcUartReady = SET, BtUartReady = SET;

int main(){
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_USART1_UART_Init();
	MX_USART2_UART_Init();

	// Set interrupt priority
	HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART2_IRQn);
	HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);



	setbuf(stdout, NULL);

	char *welcome_string = "Welcome to control panel of BlueTooth module\r\n";
	printf("%s\r\n", welcome_string);

	int pos = 0;
	char cmdBuf[100], data;
	char *send_message = "Send: ";

	while(1){
		if(PcUartReady){
			// Receive data from PC
			PcUartReady = RESET;
			data = pcReadBuf[0];
			HAL_UART_Receive_IT(&huart2, (uint8_t *)pcReadBuf, 1);

			// Process received data
			if(data == '\r' || data == '\n'){
				if(pos > 1){
					cmdBuf[pos] = 0;
					printf("%s%s\n",send_message, cmdBuf);
					RingBuffer_Write(&btTxBuf, (uint8_t *)cmdBuf, pos);
					HAL_UART_TxCpltCallback(&huart1);
					pos = 0;
				}
			}else{
				cmdBuf[pos++] = data;
			}

		}
		if(BtUartReady){
			// Receive data from BlueTooth Module
			BtUartReady = RESET;
			data = btReadBuf[0];
			HAL_UART_Receive_IT(&huart1, (uint8_t *)btReadBuf, 1);

			// Process received data
			printf("%c", data);
		}
	}
}


int __io_putchar(int ch){
	if(HAL_UART_Transmit_IT(&huart2, (uint8_t *) &ch, 1) != HAL_OK)
		RingBuffer_Write(&pcTxBuf, (uint8_t *) &ch, 1);
	return ch;
}

// Interrupt handler
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  /* Set transmission flag: transfer complete*/
  if(huart->Instance == USART1){
	  // BT
	  BtUartReady = SET;
  }else if(huart->Instance == USART2){
	  // PC
	  PcUartReady = SET;
  }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
  // Check whether the transfer buffer is empty
  if(huart->Instance == USART1){
	  // BT
	  if(RingBuffer_GetDataLength(&btTxBuf) > 0) {
	    RingBuffer_Read(&btTxBuf, &btTxData, 1);
	    HAL_UART_Transmit_IT(huart, &btTxData, 1);
	  }
  }else if(huart->Instance == USART2){
	  // PC
	  if(RingBuffer_GetDataLength(&pcTxBuf) > 0) {
	    RingBuffer_Read(&pcTxBuf, &pcTxData, 1);
	    HAL_UART_Transmit_IT(huart, &pcTxData, 1);
	  }
  }

}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
  if(huart->Instance == USART1){
	  // BT
	  if(huart->ErrorCode == HAL_UART_ERROR_ORE)
	    HAL_UART_Receive_IT(huart, (uint8_t *)btReadBuf, 1);
  }else if(huart->Instance == USART2){
	  // PC
	  if(huart->ErrorCode == HAL_UART_ERROR_ORE)
	    HAL_UART_Receive_IT(huart, (uint8_t *)pcReadBuf, 1);
  }
}


/*******************************************
 * Initialization
 */


/* System Clock Configuration */
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

    /**Initializes the CPU, AHB and APB busses clocks
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks
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

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure the main internal regulator output voltage
    */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure the Systick interrupt time
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}


/* USART1 init function */
static void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

}

/* USART2 init function */
static void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
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

}

/* GPIO for LCD and User Button */
static void MX_GPIO_Init(void)
{
	/** Configure pins as
	        * Analog
	        * Input
	        * Output
	        * EVENT_OUT
	        * EXTI
	*/


  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

}

/*******************************************
 * Utility function
 */

void Error_Handler(){
	// TODO Handle Error
	while(1);
}
