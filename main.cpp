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
//#include "ADC_Hal.h"
#include "TIMER_0.h"
//#include "TicTacToeGame.h"
//#include "PlantWatering.h"
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
	
	//focus laser
	//DDRB |= (1 << 0);
	//PORTB |= (1 <<0);
	//WaitForEitherSwitchPress(0);
	//DDRB &= ~(1 << 0);
	//PORTB &= ~(1 <<0);
	

	
	
	
	sei();
	StepperMotor TheHorizontalMotor(64, 90, &PORTC, &DDRC, PORTC5, &PORTC, &DDRC, PORTC4, &PORTC, &DDRC, PORTC3, &PORTC, &DDRC, PORTC2);
	StepperMotor TheVerticalMotor(64, 90, &PORTC, &DDRC, PORTC1, &PORTC, &DDRC, PORTC0, &PORTB, &DDRB, PORTB1, &PORTB, &DDRB, PORTB2);
	DiodeLaser theDiodeLaser(&DDRB, &PORTB, PORTB0);
	
	DiodeLaserDrawingClass DiodeDrawing(TheHorizontalMotor, TheVerticalMotor, 28, theDiodeLaser, 200000);
	//adc_init();
	
	//PlantWateringInit();
	
	
	
	
	//DDRD |= 0b11111111;
	//DDRB |= 0b11111111;
	//DDRC |= 0b11111111;
	
	//DDRC &= ~(1 << BLUEWINPIN);
	
	//PORTD = 0b10000010;
	//PORTC = 0b00100000;

	//*BLUEWINDDPORT &= ~(1 << BLUEWINPIN);
	
	//Initialize();
	//SetPinOnPort(BOTTOMLEFTYELLOWPORT, BOTTOMLEFTYELLOWPIN, 1,BOTTOMLEFTYELLOWONSHIFT);
	//SetPinOnPort(BOTTOMMIDDLEYELLOWPORT, BOTTOMMIDDLEYELLOWPIN, 1,BOTTOMMIDDLEYELLOWONSHIFT);
	//SetPinOnPort(BOTTOMRIGHTYELLOWPORT, BOTTOMRIGHTYELLOWPIN, 1,BOTTOMRIGHTYELLOWONSHIFT);
	
	//PORTD = 0b00010001;
	
	//StartTicTacToe();
	/*
	for (int i = 0; i < 8; i++){
		if (i == 1){
			SetPinOnPort(SHIFTREGISTERDATAPORT, i, 1, 1);
		}
		else{
			SetPinOnPort(SHIFTREGISTERDATAPORT, i, 0, 1);
		}
	}
	*/
	
			//*BLUEWINPORT |= (1 << BLUEWINPIN);

	/*
	PORTB |= 0b00000010;
	for (int i = 0; i < 16; i++){
		_delay_ms(5);
		PORTC |= 0b00000001;
		_delay_ms(5);
		PORTC &= 0b11111110;
	}
	
	PORTB &= 0b11111101;
	
	PORTB |= 0b00000100;	
	_delay_ms(5);

	PORTB &= 0b11111011;
	*/
	
	
	
	WaitForEitherSwitchPress(0);
	//uint32_t points[8] = {(uint32_t)0xFFFFFFFF, (uint32_t)0xFFFFFFFF, (uint32_t)0xFFFFFFFF, (uint32_t)0xFFFFFFFF, (uint32_t)0xFFFFFFFF, (uint32_t)0xFFFFFFFF, (uint32_t)0xFFFFFFFF, (uint32_t)0xFFFFFFFF};
	//DiodeDrawing.DrawHorizontalLine(256, points );
	DiodeDrawing.DrawingLoop();
    while (1) 
    {
		
		//TheVerticalMotor.DoSteps(4, 1);
		//uint32_t startTime = millis();
		//while (!millis_end(startTime, 250));
		
		/*
		
		curInt--;
		if (curInt < 1){
			curInt = 3;
		}
		*/
		
		//volts = ADC_VOLTS(converted_value);
		//printf("volts: %.2f\n", volts);
		/*
		if (lightOn)
		{
			PORTD &= 0b11110111;
			lightOn = false;
		}
		else
		{
			PORTD |= 0b00001000;
			lightOn = true;
		}
		delay_ms_custom(1000);
		*/
		//adc conversion
		
		/*
		convertedValue = adc_convert();
		memset(printBuffer, 0, sizeof(printBuffer));
		sprintf((char*)printBuffer, "Value is %u ", convertedValue);
		uart_send_string(printBuffer);
		if(convertedValue > 1000){
			PORTD |= (1 << PIND2);
		}
		else{
			PORTD &= ~(1 << PIND2);
		}
		*/
		//CheckPlants();
		
		
		
		
		//uint32_t startMillis = millis();
		//while (!millis_end(startMillis, 30000));
		
		
		
		
		
		
    }
}

