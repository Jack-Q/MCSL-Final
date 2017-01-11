#ifndef __KEY_H
#define __KEY_H

#include "stm32l4xx.h"

#define KEY_VALUE_SHIFT 	0x03
#define KEY_VALUE_ALT		0x04
#define KEY_VALUE_CTRL 	0x02
#define KEY_VALUE_WIN		0x05

typedef struct{
    int keyvalue;
    char *keyshow;
} Key;

Key KeySelect(uint8_t buf);

#endif
