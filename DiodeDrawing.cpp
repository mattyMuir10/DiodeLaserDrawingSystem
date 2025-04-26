/*
 * DiodeDrawing.cpp
 *
 * Created: 2025-04-14 4:02:48 PM
 *  Author: mattm
 */ 
#include "DiodeDrawing.h"
#include "Stepper.h"
#include <math.h>
#include "config.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "TIMER_0.h"
#include "UART_HAL.h"
#include <string.h>
#include "WATCHDOG_HAL.h"
#include <inttypes.h>
#include <stdio.h>

static uint8_t printBuffer[256] = {0};

//base constructor for diode laser
DiodeLaser::DiodeLaser(){
	
}

//set up diode laser struct with constructor
DiodeLaser::DiodeLaser(volatile uint8_t* inputOutputPort, volatile uint8_t* port, uint8_t pin) : InputOutputPort (inputOutputPort), Port(port), Pin(pin){
	
}

//create the diode drawing class with the two motors which will be the two axis of movement
DiodeLaserDrawingClass::DiodeLaserDrawingClass(StepperMotor horizontalMotor, StepperMotor verticalMotor, uint8_t stepsPerPixel, DiodeLaser theDiode, uint32_t drawingTime){
	HorizontalMotor = horizontalMotor;
	VerticalMotor = verticalMotor;
	StepsPerPixel = stepsPerPixel;
	TheDiode = theDiode;
	DrawingTime = drawingTime;
	*TheDiode.InputOutputPort |= (1 << TheDiode.Pin);	
}

//test the laser with different lengths of exposure for the target
void DiodeLaserDrawingClass::TestExposureTimes(uint8_t* exposureTimes, uint8_t numberOfDifferentTimes){
	for (int i = 0;i < numberOfDifferentTimes; i++ ){
		*TheDiode.Port |= (1 << TheDiode.Pin);
		uint32_t startTime = millis();
		//convert from seconds into milliseconds
		uint32_t lengthOfTime = (uint32_t)exposureTimes[i] * (uint32_t)1000;
		
		while (!millis_end(startTime, lengthOfTime));
		*TheDiode.Port &= ~(1 << TheDiode.Pin);
		HorizontalMotor.DoSteps(StepsPerPixel, 1);
	}
}

//draw a line of a certain length with given array (pointer) of uint32s (series of 1s, 0s which is 1 = on, 0 = off)
void DiodeLaserDrawingClass::DrawHorizontalLine(uint32_t width, uint32_t* points){
	
	//number for keeping track of which array index we are at for points and the current bit in the int
	uint8_t pointsArrayIndex = 0;
	uint8_t bitPoint = 0;
	uint32_t toDraw = 0;
	uint32_t currentBit = 0;
	DDRD |= (1 << 7);
	for (uint32_t i = 0; i < width; i++){
		bitPoint = i % 32;
		if (bitPoint == 0 && i != 0){
			pointsArrayIndex++;
		}
		//check if current bit is zero (to draw) and draw if it is
		currentBit = ((uint32_t)1 << bitPoint);
		toDraw = ((points[pointsArrayIndex]) & (currentBit));
		if (toDraw == 0){
			*TheDiode.Port |= (1 << TheDiode.Pin);
		}
		else{
			*TheDiode.Port &= ~(1 << TheDiode.Pin);
		}
		_delay_ms(100);
		//have horizontal stepper motor take a step
		HorizontalMotor.DoSteps(1, 1);
		
	
		
	}
}

