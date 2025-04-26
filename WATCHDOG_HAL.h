/*
 * WATCHDOG_HAL.h
 *
 * Created: 2025-03-29 5:00:40 PM
 *  Author: mattm
 */ 


#ifndef WATCHDOG_HAL_H_
#define WATCHDOG_HAL_H_

#include <stdint.h>

#include "config.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define wdr() __asm__ __volatile__ ("wdr")

typedef enum {
	WDT_TIMEOUT_16ms,
	WDT_TIMEOUT_32ms,
	WDT_TIMEOUT_64ms,
	WDT_TIMEOUT_125ms,
	WDT_TIMEOUT_250ms,
	WDT_TIMEOUT_500ms,
	WDT_TIMEOUT_1s,
	WDT_TIMEOUT_2s,
	WDT_TIMEOUT_4s,
	WDT_TIMEOUT_8s,
	}wdt_timeout_e;

void WDT_off(int8_t int_enable);
void WDT_Enable(void);
void WDT_Prescaler_Change(uint8_t int_enable, wdt_timeout_e timeout);

#endif /* WATCHDOG_HAL_H_ */