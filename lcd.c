/*
 * lcd.c
 *
 *  Created on: 2017.12.18.
 *      Author: David Agoston
 */

#include "lcd.h"
#include "enum.h"
#include <xparameters.h>
#include <xil_printf.h>
#include "xil_io.h"
#include "xtmrctr_l.h"
#include "xintc_l.h"

void lcd_init()
{
	uint32_t lcd_start [LCD_START_ARR] = {0x40, 0xA0, 0xC8, 0xA4, 0xA6, 0xA2, 0x2F, 0x27, 0x81, 0x10, 0xFA, 0x90, 0xAF};
	uint32_t act_reg;
	int i;
	for (i = 0; i < LCD_START_ARR; i++)
	{
		MEM32(XPAR_MYLCD_DRIVER_0_BASEADDR) = lcd_start[i];
		do
		{
			act_reg = MEM32(XPAR_MYLCD_DRIVER_0_BASEADDR);
		}
		while(act_reg & 0x200);	//BUSY?
	}
}

void initSnake()
{
	volatile static int i,j;
	uint8_t snake_byte, col, memPage, y_bit, x_head, x_tail, y_tail;
	x_tail = 9;
	y_tail = 11;
	x_head = 20;

	col = x_tail*3;
	memPage = (y_tail*3)/8;
	y_bit = (y_tail*3)%8;

	sendMemPageAddr(memPage);
	sendColAddr(col);
	snake_byte = ((1 << y_bit) |  (1 << (y_bit+1)) | (1 << (y_bit+2)));

	for(i=0; i<((x_head - x_tail)+1)*3; i++)
	{
		sendDataLCD(snake_byte);
	}

	for(i=0; i<10; i++)
	{
		//for(j=0; j<1000000; j++){}

	}
}
void moveSnek()
{

}

void drawSnekHead(snek_pos_type head)
{
	volatile static int j;
	uint8_t snake_byte, col, memPage, y_bit;

	col = head.pos_x*3;
	memPage = (head.pos_y*3)/8;
	y_bit = (head.pos_y*3)%8;

	sendColAddr(col);

	switch(head.dir)
	{
		case UP:

			break;

		case DOWN:
			break;

		case LEFT:
			break;

		case RIGHT:
			break;

		default:
			break;
	};


	snake_byte = ((1 << y_bit) |  (1 << (y_bit+1)) | (1 << (y_bit+2)));
	for(j=0; j<3; j++)
	{
		sendDataLCD(snake_byte);
	}

	switch(head.dir)
	{
		case UP:

			break;

		case DOWN:
			break;

		case LEFT:
			break;

		case RIGHT:
			break;

		default:
			break;
	};
}

void drawSnekTail(snek_pos_type tail)
{
	volatile static int j;
	uint8_t snake_byte, col, memPage, y_bit;

	col = tail.pos_x*3;
	memPage = (tail.pos_y*3)/8;
	y_bit = (tail.pos_y*3)%8;

	sendColAddr(col);
	sendMemPageAddr(memPage);
	snake_byte = ((0 << y_bit) |  (0 << (y_bit+1)) | (0 << (y_bit+2)));

	for(j=0; j<3; j++)
	{
		sendDataLCD(snake_byte);
	}
}

void test()
{
	static int i = 0;
	volatile uint8_t k;

	//sendbyteLCD(0xB4);
	//sendbyteLCD(0x08);
	//sendbyteLCD(0x12);
	for(k=0; k<50; k++)
	{
		sendDataLCD(0xE7);
	}

	if(++i%2)
		sendbyteLCD(0xA7);
	else
		sendbyteLCD(0xA6);

	sendMemPageAddr(0x04);
	for(k = 0; k < 10; k++)
	{
		sendColAddr(0x05);
		sendDataLCD(0xE7);
	}
	//xil_printf("%i", i);
}

void lcd_clrscr()
{
	uint8_t i, j;
	for(j = 0; j<LCD_MEMPAGE_DEPTH; j++)
	{
		sendMemPageAddr(j);
		sendColAddr(0x00);
		for(i = 0; i < LCD_WIDTH; i++)
		{
			sendDataLCD(0x00);
		}
	}
}

void sendColAddr(uint8_t col)
{
	uint32_t tmp, send;
	uint8_t col_H, col_L;
	//What is COL? Baby don't hurt me, don't hurt me, no more...
	if (col < LCD_WIDTH)
		col = col + LCD_W_OFFS;
	else
		col = LCD_WIDTH-1 + LCD_W_OFFS;

	col_L = col & 0x0F;
	col_H = (0x10 | (col >> 4));
	//SEND LOW_BYTE
	do tmp = MEM32(XPAR_MYLCD_DRIVER_0_BASEADDR);
		while(tmp & 0x200);	//BUSY?

	send = (uint32_t)col_L;
	MEM32(XPAR_MYLCD_DRIVER_0_BASEADDR) = send;
	//tmp = MEM32(XPAR_MYLCD_DRIVER_0_BASEADDR);
	//xil_printf("post-col_L is: %08x\n", tmp);

	//SEND HIGH_BYTE
	do tmp = MEM32(XPAR_MYLCD_DRIVER_0_BASEADDR);
		while(tmp & 0x200);	//BUSY?

	send = (uint32_t)col_H;
	MEM32(XPAR_MYLCD_DRIVER_0_BASEADDR) = send;

	//tmp = MEM32(XPAR_MYLCD_DRIVER_0_BASEADDR);
	//xil_printf("post-col_H is: %08x\n", tmp);
}
void sendMemPageAddr(uint8_t memPage)
{
	uint32_t tmp, send;
	uint8_t page;
	//What is memPage?
	page = (memPage < LCD_MEMPAGE_DEPTH)? memPage : LCD_MEMPAGE_DEPTH;

	do {
		tmp = MEM32(XPAR_MYLCD_DRIVER_0_BASEADDR);
		//xil_printf("pre-memPage is: %08x\n", tmp);
	}
		while(tmp & 0x200);	//BUSY?

	send = (uint32_t)(0xB0 | page);
	MEM32(XPAR_MYLCD_DRIVER_0_BASEADDR) = send;

	//tmp = MEM32(XPAR_MYLCD_DRIVER_0_BASEADDR);
	//xil_printf("post-memPage is: %08x\n", tmp);
}
void sendDataLCD(uint8_t data)
{
	uint32_t tmp;
	uint32_t send = 0;
	send = (send | (1 << 8) | data);

	do tmp = MEM32(XPAR_MYLCD_DRIVER_0_BASEADDR);
		while(tmp & 0x200);	//BUSY?

	MEM32(XPAR_MYLCD_DRIVER_0_BASEADDR) = send;

	//tmp = MEM32(XPAR_MYLCD_DRIVER_0_BASEADDR);
	//xil_printf("post-data is: %08x\n", tmp);
}

void sendbyteLCD(uint8_t byte)
{
	uint32_t tmp, send;

	do tmp = MEM32(XPAR_MYLCD_DRIVER_0_BASEADDR);
		while(tmp & 0x200);	//BUSY?

	send = (uint32_t)byte;
	MEM32(XPAR_MYLCD_DRIVER_0_BASEADDR) = send;

	//tmp = MEM32(XPAR_MYLCD_DRIVER_0_BASEADDR);
	//xil_printf("post-byte is: %08x\n", tmp);
}
