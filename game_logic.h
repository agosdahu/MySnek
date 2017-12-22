/*
 * game_logic.h
 *
 *  Created on: 2017.12.20.
 *      Author: David Agoston
 */

#ifndef GAME_LOGIC_H_
#define GAME_LOGIC_H_

#include "mytype.h"
#include "timer.h"
#include "lcd.h"
#include "inter.h"
#include <stdlib.h>
#include <time.h>

#define GAME_FIELD_X	34u
#define GAME_FIELD_Y	21u
#define MEM_FIELD_X		102u
#define MEM_FIELD_Y		8u

void chgLVL(uint8_t next_lvl);
void gameStep(snek_pos_type snek);
void resetGameField();
void initGameField(snek_pos_type init_head, snek_pos_type init_tail, uint8_t lvl);
void initFood();

void updateHead();
void updateTail();

void drawGameField(snek_pos_type snek, uint8_t data);
void drawMemField(snek_pos_type snek, uint8_t data);
void eraseTailGameField();
void eraseTailMemField();

dir_t conditionDir(dir_t act_dir);
uint8_t checkIfWall(snek_pos_type snek);
uint8_t checkIfFood(snek_pos_type snek);

#endif /* GAME_LOGIC_H_ */
