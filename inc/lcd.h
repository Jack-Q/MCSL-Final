#ifndef __LCD_H
#define __LCD_H

#include <stm32l476xx.h>



// [DT][   name   ][IR][MS]
// [      message...      ]
//
// [KEYS][ key name][mm:ss]
// [?] [OPT1] [OPT2] [OPT3]

// Initialize
void LCD_init();
// Display Buffer
void LCD_updateDisplay();

#endif