//loop diode drawing and wait for input
void DiodeLaserDrawingClass::DrawingLoop(){
	DDRD |= (1 << 7);
	PORTD |= (1 << 7);
	
	while (1){
		//welcome and instruct user
		memset(printBuffer, 0, sizeof(printBuffer));
		sprintf((char*)printBuffer, "Welcome To The Diode Laser Drawer! Type in 'D' to draw, 'M' to move the laser, or 'T' to test the laser\n");
		uart_send_string(printBuffer);
	//wait for input - 'D' means draw
	while (!uart_read_count());
	uint8_t currentInput = uart_read();
	if (currentInput == 'D'){
		memset(printBuffer, 0, sizeof(printBuffer));
		sprintf((char*)printBuffer, "Type in 8 bits for the width of the drawing\n");
		uart_send_string(printBuffer);
		//wait for next two bytes which are the width and height
		uint8_t width = 0;
		uint8_t height = 0;
		
		for (int i = 7; i >= 0; i--)
		{
			while (!uart_read_count());
			if (uart_read() == '1'){
				width |= (1 << i);
			}
			
		}
		
		memset(printBuffer, 0, sizeof(printBuffer));
		sprintf((char*)printBuffer, "Type in 8 bits for the height of the drawing\n");
		uart_send_string(printBuffer);
		for (int i = 7; i >= 0; i--)
		{
			while (!uart_read_count());
			if (uart_read() == '1'){
				height |= (1 << i);
			}
			
		}
		memset(printBuffer, 0, sizeof(printBuffer));
		sprintf((char*)printBuffer, "Input all bits for image. 0 is a black pixel and 1 is a white (blank) pixel\n");
		uart_send_string(printBuffer);
		
		//rest of bytes are image information (0 = draw, 1 = dont draw)
		uint32_t totalPixels =  (uint32_t)width * (uint32_t)height;
		//division rounds down so add 1 to result
		uint32_t pointsSize = (((uint32_t)width * (uint32_t)height) / (uint32_t)32) + 1;
		
		uint32_t points[pointsSize] = {(uint32_t)0};
			
		uint16_t arrayIndex = 0;
		uint32_t bitIndex = 0;
		
		
		for (uint32_t i = 0; i < totalPixels; i ++){
			arrayIndex = floor((double)i / (double)32);
			bitIndex = (uint32_t)i % 32;
			while(!uart_read_count());
			if (uart_read() == '0'){
				points[arrayIndex] &= ~((uint32_t)1 << bitIndex);
				PORTD &= ~(1 << 7);
			}
			else{
				points[arrayIndex] |= ((uint32_t)1 << bitIndex);
				PORTD |= (1 << 7);
			}
		}
		//draw the described image
		
		Draw2DImage(width, height, points);
		
		//system reset with watchdog timer so does not remain on when finished and unattended
		WDT_off(0);
		WDT_Prescaler_Change(0, WDT_TIMEOUT_1s);
		uint32_t startTime = millis();
		while (!millis_end(startTime, 10000));
		
		
		
			
			
			
		
		
	}
	
	//move one of the motors in a specified way
	else if (currentInput == 'M'){
		PORTD &= ~(1 << 7);
		memset(printBuffer, 0, sizeof(printBuffer));
		sprintf((char*)printBuffer, "choose whether you want to move the y-axis ('V') motor or x-axis ('H').\n");
		uart_send_string(printBuffer);
		while (!uart_read_count());
		currentInput = uart_read();
		//move horizontal motor
		if (currentInput == 'H'){
			
			while (!uart_read_count());
			memset(printBuffer, 0, sizeof(printBuffer));
			sprintf((char*)printBuffer, "Type in '1' to move right or '0' to move left\n");
			uart_send_string(printBuffer);
			//move forwards
			if (uart_read() == '1'){
				uint8_t ended = 0;
				uint32_t stepsToMove = 0;
				//memset(printBuffer, 0, sizeof(printBuffer));
				//sprintf((char*)printBuffer, "Input bits for the number of steps you would like the motor to take\n");
				//uart_send_string(printBuffer);
				while(!ended){
					while (!uart_read_count());
					
					currentInput = uart_read();
					//add bits to steps to move to get total steps input
					if (currentInput == '1' ){
						stepsToMove = stepsToMove << 1;
						stepsToMove += 1;
					}
					else if (currentInput == '0'){
						stepsToMove = stepsToMove << 1;
					}
					else if (currentInput == 'E'){
						ended = 1;
					}
				}
				HorizontalMotor.DoSteps(stepsToMove, 1);
			}
			//move backwards
			else{
				memset(printBuffer, 0, sizeof(printBuffer));
				sprintf((char*)printBuffer, "Input bits for the number of steps you would like the motor to take\n");
				uart_send_string(printBuffer);
					uint8_t ended = 0;
					uint32_t stepsToMove = 0;
					while(!ended){
						while (!uart_read_count());
						
						currentInput = uart_read();
						//add bits to steps to move to get total steps input
						if (currentInput == '1' ){
							stepsToMove = stepsToMove << 1;
							stepsToMove += 1;
						}
						else if (currentInput == '0'){
							stepsToMove = stepsToMove << 1;
						}
						else if (currentInput == 'E'){
							ended = 1;
						}
					}
					HorizontalMotor.DoSteps(stepsToMove, 0);
				
			}
		}
		//move vertical motor
		else if (currentInput == 'V'){
			PORTD |= (1 << 7);
			memset(printBuffer, 0, sizeof(printBuffer));
			sprintf((char*)printBuffer, "Type in '1' to move down or '0' to move up\n");
			uart_send_string(printBuffer);
			while (!uart_read_count());
			memset(printBuffer, 0, sizeof(printBuffer));
			sprintf((char*)printBuffer, "Input bits for the number of steps you would like the motor to take\n");
			uart_send_string(printBuffer);
				//move forwards
				if (uart_read() == '1'){
					PORTD &= ~(1 << 7);
					uint8_t ended = 0;
					uint32_t stepsToMove = 0;
					while(!ended){
						while (!uart_read_count());
						currentInput = uart_read();
						//add bits to steps to move to get total steps input
						if (currentInput == '1' ){
							stepsToMove = stepsToMove << 1;
							stepsToMove += 1;
						}
						else if (currentInput == '0'){
							stepsToMove = stepsToMove << 1;
						}
						else if (currentInput == 'E'){
							ended = 1;
						}
					}
					PORTD |= (1 << 7);
					VerticalMotor.DoSteps(stepsToMove, 1);
				}
				//move backwards
				else{
					//memset(printBuffer, 0, sizeof(printBuffer));
					//sprintf((char*)printBuffer, "Input bits for the number of steps you would like the motor to take. Type 'E' to finish.\n");
					//uart_send_string(printBuffer);
					uint8_t ended = 0;
					uint32_t stepsToMove = 0;
					while(!ended){
						while (!uart_read_count());
						
						currentInput = uart_read();
						//add bits to steps to move to get total steps input
						if (currentInput == '1' ){
							stepsToMove = stepsToMove << 1;
							stepsToMove += 1;
						}
						else if (currentInput == '0'){
							stepsToMove = stepsToMove << 1;
						}
						else if (currentInput == 'E'){
							ended = 1;
						}
					}
					VerticalMotor.DoSteps(stepsToMove, 0);
					
				}
		}
		
	
	}
	//test the used paper with different exposure times to the laser
	else if (currentInput == 'T'){
		memset(printBuffer, 0, sizeof(printBuffer));
		sprintf((char*)printBuffer, "Input 8 bits for number of different speeds you would like to test\n");
		uart_send_string(printBuffer);
		while (!uart_read_count());
		uint8_t numberOfDifferentTimes = 0;
		//get the number of different testing times
		for (int i = 7; i >= 0; i--){
			while (!uart_read_count());
			currentInput = uart_read();
			if (currentInput == '1'){
				numberOfDifferentTimes = numberOfDifferentTimes << 1;
				numberOfDifferentTimes += 1;
			}
			else{
				numberOfDifferentTimes = numberOfDifferentTimes << 1;
			}
		}
		memset(printBuffer, 0, sizeof(printBuffer));
		sprintf((char*)printBuffer, "Type in 8 bits for each test speed\n");
		uart_send_string(printBuffer);
		//get the testing times
		uint8_t testingTimes[numberOfDifferentTimes] = {0};
		for (int i = 0; i < numberOfDifferentTimes; i++){
			for (int j = 7; j >=0; j--){
				while (!uart_read_count());
				currentInput = uart_read();
				if (currentInput == '1'){
					testingTimes[i] = testingTimes[i] << 1;
					testingTimes[i] += 1;
				}
				else{
					testingTimes[i] = testingTimes[i] << 1;
				}
			}
		}
		//test with given parameters
		TestExposureTimes(testingTimes, numberOfDifferentTimes);
		
		
	}
	}
	
	
	
}

