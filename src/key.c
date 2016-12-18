#include "key.h"

Key KeySelect(uint8_t buf){
    Key tmp;
    switch(buf){
        case 0x0a:
            tmp.keyvalue=0x76;
            tmp.keyshow="Home";
            break;
        case 0x19:
            tmp.keyvalue=0xE0;
            tmp.keyshow="Ctrl";
            break;
        case 0x01:
            tmp.keyvalue=0x3A;
            tmp.keyshow="F1";
            break;
        case 0x02:
            tmp.keyvalue=0x3B;
            tmp.keyshow="F2";
            break;
        case 0x03:
            tmp.keyvalue=0x3C;
            tmp.keyshow="F3";
            break;
        case 0x04:
            tmp.keyvalue=0x3D;
            tmp.keyshow="F4";
            break;
        case 0x05:
            tmp.keyvalue=0x3E;
            tmp.keyshow="F5";
            break;
        case 0x06:
            tmp.keyvalue=0x3F;
            tmp.keyshow="F6";
            break;
        case 0x07:
            tmp.keyvalue=0x40;
            tmp.keyshow="F7";
            break;
        case 0x08:
            tmp.keyvalue=0x41;
            tmp.keyshow="F8";
            break;
        case 0x09:
            tmp.keyvalue=0x42;
            tmp.keyshow="F9";
            break;
        case 0x00:
            tmp.keyvalue=0xE2;
            tmp.keyshow="alt";
            break;
        case 0x0b:
            tmp.keyvalue=0x1A;
            tmp.keyshow="W";
            break;
        case 0x0e:
            tmp.keyvalue=0x16;
            tmp.keyshow="S";
            break;
        case 0x10:
            tmp.keyvalue=0x07;
            tmp.keyshow="D";
            break;
        case 0x11:
            tmp.keyvalue=0x04;
            tmp.keyshow="A";
            break;
        default:
            tmp.keyvalue=0x00;
            tmp.keyshow="other";
        }
    return tmp;
}
