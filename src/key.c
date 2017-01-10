#include "key.h"

Key KeySelect(uint8_t buf){
    Key tmp;
    switch(buf){
        case 0x19:
            tmp.keyvalue=0x00;
            tmp.keyshow="combine";
            break;
        case 0x0a:
            tmp.keyvalue=0x01;
            tmp.keyshow="Turn off";
            break;
		case 0x14:
            tmp.keyvalue=0x02;
            tmp.keyshow="ctrl";
            break;
		case 0x17:
            tmp.keyvalue=0x03;
            tmp.keyshow="shift";
            break;
		case 0x1d:
            tmp.keyvalue=0x04;
            tmp.keyshow="alt";
            break;
		case 0x43:
            tmp.keyvalue=0x05;
            tmp.keyshow="windows";
            break;
        case 0x01:
            tmp.keyvalue=0x11;
            tmp.keyshow="F1";
            break;
        case 0x02:
            tmp.keyvalue=0x12;
            tmp.keyshow="F2";
            break;
        case 0x03:
            tmp.keyvalue=0x13;
            tmp.keyshow="F3";
            break;
        case 0x04:
            tmp.keyvalue=0x14;
            tmp.keyshow="F4";
            break;
        case 0x05:
            tmp.keyvalue=0x15;
            tmp.keyshow="F5";
            break;
        case 0x06:
            tmp.keyvalue=0x16;
            tmp.keyshow="F6";
            break;
        case 0x07:
            tmp.keyvalue=0x17;
            tmp.keyshow="F7";
            break;
        case 0x08:
            tmp.keyvalue=0x18;
            tmp.keyshow="F8";
            break;
        case 0x09:
            tmp.keyvalue=0x19;
            tmp.keyshow="F9";
            break;
		case 0x40:
            tmp.keyvalue=0x1A;
            tmp.keyshow="F10";
            break;
        case 0x00:
            tmp.keyvalue=0x1B;
            tmp.keyshow="F11";
            break;
		case 0x45:
            tmp.keyvalue=0x1C;
            tmp.keyshow="F12";
            break;
		case 0x16:
            tmp.keyvalue=0x1D;
            tmp.keyshow="bksp";
            break;
        case 0x1f:
            tmp.keyvalue=0x1E;
            tmp.keyshow="del";
            break;
		case 0x0d:
            tmp.keyvalue=0x1F;
            tmp.keyshow="enter";
            break;
        case 0x0b:
            tmp.keyvalue=0x20;
            tmp.keyshow="up";
            break;
		case 0x0e:
            tmp.keyvalue=0x21;
            tmp.keyshow="down";
            break;
        case 0x10:
            tmp.keyvalue=0x22;
            tmp.keyshow="right";
            break;
        case 0x11:
            tmp.keyvalue=0x23;
            tmp.keyshow="left";
            break;
		case 0x1a:
            tmp.keyvalue=0x24;
            tmp.keyshow="A";
            break;
		case 0x1b:
            tmp.keyvalue=0x25;
            tmp.keyshow="B";
            break;
		case 0x41:
            tmp.keyvalue=0x26;
            tmp.keyshow="C";
            break;
		case 0x42:
            tmp.keyvalue=0x27;
            tmp.keyshow="V";
            break;
		case 0x1c:
            tmp.keyvalue=0x28;
            tmp.keyshow="F";
            break;
		case 0x44:
            tmp.keyvalue=0x29;
            tmp.keyshow="G";
            break;
		case 0x0c:
            tmp.keyvalue=0x2A;
            tmp.keyshow="H";
            break;
		case 0x15:
            tmp.keyvalue=0x2B;
            tmp.keyshow="D";
            break;
		case 0x18:
            tmp.keyvalue=0x2C;
            tmp.keyshow="R";
            break;
		case 0x54:
            tmp.keyvalue=0x2D;
            tmp.keyshow="Z";
            break;
		case 0x48:
            tmp.keyvalue=0x2E;
            tmp.keyshow="Y";
            break;
		case 0x12:
            tmp.keyvalue=0x2F;
            tmp.keyshow="X";
            break;
		case 0x50:
            tmp.keyvalue=0x30;
            tmp.keyshow="S";
            break;
		case 0x58:
            tmp.keyvalue=0x31;
            tmp.keyshow="T";
            break;
		case 0x4c:
            tmp.keyvalue=0x32;
            tmp.keyshow="W";
            break;
		case 0x13:
            tmp.keyvalue=0x33;
            tmp.keyshow="`U";
            break;
		case 0x0f:
            tmp.keyvalue=0x34;
            tmp.keyshow="tab";
            break;
        }
    return tmp;
}
