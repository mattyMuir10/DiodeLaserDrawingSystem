/*
 * TIMER_0.cpp
 *
 * Created: 2025-03-25 10:40:50 PM
 *  Author: mattm
 */ 

#include "TIMER_0.h"


volatile static uint32_t millis_c = 0;

ISR(TIMER0_COMPA_vect){
	millis_c++;
}

void timer0_init(void){
	 TCCR0A |= (0b10 << WGM00);
	 OCR0A = 249;
	 TIMSK0 |=(1 << OCIE0A);
	 TCCR0B |= (0b011 << CS00);
}

uint32_t millis(void){
	return millis_c;	
}

uint8_t millis_end (uint32_t start_time, uint32_t delay_time){
	
	return ((millis() - start_time) >= delay_time);
}

