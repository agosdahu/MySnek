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

void initSnake(snek_pos_type head, snek_pos_type tail)
{
	volatile static int i;
	uint8_t snek_byte, col, memPage, y_bit;

	col = tail.pos_x*3;
	memPage = (tail.pos_y*3)/8;
	y_bit = (tail.pos_y*3)%8;

	sendMemPageAddr(memPage);
	sendColAddr(col);
	snek_byte = ((1 << y_bit) |  (1 << (y_bit+1)) | (1 << (y_bit+2)));

	for(i=0; i<((head.pos_x - tail.pos_x)+1)*3; i++)
	{
		sendDataLCD(snek_byte);
	}
}

void drawSnek(snek_pos_type snek)
{
	volatile static int j;
	uint8_t snek_byte, col, memPage;

	col = snek.pos_x*3;
	memPage = (snek.pos_y*3)/8;
	sendColAddr(col);
	sendMemPageAddr(memPage);
	snek_byte = snek.byte;
	for(j=0; j<3; j++)
	{
		sendDataLCD(snek_byte);
	}
}

void test()
{
	static uint8_t memPage = 0;
	static uint8_t col = 0;
	static uint8_t byte = 0x01;
	static uint8_t init_flag = 1;


	if(init_flag)
	{
		sendMemPageAddr(memPage);
		sendColAddr(col);
		init_flag = 0;
	}

	if(col >= 102u)
	{
		memPage++;
		col = 0;
	}

	if(memPage >= 8)
	{
		memPage = 0;
		col = 0;
		byte = 0x01;
	}
	else
	{
		col++;
		byte = byte << 1;
		if(byte == 0)
			byte = 0x01;

	}

	sendMemPageAddr(memPage);
	sendColAddr(col);
	sendDataLCD(byte);
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
