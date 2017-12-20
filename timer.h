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
#include <xparameters.h>
#include "xil_printf.h"

void timer_config_int();
void timer_tickmod(uint32_t tick);
void timer_test(uint32_t testVal);

#endif /* TIMER_H_ */
