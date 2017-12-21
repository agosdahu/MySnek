/*
 * mytype.h
 *
 *  Created on: 2017.12.20.
 *      Author: David Agoston
 */

#ifndef MYTYPE_H_
#define MYTYPE_H_

#include "xparameters.h"
#include "enum.h"

//*****************************************************************************
//* Makrók a memória írásához és olvasásához.                                 *
//*****************************************************************************
#define MEM32(addr)  (*(volatile unsigned long *)(addr))

#define DIR_UP 		(1 << 0)
#define DIR_DOWN 	(1 << 1)
#define DIR_LEFT	(1 << 2)
#define DIR_RIGHT	(1 << 3)
#define DIR_CENTER	(1 << 4)

typedef unsigned long uint32_t;
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;

typedef struct POSITION{
	uint8_t pos_x;
	uint8_t pos_y;
	dir_t dir;
	uint8_t byte;
} snek_pos_type;

#endif /* MYTYPE_H_ */
