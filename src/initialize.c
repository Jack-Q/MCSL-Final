#include "global_state.h"
#include "initialize.h"
#include "lcd.h"
#include "stm32l4xx.h"
#include "string.h"
#include "usb_host.h"
#include "ctrl_time.h"

extern CTRL_status_t global_status;

extern UART_HandleTypeDef huart3, huart2;
extern RingBuffer pcTxBuf, pcRxBuf, btTxBuf, btRxBuf;
extern char pcReadBuf[1], btReadBuf[1];
extern uint8_t pcTxData, btTxData;
extern __IO ITStatus PcUartReady, BtUartReady;

extern NEC nec;

void Error_Handler();
static void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM2_Init(void);
static void NEC_IR_Init(void);
static void Init_global_state(void);
void IR_receive_callback(uint16_t, uint8_t);
void IR_repeat_callback();
void IR_error_callback();
void IR_receive_key(Key key);

void initialize()
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    LCD_init();
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

    Init_global_state();
    CTRL_timeReset();
}

/*******************************************
 * Initialization
 */

/* NEC IR Init*/
void NEC_IR_Init(void)
{
    nec.timerHandle = &htim2;
    nec.timerChannel = TIM_CHANNEL_1;
    nec.timerChannelActive = HAL_TIM_ACTIVE_CHANNEL_1;

    nec.timingBitBoundary = 1680 * 2;
    nec.timingAgcBoundary = 12500 * 2;
    nec.type = NEC_EXTENDED;

    nec.NEC_DecodedCallback = IR_receive_callback;
    nec.NEC_ErrorCallback = IR_error_callback;
    nec.NEC_RepeatCallback = IR_repeat_callback;

    NEC_Read(&nec);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &htim2)
    {
        NEC_TIM_IC_CaptureCallback(&nec);
    }
}

/*******************************************
 * Customized IR Initializer
 */
void IR_receive_callback(uint16_t address, uint8_t cmd)
{
    global_status.irConnected = 1;
    IR_receive_key(KeySelect(cmd));

    HAL_Delay(10);
    NEC_Read(&nec);
}

void IR_error_callback()
{
    global_status.irConnected = 1;
    printf("Error!\r\n");
    HAL_Delay(10);
    NEC_Read(&nec);
}

void IR_repeat_callback()
{
    global_status.irConnected = 1;
    printf("Repeat!\r\n");
    HAL_Delay(10);
    NEC_Read(&nec);
}

__weak void IR_receive_key(Key key)
{
}

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
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
    {
        Error_Handler();
    }

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART3 | RCC_PERIPHCLK_USART2 | RCC_PERIPHCLK_USB;
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
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);

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
    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
}

/*******************************************
 * Utility function
 */

void Error_Handler()
{
    // TODO Handle Error
    printf("ERROR HANDLER");
    while (1)
        ;
}

int __io_putchar(int ch)
{
    if (HAL_UART_Transmit_IT(&huart2, (uint8_t *)&ch, 1) != HAL_OK)
        RingBuffer_Write(&pcTxBuf, (uint8_t *)&ch, 1);
    return ch;
}

void Init_global_state()
{
    strcpy(global_status.name, "Not Conn.");
    global_status.deviceType = CTRL_DEVICETYPE_NC;

    global_status.irConnected = 0;
    global_status.usbConnected = 0;
    global_status.usbDeviceType = CTRL_USB_NC;

    global_status.showMessage = 0;
    global_status.message[0] = 0;

    global_status.showOption = 0;
    global_status.optionPrompt[0] = 0;
    global_status.optionType = CTRL_OPTION_OK;

    global_status.min = 0;
    global_status.dot = 0;
    global_status.sec = 0;

    global_status.keySent = 0;

    global_status.shiftKey = 0;
    global_status.ctrlKey = 0;
    global_status.winKey = 0;
    global_status.altKey = 0;

    global_status.key.keyvalue = -1;
    global_status.key.keyshow = NULL;
}
