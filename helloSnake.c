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

static uint8_t flag = 0;		// INT vezérelt round-robin vezérlõ flag-je
static snek_pos_type mySnek; 	//Fõleg (csak) a dir kell ebbõl...
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
    	tmp = swChanged(); // LVL kapcsolók pollozása
    	if(tmp)
    	{
    		act_lvl = tmp;		//change lvl, ha van változás
    		chgLVL(act_lvl);
    	}

    	writeLED(act_lvl);		// mindig aktuális szint kiírása
    	mySnek.dir = naviDir();	// navigációs gomb pollozása (csak 0-tól eltérõ irány érdekel minket)

    	if(mySnek.dir == CENTER)
    	{
    		newGame(1);
    		reNavi();
    	}

    	if(flag)				// TIMER INT hatására lefut az aktuális paraméterekkel a játék logkája -> képernyõ frissítés
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
	lcd_init();			// kijelzõ INIT
	lcd_clrscr();		// memória hulladék letörlése

	// Kezdõ pozíciók belövése
	init_head.pos_x = 13;
	init_head.pos_y = 11;
	init_tail.pos_x = 10;
	init_tail.pos_y = 11;

	mySnek.pos_x = init_head.pos_x;
	mySnek.pos_y = init_head.pos_y;
	//mySnek.dir = RIGHT; ezt a lépést a navi olvasással eldöntöttük...

	resetGameField();
	initGameField(init_head, init_tail, act_lvl);
	initSnake(init_head, init_tail); 	// lcd-re rajzoljuk a Snek-et
	initFood();							// KAJA random kirajzoltatása

	timer_config_int();	//óra indul
}

void newGame(uint8_t lvl)
{
	snek_pos_type init_head, init_tail;
	// Kezdõ pozíciók belövése
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

	resetGameField();								// játéktér legyalulása
	initGameField(init_head, init_tail, lvl);		// játéktér újrarajzolása,paraméterek
	initSnake(init_head, init_tail); 				// lcd-re rajzoljuk a Snek-et
	initFood();							// KAJA random kirajzoltatása
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
