/*
 * SwitchesHandler.h
 *
 * Created: 2025-04-02 3:52:43 PM
 *  Author: mattm
 */ 


#ifndef SWITCHESHANDLER_H_
#define SWITCHESHANDLER_H_

#include <stdint.h>

#include "config.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


void SwitchesInit(void);
uint8_t WaitForEitherSwitchPress(uint32_t msMaxTime);
uint8_t WaitForSwitchOnePress(uint32_t msMaxTime);
uint8_t WaitForSwitchTwoPress(uint32_t msMaxTime);





#endif /* SWITCHESHANDLER_H_ */