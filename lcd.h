/*
 * lcd.h
 *
 *  Created on: 2017.12.18.
 *      Author: David Agoston
 */

#ifndef LCD_H_
#define LCD_H_

#include "mytype.h"

#define LCD_START_ARR 		13
#define LCD_WIDTH 			102
#define LCD_MEMPAGE_DEPTH	8
#define LCD_HEIGHT			64
#define LCD_COL_L			0b0000
#define LCD_COL_H			0b0001
#define LCD_MEMPAGE			0b1011
#define LCD_W_OFFS			0x1E

void lcd_init();
void lcd_clrscr();
void test();
void sendColAddr(uint8_t col);
void sendMemPageAddr(uint8_t memPage);
void sendDataLCD(uint8_t data);
void sendbyteLCD(uint8_t byte);
void initSnake(snek_pos_type head, snek_pos_type tail);
void moveSnek();
void drawSnekHead(snek_pos_type);
void drawSnekTail(snek_pos_type);


#endif /* LCD_H_ */
