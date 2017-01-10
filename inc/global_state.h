#ifndef __GLOBAL_STATE_H
#define __GLOBAL_STATE_H

#include "stm32l4xx.h"
#include "NEC_Decode.h"
#include "ringbuffer.h"
#include "stm32l4xx_hal.h"
#include "key.h"


typedef enum{
	 CTRL_DEVICETYPE_NC = 0x0, 		// not connected
	 CTRL_DEVICETYPE_PC = 0x1, 		// controlling PC
	 CTRL_DEVICETYPE_PHONE = 0x2 		// controlling Phone
} CTRL_deviceType_t;
typedef enum{
	 CTRL_USB_NC = 0x0, 		// not connected
	 CTRL_USB_MOUSE = 0x1, 		// controlling PC
	 CTRL_USB_KEYBOARD = 0x2 		// controlling Phone
} CTRL_usbType_t;



typedef struct {
	char name[12];
	CTRL_deviceType_t deviceType;

	char usbConnected;
	CTRL_usbType_t usbDeviceType;
	char irConnected;

	char showMessage;
	char message[17];

	uint8_t min; // Minute
	char dot; // : or []
	uint8_t sec; // Second

	char shiftKey; // SHIFT Key Toggle
	char ctrlKey;  // CTRL Key Toggle
	char winKey;  // Win Key Toggle
	char altKey;   // ALT Key Toggle
	Key key;  // General Key

} CTRL_status_t;

static CTRL_status_t global_status;

static UART_HandleTypeDef huart3, huart2;
static RingBuffer pcTxBuf, pcRxBuf, btTxBuf, btRxBuf;
static char pcReadBuf[1], btReadBuf[1];
static uint8_t pcTxData, btTxData;
static __IO ITStatus PcUartReady = SET, BtUartReady = SET;

static NEC nec;

#endif
