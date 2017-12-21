/*
 * game_logic.h
 *
 *  Created on: 2017.12.20.
 *      Author: David Agoston
 */

#ifndef GAME_LOGIC_H_
#define GAME_LOGIC_H_

#include "mytype.h"
#define GAME_FIELD_X	34u
#define GAME_FIELD_Y	21u
#define MEM_FIELD_X		102u
#define MEM_FIELD_Y		8u

void chgLVL(uint8_t next_lvl);
void game(snek_pos_type snek);

#endif /* GAME_LOGIC_H_ */
