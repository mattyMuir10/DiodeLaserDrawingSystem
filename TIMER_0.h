/*
 * TIMER_0.h
 *
 * Created: 2025-03-25 10:40:32 PM
 *  Author: mattm
 */ 


#ifndef TIMER_0_H_
#define TIMER_0_H_

#include <stdint.h>
#include <stdio.h>

#include "config.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void timer0_init(void);
uint32_t millis(void);
uint8_t millis_end(uint32_t start_time, uint32_t delay_time);
void delay_ms_custom(uint32_t ms_delay);



#endif /* TIMER_0_H_ */