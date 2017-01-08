#include "string.h"
#include "stdio.h"

#include "stm32l4xx.h"
#include "main.h"
#include "stm32l4xx_hal.h"
#include "ringbuffer.h"
#include "NEC_Decode.h"
#include "key.h"
#include "usb_host.h"
#include "usbh_hid.h"

// Functions
void Error_Handler();
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);
void MX_USB_HOST_Process(void);
static void NEC_IR_Init(void);

// Global Data
TIM_HandleTypeDef htim2;
DMA_HandleTypeDef hdma_tim2_ch1;

UART_HandleTypeDef huart3, huart2;
RingBuffer pcTxBuf, pcRxBuf, btTxBuf, btRxBuf;
char pcReadBuf[1], btReadBuf[1];
uint8_t pcTxData, btTxData;
__IO ITStatus PcUartReady = SET, BtUartReady = SET;

NEC nec;

int main(){
	HAL_Init();
	SystemClock_Config();
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_TIM2_Init();
	MX_USART3_UART_Init();
	MX_USART2_UART_Init();
    MX_USB_HOST_Init();
	NEC_IR_Init();

	// Set interrupt priority
	HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART2_IRQn);
	HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART3_IRQn);
	HAL_NVIC_SetPriority(OTG_FS_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(OTG_FS_IRQn);

	setbuf(stdout, NULL);

	char *welcome_string = "Welcome to control panel of BlueTooth module\r\n";
	printf("%s\r\n", welcome_string);

	int pos = 0;
	char cmdBuf[100], data;
	char *send_message = "Send: ";


	while(1){
		MX_USB_HOST_Process();

		if(PcUartReady){
			// Receive data from PC
			PcUartReady = RESET;
			data = pcReadBuf[0];
			HAL_UART_Receive_IT(&huart2, (uint8_t *)pcReadBuf, 1);

			// Process received data
			if(data == '\r' || data == '\n'){
				if(pos > 1){
					cmdBuf[pos] = 0;
					printf("%s%s\r\n",send_message, cmdBuf);
					cmdBuf[pos++] = '\r';
					cmdBuf[pos++] = '\n';
					RingBuffer_Write(&btTxBuf, (uint8_t *)cmdBuf, pos);
					HAL_UART_TxCpltCallback(&huart3);
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
			HAL_UART_Receive_IT(&huart3, (uint8_t *)btReadBuf, 1);

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
  if(huart->Instance == USART3){
	  // BT
	  BtUartReady = SET;
  }else if(huart->Instance == USART2){
	  // PC
	  PcUartReady = SET;
  }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
  // Check whether the transfer buffer is empty
  if(huart->Instance == USART3){
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
  if(huart->Instance == USART3){
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
 * Customized Initializer
 */
void myNecDecodedCallback(uint16_t address, uint8_t cmd) {
	Key key = KeySelect(cmd);
    printf("Key:%s (%2d)\r\n", key.keyshow, key.keyvalue);

	RingBuffer_Write(&btTxBuf, (uint8_t *)&key.keyvalue, 1);
	HAL_UART_TxCpltCallback(&huart3);
    HAL_Delay(10);
    NEC_Read(&nec);
}

void myNecErrorCallback() {
    printf("Error!\r\n");
    HAL_Delay(10);
    NEC_Read(&nec);
}

void myNecRepeatCallback() {
    printf("Repeat!\r\n");
    HAL_Delay(10);
    NEC_Read(&nec);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) {
    if (htim == &htim2) {
        NEC_TIM_IC_CaptureCallback(&nec);
    }
}
void NEC_IR_Init(void)
{
	nec.timerHandle = &htim2;
	nec.timerChannel = TIM_CHANNEL_1;
	nec.timerChannelActive = HAL_TIM_ACTIVE_CHANNEL_1;

    nec.timingBitBoundary = 1680 * 2;
    nec.timingAgcBoundary = 12500 * 2;
    nec.type = NEC_EXTENDED;

    nec.NEC_DecodedCallback = myNecDecodedCallback;
    nec.NEC_ErrorCallback = myNecErrorCallback;
    nec.NEC_RepeatCallback = myNecRepeatCallback;

    NEC_Read(&nec);
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
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40; // 10
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

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_USB;
  PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLLSAI1;
  PeriphClkInit.PLLSAI1.PLLSAI1Source = RCC_PLLSOURCE_MSI;
  PeriphClkInit.PLLSAI1.PLLSAI1M = 1;
  PeriphClkInit.PLLSAI1.PLLSAI1N = 24;
  PeriphClkInit.PLLSAI1.PLLSAI1P = RCC_PLLP_DIV7;
  PeriphClkInit.PLLSAI1.PLLSAI1Q = RCC_PLLQ_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1R = RCC_PLLR_DIV2;
  PeriphClkInit.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_48M2CLK;
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



/* TIM2 init function */
static void MX_TIM2_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_SlaveConfigTypeDef sSlaveConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_IC_InitTypeDef sConfigIC;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 39;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 0xffffffff;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }

  if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }


  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;
  sSlaveConfig.InputTrigger = TIM_TS_TI1FP1;
  sSlaveConfig.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  sSlaveConfig.TriggerFilter = 4;
  if (HAL_TIM_SlaveConfigSynchronization(&htim2, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_ENABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 4;
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

}

/* USART3 init function */
static void MX_USART3_UART_Init(void)
{

  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
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


/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 4, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

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

/******************************************
 * USB HID Callbacks
 */

void USBH_HID_EventCallback(USBH_HandleTypeDef *phost)
{
	HID_MOUSE_Info_TypeDef* mouse_info = NULL;
	HID_KEYBD_Info_TypeDef* keybd_info = NULL;
	// Check device type
	switch(USBH_HID_GetDeviceType(phost)){
	case HID_MOUSE:
		printf("[USB_MOUSE]");
		mouse_info = USBH_HID_GetMouseInfo(phost);
		if(mouse_info){
			printf("%3d, %3d, %d:%d:%d", mouse_info->x, mouse_info->y, mouse_info->buttons[0], mouse_info->buttons[1], mouse_info->buttons[2]);
		}else{
			printf("BUSY");
		}
	break;
	case HID_KEYBOARD: printf("[USB_KEYBOARD]");
	keybd_info = USBH_HID_GetKeybdInfo(phost);
	if(keybd_info){
		printf("STAT: %d", keybd_info->state);
	}else{
		printf("BUSY");
	}
	break;
	case HID_UNKNOWN: default: printf("[USB_UNKNOW]"); break;
	}

	printf("\n");
}

/*******************************************
 * Utility function
 */

void Error_Handler(){
	// TODO Handle Error
	while(1);
}
