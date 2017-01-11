#include "stdio.h"
#include "string.h"

#include "stm32l4xx.h"
#include "stm32l4xx_hal.h"
#include "usb_host.h"
#include "usbh_hid.h"

#include "global_state.h"
#include "initialize.h"
#include "key.h"
#include "lcd.h"
#include "ringbuffer.h"
#include "main.h"
#include "ctrl_time.h"

extern CTRL_status_t global_status;

extern UART_HandleTypeDef huart3, huart2;
extern RingBuffer pcTxBuf, pcRxBuf, btTxBuf, btRxBuf;
extern char pcReadBuf[1], btReadBuf[1];
extern uint8_t pcTxData, btTxData;
extern __IO ITStatus PcUartReady, BtUartReady;


int main() {
	initialize();

	int pos = 0;
	char cmdBuf[100], data;
	char *send_message = "Send: ";

	uint32_t lastBtReady = HAL_GetTick();

	// TODO: Test
	global_status.deviceType = CTRL_DEVICETYPE_PC;

	while (1) {
		MX_USB_HOST_Process(); // handle background USB process
		CTRL_timeUpdate();		 // update time display
		LCD_updateDisplay();	 // update LCD display

		// Handle Blue-tooth Data
		if (BtUartReady) {
			if(global_status.deviceType != CTRL_DEVICETYPE_PHONE){

			}
			lastBtReady = HAL_GetTick();
			// Receive data from BlueTooth Module
			BtUartReady = RESET;
			data = btReadBuf[0];
			HAL_UART_Receive_IT(&huart3, (uint8_t *) btReadBuf, 1);

			// Process received data
			printf("%c", data);
		}

		if (PcUartReady) {
			// Receive data from PC
			PcUartReady = RESET;
			data = pcReadBuf[0];
			HAL_UART_Receive_IT(&huart2, (uint8_t *) pcReadBuf, 1);

			// Process received data
			if (data == '\r' || data == '\n') {
				if (pos > 1) {
					cmdBuf[pos] = 0;
					printf("%s%s\r\n", send_message, cmdBuf);
					cmdBuf[pos++] = '\r';
					cmdBuf[pos++] = '\n';
					RingBuffer_Write(&btTxBuf, (uint8_t *) cmdBuf, pos);
					HAL_UART_TxCpltCallback(&huart3);
					pos = 0;
				}
			} else {
				cmdBuf[pos++] = data;
			}
		}

	}
}

// Callback when IR receive key press (exclude repeated key press)
void IR_receive_key(Key key){
	if(global_status.keySent){
		global_status.shiftKey = 0;
		global_status.altKey = 0;
		global_status.ctrlKey = 0;
		global_status.winKey = 0;
		global_status.key.keyvalue = -1;
	}
	switch(key.keyvalue){
	case KEY_VALUE_SHIFT:
		global_status.shiftKey = !global_status.shiftKey;
		break;
	case KEY_VALUE_ALT:
		global_status.altKey = !global_status.altKey;
		break;
	case KEY_VALUE_CTRL:
		global_status.ctrlKey= !global_status.ctrlKey;
		break;
	case KEY_VALUE_WIN:
		global_status.winKey = !global_status.winKey;
		break;
	case -1:
		break;
	default: // General keys
		global_status.key.keyshow= key.keyshow;
		global_status.key.keyvalue= key.keyvalue;
		global_status.keyReady = 1;
		break;
	}
}


// Interrupt handler
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	/* Set transmission flag: transfer complete*/
	if (huart->Instance == USART3) {
		// BT
		BtUartReady = SET;
	} else if (huart->Instance == USART2) {
		// PC
		PcUartReady = SET;
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
	// Check whether the transfer buffer is empty
	if (huart->Instance == USART3) {
		// BT
		if (RingBuffer_GetDataLength(&btTxBuf) > 0) {
			RingBuffer_Read(&btTxBuf, &btTxData, 1);
			HAL_UART_Transmit_IT(huart, &btTxData, 1);
		}
	} else if (huart->Instance == USART2) {
		// PC
		if (RingBuffer_GetDataLength(&pcTxBuf) > 0) {
			RingBuffer_Read(&pcTxBuf, &pcTxData, 1);
			HAL_UART_Transmit_IT(huart, &pcTxData, 1);
		}
	}
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART3) {
		// BT
		if (huart->ErrorCode == HAL_UART_ERROR_ORE)
			HAL_UART_Receive_IT(huart, (uint8_t *) btReadBuf, 1);
	} else if (huart->Instance == USART2) {
		// PC
		if (huart->ErrorCode == HAL_UART_ERROR_ORE)
			HAL_UART_Receive_IT(huart, (uint8_t *) pcReadBuf, 1);
	}
}


/******************************************
 * USB HID Callbacks
 */

void USBH_HID_EventCallback(USBH_HandleTypeDef *phost) {
	HID_MOUSE_Info_TypeDef *mouse_info = NULL;
	HID_KEYBD_Info_TypeDef *keybd_info = NULL;
	// Check device type
	switch (USBH_HID_GetDeviceType(phost)) {
	case HID_MOUSE:
		printf("[USB_MOUSE]");
		mouse_info = USBH_HID_GetMouseInfo(phost);
		if (mouse_info) {
			printf("%3d, %3d, %d:%d:%d", mouse_info->x, mouse_info->y,
					mouse_info->buttons[0], mouse_info->buttons[1],
					mouse_info->buttons[2]);
		} else {
			printf("BUSY");
		}
		break;
	case HID_KEYBOARD:
		printf("[USB_KEYBOARD]");
		keybd_info = USBH_HID_GetKeybdInfo(phost);
		if (keybd_info) {
			printf("STAT: %d", keybd_info->state);
		} else {
			printf("BUSY");
		}
		break;
	case HID_UNKNOWN:
	default:
		printf("[USB_UNKNOW]");
		break;
	}

	printf("\n");
}
