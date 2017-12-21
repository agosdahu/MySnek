/*
 * Copyright (c) 2009-2012 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */


#include <stdio.h>
#include "platform.h"
#include "xil_io.h"
#include "xil_printf.h"
#include "lcd.h"
#include "mytype.h"
#include "timer.h"
#include "io_ports.h"
#include "inter.h"
#include "game_logic.h"

void init();

static uint8_t flag = 0;
static snek_pos_type mySnek;

int main()
{
	mySnek.dir = RIGHT;
	uint8_t tmp;
	static uint8_t act_lvl = 1;

	init();

    while(1)
    {
    	tmp = swChanged();
    	if(tmp)
    	{
    		act_lvl = tmp;		//change lvl 1v1
    		chgLVL(act_lvl);
    	}

    	writeLED(act_lvl);
    	mySnek.dir = naviDir();
    	write7Seg(78);
    	if(flag)
    	{
    		// Do the Snek!
    		// DIR kiértékelése
    		// GAME_FIELD-be mentés/KAJA/halál
    		// GAME_FIELD-ben ok -> MEM_FIELDBE
    		// túúlcsorduló pixelek (memPage)
    		// LCD draw

    		clrFlag();
    	}
		timer_test((uint32_t)dir);
    }
    return 0;

}

void setFlag()
{
	flag = 1;
}

void clrFlag()
{
	flag = 0;
}

void init()
{
	snek_pos_type init_head, init_tail;

	init_platform();
	lcd_init();
	lcd_clrscr();
	init_head.pos_x = 13;
	init_head.pos_y = 11;
	init_tail.pos_x = 10;
	init_tail.pos_y = 11;

	mySnek.pos_x = init_head.pos_x;
	mySnek.pos_y = init_head.pos_y;

	initSnake(init_head, init_tail);
	//#TODO SNEK mentése game_field-en és memField-en
	timer_config_int();
}
