#ifndef __INITIALIZE_H
#define __INITIALIZE_H

#include "global_state.h"


static TIM_HandleTypeDef htim2;
static DMA_HandleTypeDef hdma_tim2_ch1;

#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA

extern void IR_receive_callback(uint16_t, uint8_t);
extern void IR_repeat_callback();
extern void IR_error_callback();

void initialize();
void Error_Handler();

#endif
