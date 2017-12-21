/*
 * game_logic.c
 *
 *  Created on: 2017.12.20.
 *      Author: David Agoston
 */
#include "game_logic.h"
#include "timer.h"

static uint8_t game_field[GAME_FIELD_X][GAME_FIELD_Y];
static uint8_t mem_field[MEM_FIELD_X][MEM_FIELD_Y];


static snek_pos_type head;
static snek_pos_type tail;

void chgLVL(uint8_t next_lvl)
{
	uint32_t ctr;

	switch (next_lvl)
		{
		case 1:
			ctr = LVL_1;
			break;
		case 2:
			ctr = LVL_2;
			break;
		case 3:
			ctr = LVL_3;
			break;
		case 4:
			ctr = LVL_4;
			break;
		case 5:
			ctr = LVL_5;
			break;
		case 6:
			ctr = LVL_6;
			break;
		case 7:
			ctr = LVL_7;
			break;
		case 8:
			ctr = LVL_8;
			break;
		default:
			ctr = LVL_1;
		}
	timer_tickmod(ctr);
}


