#ifndef __MAIN_H
#define __MAIN_H

#include "stdio.h"
#include "global_state.h"

void IR_receive_callback(uint16_t, uint8_t);
void IR_repeat_callback();
void IR_error_callback();


typedef enum{
	LCD_DEVICETYPE_NC = 0x0, 		// not connected
	LCD_DEVICETYPE_PC = 0x1, 		// controlling PC
	LCD_DEVICETYPE_PHONE = 0x2 		// controlling Phone
} CTRL_deviceType_t;


typedef struct {
	char name[8];
	CTRL_deviceType_t deviceType;

	char mouseConnected;
	char keyboardConnected;
	char irConnected;

	char showMessage;
	char message[16];

	char min; // Minute
	char dot; // : or []
	char sec; // Second

	char shiftKey; // SHIFT Key Toggle
	char ctrlKey;  // CTRL Key Toggle
	char metaKey;  // META (Win) Key Toggle
	char altKey;   // ALT Key Toggle
	char keyName;  // General Key

} CTRL_status_t;
#endif
