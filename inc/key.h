#ifndef __KEY_H
#define __KEY_H

#include "stm32l4xx.h"

#define KEY_VALUE_SEND 0x01
#define KEY_VALUE_SHIFT 0x03
#define KEY_VALUE_ALT 0x04
#define KEY_VALUE_CTRL 0x02
#define KEY_VALUE_WIN 0x05
#define KEY_VALUE_POWER 0x06
#define KEY_VALUE_OK 0x07
#define KEY_VALUE_NUM_LOCK 0x33

typedef struct {
  int keyvalue;
  char *keyshow;
  uint8_t virtualCode;
} Key;

Key KeySelect(uint8_t buf);

#endif
