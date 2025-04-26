/*
 * Setup.cpp
 *
 * Created: 2025-03-22 11:56:35 PM
 * Author : mattm
 */ 
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "config.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "UART_HAL.h"
#include "TIMER_0.h"
#include "SwitchesHandler.h"
#include "Stepper.h"
#include "DiodeDrawing.h"
#include "WATCHDOG_HAL.h"





int main(void)
{
	
	
	//make sure watchdog timer is off at start of program
	WDT_off(0);
	timer0_init();
	
	SwitchesInit();
	uart_init(9600, 0);
	
	sei();

	//set up the diode drawing class with both axes of motors and the laser
	StepperMotor TheHorizontalMotor(64, 90, &PORTC, &DDRC, PORTC5, &PORTC, &DDRC, PORTC4, &PORTC, &DDRC, PORTC3, &PORTC, &DDRC, PORTC2);
	StepperMotor TheVerticalMotor(64, 90, &PORTC, &DDRC, PORTC1, &PORTC, &DDRC, PORTC0, &PORTB, &DDRB, PORTB1, &PORTB, &DDRB, PORTB2);
	DiodeLaser theDiodeLaser(&DDRB, &PORTB, PORTB0);
	
	DiodeLaserDrawingClass DiodeDrawing(TheHorizontalMotor, TheVerticalMotor, 28, theDiodeLaser, 200000);
	//wait until switch is pressed before entering the diode drawing loop
	WaitForEitherSwitchPress(0);
	DiodeDrawing.DrawingLoop();
}

