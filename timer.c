/*
 * timer.c
 *
 *  Created on: 2017.12.20.
 *      Author: David Agoston
 */

#include "timer.h"

void int_handler(void *instancePtr);

void timer_config_int()
{
    //TIMER INIT
	uint32_t val = 0;
    uint32_t load = 0;

    //10 usec-et bet�lt�nk -> tick id�; (TLRx + 2) / AXI_CLOCK (50MHz)
    load = 49999998; //#TODO ez mennyi legyen 1s ???


    //Load �rt�k bet�lt�se a 0-s timer load regiszter�be
    XTmrCtr_WriteReg(XPAR_AXI_TIMER_0_BASEADDR, 0, XTC_TLR_OFFSET, load);

    //Lefele sz�ml�l� bit �s a bet�lt�s bit maszkol�sa, �s bet�lt�se a st�tusz regiszterbe; interrupt bit t�rl�se
    // + auto reload mask, + interrupt enged�lyez�s
    val = (XTC_CSR_DOWN_COUNT_MASK | XTC_CSR_LOAD_MASK | XTC_CSR_AUTO_RELOAD_MASK | XTC_CSR_ENABLE_INT_MASK);
    XTmrCtr_SetControlStatusReg(XPAR_AXI_TIMER_0_BASEADDR + XTC_TCSR_OFFSET, 0, val);

    //Load bit kiszed�se
    val &= ~XTC_CSR_LOAD_MASK;

    //Timer elind�t�sa
    val |= XTC_CSR_ENABLE_TMR_MASK;

    //St�tusz regiszterbe bet�ltj�k a biteket
    XTmrCtr_SetControlStatusReg(XPAR_AXI_TIMER_0_BASEADDR + XTC_TCSR_OFFSET, 0, val);

    //LCD CLoCK INTERRUPT INIT
    XIntc_RegisterHandler(XPAR_INTC_0_BASEADDR, XPAR_MICROBLAZE_0_INTC_AXI_TIMER_0_INTERRUPT_INTR, (XInterruptHandler) int_handler, NULL);


    XIntc_MasterEnable(XPAR_INTC_0_BASEADDR);

    //Az interrupt enged�lyez�se
    XIntc_EnableIntr(XPAR_INTC_0_BASEADDR, XPAR_AXI_TIMER_0_INTERRUPT_MASK);

    microblaze_enable_interrupts();
}

void timer_tickmod(uint32_t tick)
{
	uint32_t val = 0;

	// Timer le�ll�t�s, hogy �jra t�lthess�k a LOAD regisztert
	val = XTmrCtr_GetControlStatusReg(XPAR_AXI_TIMER_0_BASEADDR, 0);
	val &= ~XTC_CSR_ENABLE_TMR_MASK;
	XTmrCtr_SetControlStatusReg(XPAR_AXI_TIMER_0_BASEADDR + XTC_TCSR_OFFSET, 0, val);

	//Load �rt�k bet�lt�se a 0-s timer load regiszter�be
	XTmrCtr_WriteReg(XPAR_AXI_TIMER_0_BASEADDR, 0, XTC_TLR_OFFSET, tick);

	//LOAD bit-tel bet�ltj�k az �j �rt�ket
	val |= XTC_CSR_LOAD_MASK;
	XTmrCtr_SetControlStatusReg(XPAR_AXI_TIMER_0_BASEADDR + XTC_TCSR_OFFSET, 0, val);

	//Load bit kiszed�se
	val &= ~XTC_CSR_LOAD_MASK;
	//Timer enged�lyez�s
	val |= XTC_CSR_ENABLE_TMR_MASK;
	//St�tusz regiszterbe bet�ltj�k a biteket
	XTmrCtr_SetControlStatusReg(XPAR_AXI_TIMER_0_BASEADDR + XTC_TCSR_OFFSET, 0, val);

}

void timer_test(uint32_t testVal)
{
	static uint8_t flag = 0x00;
	//TEST
	    if((testVal == 0x00000010) && flag)
	    {
	    	flag = 0x00;
	    	xil_printf("FLAG_ZERO\n");
	    }
	    else if((testVal == 0x00000001) && !flag)
	    {
	    	timer_tickmod(49999998);
	    	xil_printf("SLOW\n");
	    	flag = 1;
	    }
	    else if((testVal == 0x00000002) && !flag)
	    {
	    	timer_tickmod(4999998);
	    	xil_printf("FAST\n");
			flag = 1;
	    }
	    else if((testVal == 0x00000004) && !flag)
	        {
	        	timer_tickmod(24999998);
	        	xil_printf("MID\n");
	    		flag = 1;
	        }
}

void int_handler(void *instancePtr)
{
	// #TODO mit csin�ljon ez a sz@r
	setFlag();

    uint32_t csr;
    csr = XTmrCtr_GetControlStatusReg(XPAR_AXI_TIMER_0_BASEADDR, 0);
    XTmrCtr_SetControlStatusReg(XPAR_AXI_TIMER_0_BASEADDR + XTC_TCSR_OFFSET, 0, csr);
}
