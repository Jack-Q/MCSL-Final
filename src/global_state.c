#define GLOBAL_DEFINITION

#include "global_state.h"

CTRL_status_t global_status;

UART_HandleTypeDef huart3, huart2;
RingBuffer pcTxBuf, pcRxBuf, btTxBuf, btRxBuf;
char pcReadBuf[1], btReadBuf[1];
uint8_t pcTxData, btTxData;
__IO ITStatus PcUartReady = SET, BtUartReady = SET;

NEC nec;
TIM_HandleTypeDef htim2;
DMA_HandleTypeDef hdma_tim2_ch1;