//draw a 2d image
void DiodeLaserDrawingClass::Draw2DImage(uint32_t width, uint32_t height, uint32_t* points){
	uint32_t totalPixels = width * height;
	uint16_t arrayIndex = 0;
	uint32_t bitIndex = 0;
	uint32_t toDraw = 0;
	uint32_t currentBitMask = 0;
	
	
	for (uint32_t i = 0; i < totalPixels; i++){
		//to the beginning of line and down one each time get to maximum width
		if ((i % width) == 0 && i !=0){
			//turn off diode when moving across the page
			*TheDiode.Port &= ~(1 << TheDiode.Pin);
			HorizontalMotor.DoSteps(width * StepsPerPixel, 0);
			//vertical motor needs to move up for laser to move "down" which is a "backwards" movement
			VerticalMotor.DoSteps(StepsPerPixel, 0);
			memset(printBuffer, 0, sizeof(printBuffer));
			sprintf((char*)printBuffer, "Moved down at %u\n", i);
			uart_send_string(printBuffer);
		}
		arrayIndex = floor((double)i / (double)32);
		bitIndex = i % 32;
		currentBitMask = ((uint32_t)1 << bitIndex);
		
		//check if current bit is zero (to draw) and draw if it is
		toDraw = ((points[arrayIndex]) & (currentBitMask));
		if (toDraw == 0){
			*TheDiode.Port |= (1 << TheDiode.Pin);
			//wait a while on the spot to draw pixel
			uint32_t startTime = millis();
			while (!millis_end(startTime, DrawingTime));
			*TheDiode.Port &= ~(1 << TheDiode.Pin);
			//let laser cool off
			startTime = millis();
			while (!millis_end(startTime, DrawingTime / 3));
			memset(printBuffer, 0, sizeof(printBuffer));
			sprintf((char*)printBuffer, "Drew a dot %u\n", i);
			uart_send_string(printBuffer);
		}
		else{
			*TheDiode.Port &= ~(1 << TheDiode.Pin);
		}
		//have horizontal stepper motor take a step
		HorizontalMotor.DoSteps(StepsPerPixel, 1);
		
	}
	//turn off diode when finished
	*TheDiode.Port &= ~(1 << TheDiode.Pin);
	
	
	
}

