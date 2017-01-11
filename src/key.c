#include "key.h"
#include "global_state.h"

extern CTRL_status_t global_status;


Key KeySelect(uint8_t buf) {
  Key tmp;
  switch (buf) {
    case 0x19:
      tmp.keyvalue = KEY_VALUE_SEND;
      tmp.keyshow = "send";
      tmp.virtualCode = 0x00;
      break;
    case 0x0a:
      tmp.keyvalue = KEY_VALUE_POWER;  // 0x06
      tmp.keyshow = "Turn off";
      tmp.virtualCode = 0x00;
      break;
    case 0x14:
      tmp.keyvalue = KEY_VALUE_CTRL;  // 0x02
      tmp.keyshow = "ctrl";
      tmp.virtualCode = 0x11;
      break;
    case 0x17:
      tmp.keyvalue = KEY_VALUE_SHIFT;  // 0x03
      tmp.keyshow = "shift";
      tmp.virtualCode = 0x10;
      break;
    case 0x1d:
      tmp.keyvalue = KEY_VALUE_ALT;  // 0x04
      tmp.keyshow = "alt";
      tmp.virtualCode = 0x12;
      break;
    case 0x43:
      tmp.keyvalue = KEY_VALUE_WIN;  // 0x05
      tmp.keyshow = "windows";
      tmp.virtualCode = 0x5B;
      break;
    case 0x01:
      if (global_status.numberLock) {
        tmp.keyvalue = 0x11;
        tmp.keyshow = " 1 ";
        tmp.virtualCode = 0x31;
      } else {
        tmp.keyvalue = 0x11;
        tmp.keyshow = "F1";
        tmp.virtualCode = 0x70;
      }
      break;
    case 0x02:
	  if (global_status.numberLock) {
        tmp.keyvalue = 0x12;
        tmp.keyshow = " 2 ";
        tmp.virtualCode = 0x32;
      } else {
        tmp.keyvalue = 0x12;
        tmp.keyshow = "F2";
        tmp.virtualCode = 0x71;
      }
      break;
    case 0x03:
	  if (global_status.numberLock) {
        tmp.keyvalue = 0x13;
        tmp.keyshow = " 3 ";
        tmp.virtualCode = 0x33;
      } else {
        tmp.keyvalue = 0x13;
        tmp.keyshow = "F3";
        tmp.virtualCode = 0x72;
      }
      break;
    case 0x04:
	  if (global_status.numberLock) {
        tmp.keyvalue = 0x14;
        tmp.keyshow = " 4 ";
        tmp.virtualCode = 0x34;
      } else {
        tmp.keyvalue = 0x14;
        tmp.keyshow = "F4";
        tmp.virtualCode = 0x73;
      }
      break;
    case 0x05:
	  if (global_status.numberLock) {
        tmp.keyvalue = 0x15;
        tmp.keyshow = " 5 ";
        tmp.virtualCode = 0x35;
      } else {
        tmp.keyvalue = 0x15;
        tmp.keyshow = "F5";
        tmp.virtualCode = 0x74;
      }
      break;
    case 0x06:
	  if (global_status.numberLock) {
        tmp.keyvalue = 0x16;
        tmp.keyshow = " 6 ";
        tmp.virtualCode = 0x36;
      } else {
        tmp.keyvalue = 0x16;
        tmp.keyshow = "F6";
        tmp.virtualCode = 0x75;
      }
      break;
    case 0x07:
	  if (global_status.numberLock) {
        tmp.keyvalue = 0x17;
        tmp.keyshow = " 7 ";
        tmp.virtualCode = 0x37;
      } else {
        tmp.keyvalue = 0x17;
        tmp.keyshow = "F7";
        tmp.virtualCode = 0x76;
      }
      break;
    case 0x08:
	  if (global_status.numberLock) {
        tmp.keyvalue = 0x18;
        tmp.keyshow = " 8 ";
        tmp.virtualCode = 0x38;
      } else {
        tmp.keyvalue = 0x18;
        tmp.keyshow = "F8";
        tmp.virtualCode = 0x77;
      }
      break;
    case 0x09:
	  if (global_status.numberLock) {
        tmp.keyvalue = 0x19;
        tmp.keyshow = " 9 ";
        tmp.virtualCode = 0x39;
      } else {
        tmp.keyvalue = 0x19;
        tmp.keyshow = "F9";
        tmp.virtualCode = 0x78;
      }
      break;
    case 0x40:
      tmp.keyvalue = 0x1A;
      tmp.keyshow = "F11";
      tmp.virtualCode = 0x79;
      break;
    case 0x00:
	  if (global_status.numberLock) {
        tmp.keyvalue = 0x1B;
        tmp.keyshow = " 0 ";
        tmp.virtualCode = 0x30;
      } else {
        tmp.keyvalue = 0x1B;
        tmp.keyshow = "F10";
        tmp.virtualCode = 0x7A;
      }
      break;
    case 0x45:
      tmp.keyvalue = 0x1C;
      tmp.keyshow = "F12";
      tmp.virtualCode = 0x7B;
      break;
    case 0x16:
      tmp.keyvalue = 0x1D;
      tmp.keyshow = "bksp";
      tmp.virtualCode = 0x08;
      break;
    case 0x1f:
      tmp.keyvalue = 0x1E;
      tmp.keyshow = "del";
      tmp.virtualCode = 0x2E;
      break;
    case 0x0d:
      tmp.keyvalue = KEY_VALUE_OK;
      tmp.keyshow = "enter";
      tmp.virtualCode = 0x0D;
      break;
    case 0x0b:
      tmp.keyvalue = 0x20;
      tmp.keyshow = "up";
      tmp.virtualCode = 0x26;
      break;
    case 0x0e:
      tmp.keyvalue = 0x21;
      tmp.keyshow = "down";
      tmp.virtualCode = 0x28;
      break;
    case 0x10:
      tmp.keyvalue = 0x22;
      tmp.keyshow = "right";
      tmp.virtualCode = 0x27;
      break;
    case 0x11:
      tmp.keyvalue = 0x23;
      tmp.keyshow = "left";
      tmp.virtualCode = 0x25;
      break;
    case 0x1a:
      tmp.keyvalue = 0x24;
      tmp.keyshow = "A";
      tmp.virtualCode = 0x41;
      break;
    case 0x1b:
      tmp.keyvalue = 0x25;
      tmp.keyshow = "B";
      tmp.virtualCode = 0x42;
      break;
    case 0x41:
      tmp.keyvalue = 0x26;
      tmp.keyshow = "C";
      tmp.virtualCode = 0x43;
      break;
    case 0x42:
      tmp.keyvalue = 0x27;
      tmp.keyshow = "V";
      tmp.virtualCode = 0x56;
      break;
    case 0x1c:
      tmp.keyvalue = 0x28;
      tmp.keyshow = "F";
      tmp.virtualCode = 0x46;
      break;
    case 0x44:
      tmp.keyvalue = 0x29;
      tmp.keyshow = "G";
      tmp.virtualCode = 0x47;
      break;
    case 0x0c:
      tmp.keyvalue = 0x2A;
      tmp.keyshow = "H";
      tmp.virtualCode = 0x48;
      break;
    case 0x15:
      tmp.keyvalue = 0x2B;
      tmp.keyshow = "D";
      tmp.virtualCode = 0x44;
      break;
    case 0x18:
      tmp.keyvalue = 0x2C;
      tmp.keyshow = "R";
      tmp.virtualCode = 0x52;
      break;
    case 0x54:
      tmp.keyvalue = 0x2D;
      tmp.keyshow = "Z";
      tmp.virtualCode = 0x5A;
      break;
    case 0x48:
      tmp.keyvalue = 0x2E;
      tmp.keyshow = "Y";
      tmp.virtualCode = 0x59;
      break;
    case 0x12:
      tmp.keyvalue = 0x2F;
      tmp.keyshow = "X";
      tmp.virtualCode = 0x58;
      break;
    case 0x50:
      tmp.keyvalue = 0x30;
      tmp.keyshow = "S";
      tmp.virtualCode = 0x53;
      break;
    case 0x58:
      tmp.keyvalue = 0x31;
      tmp.keyshow = "T";
      tmp.virtualCode = 0x54;
      break;
    case 0x4c:
      tmp.keyvalue = 0x32;
      tmp.keyshow = "W";
      tmp.virtualCode = 0x57;
      break;
    case 0x13:
      tmp.keyvalue = 0x33;
      tmp.keyshow = "`U";
      tmp.virtualCode = 0x00;
      break;
    case 0x0f:
      tmp.keyvalue = 0x34;
      tmp.keyshow = "tab";
      tmp.virtualCode = 0x09;
      break;
    default:
      tmp.keyvalue = -1;
      tmp.keyshow = "";
      tmp.virtualCode = 0x00;
      break;
  }
  return tmp;
}
