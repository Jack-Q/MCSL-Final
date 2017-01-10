#ifndef __INITIALIZE_H
#define __INITIALIZE_H

#include "global_state.h"

extern void IR_receive_callback(uint16_t, uint8_t);
extern void IR_repeat_callback();
extern void IR_error_callback();

void initialize();
void Error_Handler();

#endif
