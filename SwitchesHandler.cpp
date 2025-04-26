/*
 * SwitchesHandler.cpp
 *
 * Created: 2025-04-02 3:53:05 PM
 *  Author: mattm
 */ 
#include "SwitchesHandler.h"
#include "TIMER_0.h"

volatile static uint8_t firstSwitchWasPressed;
volatile static uint8_t secondSwitchWasPressed;


//interrupt when first switch is pressed
ISR(INT0_vect){
	firstSwitchWasPressed = 1;
}

//interrupt when second switch is pressed
ISR(INT1_vect){
	secondSwitchWasPressed = 1;
}


//enables the switches to be used as interrupts and enable pullups for them
void SwitchesInit(void){
	DDRD &= 0b11110011;
	PORTD |= 0b00001100;
	EICRA |= (0b11 << ISC10) | (0b11 << ISC00);
	EIMSK = 0b00000011;
}

//waits for one switch to be pressed and return a uint8 which is 0 for the first switch and 1 for the second
uint8_t WaitForEitherSwitchPress(uint32_t msMaxTime){
	firstSwitchWasPressed = 0;
	secondSwitchWasPressed = 0;
	uint32_t startTime = millis();
	while ((firstSwitchWasPressed == 0 && secondSwitchWasPressed == 0) && (!millis_end(startTime, msMaxTime) || msMaxTime == 0));
	if (millis_end(startTime, msMaxTime) && msMaxTime != 0){
		return 2;
	}
	return (secondSwitchWasPressed);
}
//waits for the first switch to be pressed then returns
uint8_t WaitForSwitchOnePress(uint32_t msMaxTime){
	firstSwitchWasPressed = 0;
	uint32_t startTime = millis();
	while (firstSwitchWasPressed == 0 && (!millis_end(startTime, msMaxTime) && msMaxTime != 0));
	if (millis_end(startTime, msMaxTime)&& msMaxTime != 0){
		return 0;
	}
	return 1;
}
//waits for the second switch to be pressed then returns
uint8_t WaitForSwitchTwoPress(uint32_t msMaxTime){
	secondSwitchWasPressed = 0;
	uint32_t startTime = millis();
	while (secondSwitchWasPressed == 0 && (!millis_end(startTime, msMaxTime) && msMaxTime != 0));
	if (millis_end(startTime, msMaxTime)&& msMaxTime != 0){
		return 0;
	}
	return 1;
}