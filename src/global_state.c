/********************************************************
 * Micro Computer System Lab - Final Project
 *                                               Group 49
 ********************************************************
 * Bluetooth & IR Controller
 ********************************************************/


#define GLOBAL_DEFINITION

#include "global_state.h"

CTRL_status_t global_status;

UART_HandleTypeDef huart3, huart2;
RingBuffer pcTxBuf, pcRxBuf;
char pcReadBuf[1], btReadBuf[1];
uint8_t pcTxData;
__IO ITStatus PcUartReady = SET;

NEC nec;
TIM_HandleTypeDef htim2;
DMA_HandleTypeDef hdma_tim2_ch1;

