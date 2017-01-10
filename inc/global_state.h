#ifndef __GLOBAL_STATE_H
#define __GLOBAL_STATE_H

#include "stm32l4xx.h"
#include "NEC_Decode.h"
#include "ringbuffer.h"
#include "stm32l4xx_hal.h"
#include "key.h"

typedef enum {
	CTRL_DEVICETYPE_NC = 0x0, 		// not connected
	CTRL_DEVICETYPE_PC = 0x1, 		// controlling PC
	CTRL_DEVICETYPE_PHONE = 0x2 		// controlling Phone
} CTRL_deviceType_t;
typedef enum {
	CTRL_USB_NC = 0x0, 		// not connected
	CTRL_USB_MOUSE = 0x1, 		// controlling PC
	CTRL_USB_KEYBOARD = 0x2 		// controlling Phone
} CTRL_usbType_t;

typedef struct {
	char name[12];
	CTRL_deviceType_t deviceType;

	uint8_t usbConnected;
	CTRL_usbType_t usbDeviceType;
	uint8_t irConnected;

	uint8_t showMessage;
	char message[17];

	uint8_t min; // Minute
	char dot; // : or []
	uint8_t sec; // Second

	uint8_t shiftKey; // SHIFT Key Toggle
	uint8_t ctrlKey;  // CTRL Key Toggle
	uint8_t winKey;  // Win Key Toggle
	uint8_t altKey;   // ALT Key Toggle
	Key key;  // General Key

} CTRL_status_t;

#endif
