/*
 * timer.h
 *
 *  Created on: 2017.12.20.
 *      Author: David Agoston
 */

#ifndef TIMER_H_
#define TIMER_H_

#include "mytype.h"
#include "xtmrctr_l.h"
#include "xintc_l.h"
#include "xil_printf.h"
#include "inter.h"

#define LVL_1	47500000u
#define LVL_2	40000000u
#define LVL_3	33000000u
#define LVL_4	26500000u
#define LVL_5	21500000u
#define LVL_6	15500000u
#define LVL_7	13000000u
#define LVL_8	10000000u

void timer_config_int();
void timer_tickmod(uint32_t tick);
void timer_test(uint32_t testVal);



#endif /* TIMER_H_ */
