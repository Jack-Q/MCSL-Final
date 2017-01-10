#include "stm32l4xx.h"

typedef struct{
    int keyvalue;
    char *keyshow;
} Key;

Key KeySelect(uint8_t buf);
