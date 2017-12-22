/*
 * io_ports.h
 *
 *  Created on: 2017.12.21.
 *      Author: David Agoston
 */

#ifndef IO_PORTS_H_
#define IO_PORTS_H_

#include "mytype.h"
#include <xil_io.h>


#define CPLD_CTRL		0x00
#define CPLD_NAVI		0x04
#define CPLD_DIPSW		0x08
#define CPLD_7SEG_H		0x0C
#define CPLD_7SEG_L		0x10
#define CPLD_LED 		0x14

void reNavi();
uint8_t swChanged();
dir_t naviDir();
void write7Seg(uint8_t num);
void writeLED(uint8_t num);

#endif /* IO_PORTS_H_ */
