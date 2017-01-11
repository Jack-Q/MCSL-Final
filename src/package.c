#include "global_state.h"
#include "package.h"
#include "stm32l4xx_hal.h"
#include "string.h"

extern CTRL_status_t global_status;

#define PKG_RX_NAME_0 0x20
#define PKG_RX_NAME_1 0x21
#define PKG_RX_NAME_2 0x22
#define PKG_RX_NAME_3 0x23
#define PKG_RX_CONNECT 0xff
#define PKG_RX_MSG_0 0x70
#define PKG_RX_MSG_1 0x71
#define PKG_RX_MSG_2 0x72
#define PKG_RX_MSG_3 0x73
#define PKG_RX_MSG_4 0x74
#define PKG_RX_MSG_5 0x75

#define PKG_CONN_PC 0x01
#define PKG_CONN_ANDROID 0x02

#define PKG_TX_KEY 0xaa
#define PKG_TX_MOUSE 0x55
#define PKG_TX_ACT 0xdd

#define PKG_MOUSE_LEFT 0x01
#define PKG_MOUSE_RIGHT 0x02
#define PKG_MOUSE_MID_KEY 0x04
#define PKG_MOUSE_MID_UP 0x08
#define PKG_MOUSE_MID_DOWN 0x10

#define PKG_KEY_SHIFT 0x01
#define PKG_KEY_ALT 0x02
#define PKG_KEY_CTRL 0x04
#define PKG_KEY_WIN 0x08

#define PKG_ACT_SHUTDOWN 0x01

void encodeKeyPackage(CTRL_dataPackage_t *pkg) {
  pkg->data[0] = PKG_TX_KEY;
  pkg->data[1] = 0;
  if (global_status.shiftKey) pkg->data[1] |= PKG_KEY_SHIFT;
  if (global_status.ctrlKey) pkg->data[1] |= PKG_KEY_CTRL;
  if (global_status.altKey) pkg->data[1] |= PKG_KEY_ALT;
  if (global_status.winKey) pkg->data[1] |= PKG_KEY_WIN;

  if (global_status.key.keyvalue == -1)
    pkg->data[2] = 0;
  else
    pkg->data[2] = global_status.key.keyvalue;

  pkg->data[3] = 0;
  pkg->pos = 0;
}
void encodeMousePackage(CTRL_dataPackage_t *pkg) {
  pkg->data[0] = PKG_TX_MOUSE;
  pkg->pos = 0;
}
void encodeActPackage(CTRL_dataPackage_t *pkg) {
	pkg->data[0] = PKG_TX_ACT;
	pkg->data[1] = 0; // Confirmation
	pkg->data[2] = 0; // Delay
	pkg->data[3] = global_status.actCode;
	pkg->pos = 0;
}

void decodePackage(CTRL_dataPackage_t *pkg) {
  if (!pkg) return;
  switch (pkg->data[0]) {
    case PKG_RX_NAME_0:
    case PKG_RX_NAME_1:
    case PKG_RX_NAME_2:
    case PKG_RX_NAME_3:
      memcpy(global_status.name + 3 * (pkg->data[0] & 0x0f), pkg->data + 1, 3);
      break;

    case PKG_RX_MSG_0:
    case PKG_RX_MSG_1:
    case PKG_RX_MSG_2:
    case PKG_RX_MSG_3:
    case PKG_RX_MSG_4:
    case PKG_RX_MSG_5:
      memcpy(global_status.message + 3 * (pkg->data[0] & 0x0f), pkg->data + 1,
             3);
      if (!pkg->data[1] || !pkg->data[2] || !pkg->data[3]) {
        global_status.showMessage = 1;
      }
      break;

    case PKG_RX_CONNECT:

      // if (global_status.deviceType == CTRL_DEVICETYPE_NC) {
      //   // New connection
      //   // memset(global_status.name, 0, sizeof(global_status.name));
      // }
      global_status.deviceType = pkg->data[1] == PKG_CONN_PC
                                     ? CTRL_DEVICETYPE_PC
                                     : CTRL_DEVICETYPE_PHONE;
      global_status.lastConn = HAL_GetTick();
      break;
  }
}
