/*
 * game_logic.c
 *
 *  Created on: 2017.12.20.
 *      Author: David Agoston
 */
#include "game_logic.h"

static uint8_t game_field[GAME_FIELD_X][GAME_FIELD_Y]; 	// 34*21 0->nothing, 1->snake, 2->food
static uint8_t mem_field[MEM_FIELD_X][MEM_FIELD_Y];		// 102*8
static dir_t trail[GAME_FIELD_X][GAME_FIELD_Y];

static snek_pos_type head;
static snek_pos_type tail;
static uint8_t rand_flag = 1;



void gameStep(snek_pos_type snek)
{
	head.dir = conditionDir(snek.dir);			//irány frissítése


	/* FALBA ÜTKÖZÜNK?*/
	if(checkIfWall(head))
	{//#TODO HALÁL
		gg();
	}

	if(checkIfFood(head))
	{//#TODO KAJA
		//plussz pont 99+ -> szintlépés
		pointPlus();   			//új kaja generálás included
		/*kígyó növelés*/
		updateHead();
		drawGameField(head, 1);
	}
	else
	{
		updateHead();
		drawGameField(head, 1);
		eraseTailGameField();
		updateTail();
	}
}

void updateHead()
{
	switch(head.dir)
	{
		case UP:
			head.pos_y -= 1;
			break;
		case DOWN:
			head.pos_y += 1;
			break;
		case LEFT:
			head.pos_x -= 1;
			break;
		case RIGHT:
			head.pos_x += 1;
			break;
		default:
			break;
	}
}

void updateTail()
{
	/* ha az adott mezõn sikerült */
	if(trail[tail.pos_x][tail.pos_y])
	{
		tail.dir = trail[tail.pos_x][tail.pos_y];	// DIR frissítés
		trail[tail.pos_x][tail.pos_y] = 0;			//ezt a fordulást sikerült megtenni, ha legközelebb erre jövünk, akkor már ne.
	}

	switch(tail.dir)
	{
		case UP:
			tail.pos_y -= 1;
			break;
		case DOWN:
			tail.pos_y += 1;
			break;
		case LEFT:
			tail.pos_x -= 1;
			break;
		case RIGHT:
			tail.pos_x += 1;
			break;
		default:
			break;
	}
}

dir_t conditionDir(dir_t act_dir)
{
	dir_t prev_dir = head.dir;

	switch (prev_dir)
	{
		case UP:
			if((act_dir == LEFT) || (act_dir == RIGHT))
				prev_dir = act_dir;
			break;
		case DOWN:
			if((act_dir == LEFT) || (act_dir == RIGHT))
				prev_dir = act_dir;
			break;
		case LEFT:
			if((act_dir == UP) || (act_dir == DOWN))
				prev_dir = act_dir;
			break;
		case RIGHT:
			if((act_dir == UP) || (act_dir == DOWN))
				prev_dir = act_dir;
			break;
		case CENTER:
			break;
	}
	/*  el kell menteni a farok számára a fordulási útvonalakat  */
	if(head.dir != prev_dir)
		trail[head.pos_x][head.pos_y] = prev_dir;

	return prev_dir;
}

uint8_t checkIfWall(snek_pos_type snek) 		// return 1 if next step is invalid
{
	uint8_t next_x, next_y, death;
	switch(snek.dir)
	{
	case UP:
		next_x = snek.pos_x;
		next_y = snek.pos_y - 1;
		break;
	case DOWN:
		next_x = snek.pos_x;
		next_y = snek.pos_y + 1;
		break;
	case LEFT:
		next_x = snek.pos_x - 1;
		next_y = snek.pos_y;
		break;
	case RIGHT:
		next_x = snek.pos_x + 1;
		next_y = snek.pos_y;
		break;
	default:
		next_x = snek.pos_x;
		next_y = snek.pos_y;
		break;
	}
	if((next_x >= GAME_FIELD_X) || (next_y >= GAME_FIELD_Y))
		death = 1;
	else if(game_field[next_x][next_y] == 1)
		death = 1;
	else
		death = 0;

	return death;
}

