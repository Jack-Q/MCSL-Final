/********************************************************
 * Micro Computer System Lab - Final Project
 *                                               Group 49
 ********************************************************
 * Bluetooth & IR Controller
 ********************************************************/


#ifndef __INITIALIZE_H
#define __INITIALIZE_H

#include "global_state.h"


extern TIM_HandleTypeDef htim2;
extern DMA_HandleTypeDef hdma_tim2_ch1;

#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA


void initialize();
void Error_Handler();

#endif
