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
void newGame(uint8_t lvl);

static uint8_t flag = 0;		// INT vez�relt round-robin vez�rl� flag-je
static snek_pos_type mySnek; 	//F�leg (csak) a dir kell ebb�l...
static uint8_t act_lvl = 1;
static uint8_t act_pnt = 98;
static uint8_t end_of_game = 0;

int main()
{
	uint8_t tmp;

	init();

    while(1)
    {
    	write7Seg(act_pnt);
    	tmp = swChanged(); // LVL kapcsol�k polloz�sa
    	if(tmp)
    	{
    		act_lvl = tmp;		//change lvl, ha van v�ltoz�s
    		chgLVL(act_lvl);
    	}

    	writeLED(act_lvl);		// mindig aktu�lis szint ki�r�sa
    	mySnek.dir = naviDir();	// navig�ci�s gomb polloz�sa (csak 0-t�l elt�r� ir�ny �rdekel minket)

    	if(mySnek.dir == CENTER)
    	{
    		newGame(1);
    		reNavi();
    	}

    	if(flag)				// TIMER INT hat�s�ra lefut az aktu�lis param�terekkel a j�t�k logk�ja -> k�perny� friss�t�s
    	{
    		if(end_of_game)
    		{
    			test();
    		}

    		else
    			gameStep(mySnek);

    		clrFlag();
    	}
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

void levelUP()
{
	if(act_lvl < 8)
	{
		act_lvl++;
		lcd_clrscr();
		chgLVL(act_lvl);
		newGame(act_lvl);
	}
}

void init()
{
	snek_pos_type init_head, init_tail;

	//init_platform();
	lcd_init();			// kijelz� INIT
	lcd_clrscr();		// mem�ria hullad�k let�rl�se

	// Kezd� poz�ci�k bel�v�se
	init_head.pos_x = 13;
	init_head.pos_y = 11;
	init_tail.pos_x = 10;
	init_tail.pos_y = 11;

	mySnek.pos_x = init_head.pos_x;
	mySnek.pos_y = init_head.pos_y;
	//mySnek.dir = RIGHT; ezt a l�p�st a navi olvas�ssal eld�nt�tt�k...

	resetGameField();
	initGameField(init_head, init_tail, act_lvl);
	initSnake(init_head, init_tail); 	// lcd-re rajzoljuk a Snek-et
	initFood();							// KAJA random kirajzoltat�sa

	timer_config_int();	//�ra indul
}

void newGame(uint8_t lvl)
{
	snek_pos_type init_head, init_tail;
	// Kezd� poz�ci�k bel�v�se
	init_head.pos_x = 13;
	init_head.pos_y = 11;
	init_tail.pos_x = 10;
	init_tail.pos_y = 11;

	mySnek.pos_x = init_head.pos_x;
	mySnek.pos_y = init_head.pos_y;
	mySnek.dir = RIGHT;

	lcd_clrscr();
	act_lvl = lvl;
	clrPoints();
	end_of_game = 0;

	resetGameField();								// j�t�kt�r legyalul�sa
	initGameField(init_head, init_tail, lvl);		// j�t�kt�r �jrarajzol�sa,param�terek
	initSnake(init_head, init_tail); 				// lcd-re rajzoljuk a Snek-et
	initFood();							// KAJA random kirajzoltat�sa
}

void clrPoints()
{
	act_pnt = 0;
}

void gg()
{
	end_of_game = 1;
	timer_tickmod(500000);
}

void pointPlus()
{
	if(act_pnt == 99)
	{
		if(act_lvl == 8)
		{
			gg();
		}
		else
		{
			levelUP();
			clrPoints();
		}
	}
	else
	{
		act_pnt++;
		initFood();
	}
}
