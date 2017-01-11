#ifndef __LCD_H
#define __LCD_H

#include <stm32l476xx.h>

// Initialize
void LCD_init();

// Update font of device icon,
void LCD_updateFont();

// Display Buffer
void LCD_updateDisplay();

#endif
