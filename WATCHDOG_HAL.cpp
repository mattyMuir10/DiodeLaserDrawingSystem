/*
 * WATCHDOG_HAL.cpp
 *
 * Created: 2025-03-29 5:01:06 PM
 *  Author: mattm
 */ 
#include "WATCHDOG_HAL.h"

void WDT_off(int8_t int_enable)
{
	
	
	if (int_enable != 0){
		cli();
	}
	
	wdr();
	/* Clear WDRF in MCUSR */
	MCUSR &= ~(1<<WDRF);
	/* Write logical one to WDCE and WDE */
	/* Keep old prescaler setting to prevent unintentional time-out
	*/
	WDTCSR |= (1<<WDCE) | (1<<WDE);
	/* Turn off WDT */
	WDTCSR = 0x00;
	
	if (int_enable != 0){
		sei();
	}
	
	
}

void WDT_Enable(void){
	wdr();
	MCUSR &= ~(1 << WDRF);
	WDTCSR |= (1 << WDCE) | (1 << WDE);
	WDTCSR |= (1 << WDE);
}

void WDT_Prescaler_Change(uint8_t int_enable, wdt_timeout_e timeout)
{
	if (int_enable != 0){
		cli();
	}
	
	wdr();
	/* Start timed equence */
	WDTCSR |= (1<<WDCE) | (1<<WDE);
	/* Set new prescaler(time-out) value = 64K cycles (~0.5 s) */
	WDTCSR = (1 << WDE) | (timeout << WDP0);
	if (int_enable != 0){
		sei();
	}
}

