#ifndef __GLOBAL_STATE_H
#define __GLOBAL_STATE_H

#include "NEC_Decode.h"
#include "key.h"
#include "ringbuffer.h"
#include "stm32l4xx.h"
#include "stm32l4xx_hal.h"

typedef enum {
  CTRL_DEVICETYPE_NC = 0x0,    // not connected
  CTRL_DEVICETYPE_PC = 0x1,    // controlling PC
  CTRL_DEVICETYPE_PHONE = 0x2  // controlling Phone
} CTRL_deviceType_t;

typedef enum {
  CTRL_USB_NC = 0x0,       // not connected
  CTRL_USB_MOUSE = 0x1,    // controlling PC
  CTRL_USB_KEYBOARD = 0x2  // controlling Phone
} CTRL_usbType_t;

typedef enum {
  CTRL_OPTION_OK = 0x0,        // OK only
  CTRL_OPTION_YES_NO = 0x1,    // Yes, No
  CTRL_OPTION_SURE_CANC = 0x2  // Sure, Cancel
} CTRL_optionType_t;

typedef struct {
  int pos;
  char data[4];
} CTRL_dataPackage_t;

typedef struct {
  char name[12];
  CTRL_deviceType_t deviceType;
  int lastConn;

  uint8_t usbConnected;
  CTRL_usbType_t usbDeviceType;
  uint8_t irConnected;

  uint8_t blueRxReady;
  CTRL_dataPackage_t blueRx;
  uint8_t blueTxReady;
  CTRL_dataPackage_t blueTx;

  uint8_t showMessage;
  char message[17];

  uint8_t showOption;
  char optionPrompt[17];
  CTRL_optionType_t optionType;

  uint8_t min;  // Minute
  char dot;     // : or []
  uint8_t sec;  // Second

  uint8_t keySent;   // whether currently displayed key sent
  uint8_t keyReady;  // whether currently displayed key is ready to send

  uint8_t shiftKey;  // SHIFT Key Toggle
  uint8_t ctrlKey;   // CTRL Key Toggle
  uint8_t winKey;    // Win Key Toggle
  uint8_t altKey;    // ALT Key Toggle
  Key key;           // General Key

} CTRL_status_t;

#endif