uint8_t checkIfFood(snek_pos_type snek)
{
	uint8_t next_x, next_y, food;
		switch(snek.dir)
		{
			case UP:
				next_x = snek.pos_x;
				next_y = snek.pos_y - 1;
				break;
			case DOWN:
				next_x = snek.pos_x;
				next_y = snek.pos_y + 1;
				break;
			case LEFT:
				next_x = snek.pos_x - 1;
				next_y = snek.pos_y;
				break;
			case RIGHT:
				next_x = snek.pos_x + 1;
				next_y = snek.pos_y;
				break;
			default:
				next_x = snek.pos_x;
				next_y = snek.pos_y;
				break;
		}

		if(game_field[next_x][next_y] == 2)
			food = 1;
		else
			food = 0;

		return food;
}

void drawGameField(snek_pos_type snek, uint8_t data) // DATA 0->nothing, 1->snake, 2->food
{
	if(data > 0)
	{
		game_field[snek.pos_x][snek.pos_y] = data;
		drawMemField(snek, data);
	}
}

void drawMemField(snek_pos_type snek, uint8_t data) // DATA [0/1] -> drawing snake, [2] -> drawing food [ALSO - LCD drawing]
{
	volatile uint8_t i;
	volatile uint8_t food [3];
	volatile uint8_t ovf_food [3];

	uint8_t col, memPage, y_bit;
	uint8_t ovf = 0;
	uint8_t snek_byte = 0;
	uint8_t ovf_byte = 0;

	for(i = 0; i<3; i++)
	{
		food[i] = 0;
		ovf_food[i] = 0;
	}

	col = snek.pos_x*3;				// sor kiszámítása
	memPage = (snek.pos_y*3)/8;		// memPage
	y_bit = (snek.pos_y*3)%8;			// 3*3-as bal oszlopának LCD-n felsõ bit-je az adott memPage-en

/* SNEK DRAWING */
	if(data == 1)							// azaz SNEK rajzolás megy
	{
		// lap-túlcsordulás védelem
		if(y_bit < 6)  // nincs OVF
		{
			snek_byte = ((1 << y_bit) |  (1 << (y_bit+1)) | (1 << (y_bit+2)));
			ovf = 0;
		}
		else if(y_bit == 6) // 1db ovfbit
		{
			snek_byte = ((1 << y_bit) |  (1 << (y_bit+1)));
			ovf_byte = ((1 << 0));
			ovf = 1;
		}
		else	//y_bit == 7, 2db of bit
		{
			snek_byte = (1 << y_bit);
			ovf_byte = ((1 << 0) | (1 << 1));
			ovf = 1;
		}

/* 3*3-as kocka rajzolása */
		if(ovf) // bonyi rajzolás
		{
			sendMemPageAddr(memPage);
			sendColAddr(col);
			for(i = 0; i < 3; i++)
			{
				mem_field[col + i][memPage] = (mem_field[col + i][memPage] | snek_byte );
				sendDataLCD(mem_field[col + i][memPage]);
			}

			sendMemPageAddr(memPage+1);
			sendColAddr(col);
			for(i = 0; i < 3; i++)
			{
				mem_field[col + i][memPage+1] = (mem_field[col + i][memPage+1] | ovf_byte);
				sendDataLCD(mem_field[col + i][memPage+1]);
			}
		}
		else	// A 3*3 négyzet egy memPage-en van
		{
			sendMemPageAddr(memPage);
			sendColAddr(col);

			for(i = 0; i < 3; i++)
			{
				mem_field[col + i][memPage] = (mem_field[col +i][memPage] | snek_byte );
				sendDataLCD(mem_field[col + i][memPage]);
			}
		}
	}
/* FOOD DRAWING */
	else if (data == 2)
	{
		// lap-túlcsordulás védelem
		if(y_bit < 6)  // nincs OVF
		{
			food[0] = ((0 << y_bit) |  (1 << (y_bit+1)) | (0 << (y_bit+2)));
			food[1] = ((1 << y_bit) |  (0 << (y_bit+1)) | (1 << (y_bit+2)));
			food[2] = ((0 << y_bit) |  (1 << (y_bit+1)) | (0 << (y_bit+2)));
			ovf = 0;
		}
		else if(y_bit == 6) // 1db ovfbit
		{
			food[0] = ((0 << y_bit) | (1 << (y_bit+1)));
			food[1] = ((1 << y_bit) | (0 << (y_bit+1)));
			food[2] = ((0 << y_bit) | (1 << (y_bit+1)));

			ovf_food[0] = (0 << 0);
			ovf_food[1] = (1 << 0);
			ovf_food[2] = (0 << 0);
			ovf = 1;
		}
		else	//y_bit == 7, 2db of bit
		{
			food[0] = (0 << y_bit);
			food[1] = (1 << y_bit);
			food[2] = (0 << y_bit);

			ovf_food[0] = ((1 << 0) |  (0 << 1));
			ovf_food[1] = ((0 << 0) |  (1 << 1));
			ovf_food[2] = ((1 << 0) |  (0 << 1));
			ovf = 1;
		}

/* 3*3-as kocka rajzolása */
		if(ovf) // bonyi rajzolás
		{
			sendMemPageAddr(memPage);
			sendColAddr(col);
			for(i = 0; i < 3; i++)
			{
				mem_field[col +i][memPage] = (mem_field[col +i][memPage] | food[i] );
				sendDataLCD(mem_field[col +i][memPage]);
			}

			sendMemPageAddr(memPage+1);
			sendColAddr(col);
			for(i = 0; i < 3; i++)
			{
				mem_field[col +i][memPage+1] = (mem_field[col +i][memPage+1] | ovf_food[i]);
				sendDataLCD(mem_field[col +i][memPage+1]);
			}
		}
		else	// A 3*3 négyzet egy memPage-en van
		{
			sendMemPageAddr(memPage);
			sendColAddr(col);

			for(i = 0; i < 3; i++)
			{
				mem_field[col +i][memPage] = (mem_field[col +i][memPage] | food[i] );
				sendDataLCD(mem_field[col +i][memPage]);
			}
		}
	}
}

