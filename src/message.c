#include "global_state.h"
#include "message.h"
#include "stm32l4xx_hal.h"
#include "string.h"

#define MESSAGE_DISPLAY_LENGTH 3000
extern CTRL_status_t global_status;

static int messageDisplayTime = 0;

void displayMessage(char *message) {
	strcpy(global_status.message, message);
	global_status.showMessage = 1;
	messageDisplayTime = HAL_GetTick();
}

void updateMessage() {
	if(global_status.showMessage){
		if(HAL_GetTick() - messageDisplayTime > MESSAGE_DISPLAY_LENGTH){
			global_status.showMessage = 0;
		}
	}
}
