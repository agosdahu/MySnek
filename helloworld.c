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

/*
 * helloworld.c: simple test application
 *
 * This application configures UART 16550 to baud rate 9600.
 * PS7 UART (Zynq) is not initialized by this application, since
 * bootrom/bsp configures it to baud rate 115200
 *
 * ------------------------------------------------
 * | UART TYPE   BAUD RATE                        |
 * ------------------------------------------------
 *   uartns550   9600
 *   uartlite    Configurable only in HW design
 *   ps7_uart    115200 (configured by bootrom/bsp)
 */

#include <stdio.h>
#include "platform.h"
#include "xparameters.h"
#include "xil_io.h"
#include "xil_printf.h"
#include "lcd.h"
#include "mytype.h"
#include "timer.h"

int main()
{
    init_platform();
    lcd_init();
    lcd_clrscr();
    initSnake();
    timer_config_int();

    uint32_t leds = 0;
    uint32_t switches = 0;
    uint8_t flag = 0x00;

    while(1) {

    Xil_Out32(XPAR_CPLD_0_BASEADDR + 0x0C, 0x04);
    Xil_Out32(XPAR_CPLD_0_BASEADDR + 0x10, 0x05);

    switches = Xil_In32(XPAR_CPLD_0_BASEADDR + 0x08);
    Xil_Out32(XPAR_CPLD_0_BASEADDR + 0x14, switches);
    leds = Xil_In32(XPAR_CPLD_0_BASEADDR + 0x04);
   // xil_printf("ledek: %08x\n", leds);

    //TEST
    if((leds == 0xEE000010) && flag)
    {
    	flag = 0x00;
    	xil_printf("FLAG_ZERO\n");
    }
    else if((leds == 0xEE000001) && !flag)
    {
    	timer_tickmod(49998000);
    	xil_printf("SLOW\n");
    	flag = 1;
    }
    else if((leds == 0xEE000002) && !flag)
    {
    	timer_tickmod(4999800);
    	xil_printf("FAST\n");
		flag = 1;
    }
    else if((leds == 0xEE000004) && !flag)
        {
        	timer_tickmod(24999000);
        	xil_printf("MID\n");
    		flag = 1;
        }



    }
    return 0;

}
