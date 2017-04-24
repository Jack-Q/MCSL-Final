/********************************************************
 * Micro Computer System Lab - Final Project
 *                                               Group 49
 ********************************************************
 * Bluetooth & IR Controller
 ********************************************************/


#ifndef __PACKAGE_H
#define __PACKAGE_H

#include "global_state.h"

void encodeKeyPackage(CTRL_dataPackage_t *pkg);
void encodeMousePackage(CTRL_dataPackage_t *pkg);
void encodeActPackage(CTRL_dataPackage_t *pkg);
void decodePackage(CTRL_dataPackage_t *pkg);
#endif