void eraseTailGameField()
{
	game_field[tail.pos_x][tail.pos_y] = 0;
	eraseTailMemField();
}

void eraseTailMemField()
{
	volatile uint8_t i;
	uint8_t col, memPage, y_bit;
	uint8_t ovf = 0;
	uint8_t snek_byte = 0xFF;
	uint8_t ovf_byte = 0xFF;

	col = tail.pos_x*3;				// sor kiszámítása
	memPage = (tail.pos_y*3)/8;		// memPage
	y_bit = (tail.pos_y*3)%8;		// 3*3-as bal oszlopának LCD-n felsõ bit-je az adott memPage-en

	// lap-túlcsordulás védelem
	if(y_bit < 6)  // nincs OVF
	{
		snek_byte = ~snek_byte | ~((1 << y_bit) |  (1 << (y_bit+1)) | (1 << (y_bit+2)));
		ovf = 0;
	}
	else if(y_bit == 6) // 1db ovfbit
	{
		snek_byte = ~snek_byte | ~((1 << y_bit) |  (1 << (y_bit+1)));
		ovf_byte = ~ovf_byte | ~((1 << 0));
		ovf = 1;
	}
	else	//y_bit == 7, 2db ovf bit
	{
		snek_byte = ~snek_byte | ~(1 << y_bit);
		ovf_byte = ~ovf_byte | ~((1 << 0) | (1 << 1));
		ovf = 1;
	}

	/* 3*3-as kocka rajzolása */
	if(ovf) // bonyi rajzolás
	{
		sendMemPageAddr(memPage);
		sendColAddr(col);
		for(i = 0; i < 3; i++)
		{
			mem_field[col +i][memPage] = (mem_field[col +i][memPage] & snek_byte );
			sendDataLCD(mem_field[col +i][memPage]);
		}

		sendMemPageAddr(memPage+1);
		sendColAddr(col);
		for(i = 0; i < 3; i++)
		{
			mem_field[col +i][memPage+1] = (mem_field[col +i][memPage+1] & ovf_byte);
			sendDataLCD(mem_field[col +i][memPage+1]);
		}
	}
	else	// A 3*3 négyzet egy memPage-en van
	{
		sendMemPageAddr(memPage);
		sendColAddr(col);

		for(i = 0; i < 3; i++)
		{
			mem_field[col +i][memPage] = (mem_field[col +i][memPage] & snek_byte );
			sendDataLCD(mem_field[col +i][memPage]);
		}
	}
}

