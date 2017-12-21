/*
 * io_ports.c
 *
 *  Created on: 2017.12.21.
 *      Author: David Agoston
 */

#include "io_ports.h"

uint8_t swChanged() //visszaadjuk a szint �rt�k�t, ha voltv�ltoz�s, egy�bk�nt meg 0
{
	uint8_t lvl = 0;
	static uint32_t prev_sw = 0;
	uint32_t switches;

	switches = Xil_In32(XPAR_CPLD_0_BASEADDR + CPLD_DIPSW);
	if(prev_sw == switches)
		lvl = 0;
	else
	{
		if (switches & 0x00000080)
		{
			lvl = 8;
		}
		else if (switches & 0x00000040){
			lvl = 7;
		}
		else if (switches & 0x00000020){
			lvl = 6;
		}
		else if (switches & 0x00000010){
			lvl = 5;
		}
		else if (switches & 0x00000008){
			lvl = 4;
		}
		else if (switches & 0x00000004){
			lvl = 3;
		}
		else if (switches & 0x00000002){
			lvl = 2;
		}
		else if (switches & 0x00000001){
			lvl = 1;
		}
		else if (switches == 0)
			lvl = 1;

		prev_sw = switches;
	}

	return (uint8_t)lvl;
}

dir_t naviDir()
{
	uint32_t navi;

	navi = Xil_In32(XPAR_CPLD_0_BASEADDR + CPLD_NAVI);

	return (dir_t)navi;

}

void write7Seg(uint8_t num)
{
	Xil_Out32(XPAR_CPLD_0_BASEADDR + CPLD_7SEG_H, (uint32_t)(num/10));
	Xil_Out32(XPAR_CPLD_0_BASEADDR + CPLD_7SEG_L, (uint32_t)(num%10));
}

void writeLED(uint8_t lvl)
{
	uint32_t leds = 0;
	switch (lvl) {
				case 1:
					leds = 0x01;
					break;
				case 2:
					leds = 0x03;
					break;
				case 3:
					leds = 0x07;
					break;
				case 4:
					leds = 0x0f;
					break;
				case 5:
					leds = 0x1f;
					break;
				case 6:
					leds = 0x3f;
					break;
				case 7:
					leds = 0x7f;
					break;
				case 8:
					leds = 0xff;
					break;
				default:
					leds = 0x01;

			}
			Xil_Out32(XPAR_CPLD_0_BASEADDR + CPLD_LED, leds);
}
