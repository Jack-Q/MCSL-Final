#include "ctrl_time.h"
#include "stm32l4xx_hal.h"
#include "global_state.h"


#define CTRL_timeTickSpan 1000

extern CTRL_status_t global_status;

static int initTick = 0;

void CTRL_timeReset(){
global_status.min = 0;
global_status.sec = 0;
global_status.dot = 0;
initTick = 0;
}

void CTRL_timeUpdate()
{
    int tick = HAL_GetTick() - initTick;
    global_status.min = tick / (60 * CTRL_timeTickSpan);
    global_status.dot = tick * 2 / CTRL_timeTickSpan & 0x01;
    global_status.sec = tick / CTRL_timeTickSpan % 60;
}
