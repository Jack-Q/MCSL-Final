#ifndef __GLOBAL_STATE_H
#define __GLOBAL_STATE_H

#include "stm32l4xx.h"
#include "NEC_Decode.h"
#include "ringbuffer.h"
#include "stm32l4xx_hal.h"


static UART_HandleTypeDef huart3, huart2;
static RingBuffer pcTxBuf, pcRxBuf, btTxBuf, btRxBuf;
static char pcReadBuf[1], btReadBuf[1];
static uint8_t pcTxData, btTxData;
static __IO ITStatus PcUartReady = SET, BtUartReady = SET;

static NEC nec;

#endif
