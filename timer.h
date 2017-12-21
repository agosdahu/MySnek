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

#define LVL_1	47500000u
#define LVL_2	42500000u
#define LVL_3	37500000u
#define LVL_4	32500000u
#define LVL_5	27500000u
#define LVL_6	22500000u
#define LVL_7	17500000u
#define LVL_8	12500000u

void timer_config_int();
void timer_tickmod(uint32_t tick);
void timer_test(uint32_t testVal);



#endif /* TIMER_H_ */