void initFood()
{
	uint8_t food_x, food_y;
	uint8_t regen;
	snek_pos_type food_snek;

	if(rand_flag)
		{
			srand(time(NULL));
			rand_flag = 0;
		}
	// lövünk egyet 0-33 ill. 0-20 között
		food_x = rand() % 33;
		food_y = rand() % 20;
	do {
		if(game_field[food_x][food_y] == 1)		//Van itt kígyó
		{
			regen = 1;							// "one more time" - DaftPunk
			// végig lépkedünk a játéktéren keresve a jó helyet.
			if(++food_x == 34)
			{
				food_x = 0;
				if(++food_y == 21)
					food_y = 0;
			}
		}
		else
		{
			regen = 0;
		}

	} while (regen);

	food_snek.pos_x = food_x;
	food_snek.pos_y = food_y;
	drawGameField(food_snek, 2);
}

void initGameField(snek_pos_type init_head, snek_pos_type init_tail, uint8_t lvl)
{
	volatile uint8_t i;
	uint8_t snek_byte, col, memPage, y_bit;

	col = init_tail.pos_x*3;				// sor kiszámítása
	memPage = (init_tail.pos_y*3)/8;		// memPage
	y_bit = (init_tail.pos_y*3)%8;			// 3*3-as bal oszlopának LCD-n felsõ bit-je az adott memPage-en

	snek_byte = ((1 << y_bit) |  (1 << (y_bit+1)) | (1 << (y_bit+2)));	//memField bitjei (most nem kell lap-túlcsordulás check, mert init)

	/* ITT A JÁTÉKTÉRBE BELEKERÜL A KEZDETI SNEK  */
	for(i=0; i<((init_head.pos_x - init_tail.pos_x)+1); i++)
	{
		game_field[init_tail.pos_x + i][init_tail.pos_y] = 1;			//gameField írás
		mem_field[(col+i*3)][memPage] = snek_byte;						//mem_field írás
		mem_field[(col+i*3)+1][memPage] = snek_byte;					//mem_field írás
		mem_field[(col+i*3)+2][memPage] = snek_byte;					//mem_field írás

	}

	// ITT ADJUK ÁT A LOKÁLIS SNEK PARAMÉTEREIT
	head.pos_x = init_head.pos_x;
	head.pos_y = init_head.pos_y;
	head.dir = RIGHT;				//mert csak
	tail.pos_x = init_tail.pos_x;
	tail.pos_y = init_tail.pos_y;
	tail.dir = RIGHT;
	chgLVL(lvl);					//mert init

}

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

void resetGameField()
{
	/* Csak a memóriák kinullázása */

	volatile int i, j;

	for(i=0; i<GAME_FIELD_X; i++)				//GAME_FIELD RESET
	{
		for(j=0; j<GAME_FIELD_Y; j++)
		{
			game_field[i][j] = 0;
			trail[i][j] = 0;
		}
	}
	for(i=0; i<MEM_FIELD_X; i++)				//MEM_FIELD RESET
	{
		for(j=0; j<MEM_FIELD_Y; j++)
		{
			mem_field[i][j] = 0;
		}
	}
}
