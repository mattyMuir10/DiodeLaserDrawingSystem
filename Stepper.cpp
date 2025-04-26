/*
 * Stepper.cpp
 *
 * Created: 2025-04-13 10:56:45 PM
 *  Author: mattm
 */ 
#include "Stepper.h"
#include "PlantWatering.h"
#include "TIMER_0.h"
#include "PINSETTING.h"

//base constructor
StepperMotor::StepperMotor(){
	
}

//initialize stepper motor values and set registers
StepperMotor::StepperMotor(uint8_t stepsPerRevolution, uint8_t pulseFrequency, volatile uint8_t* INONEPORT, volatile uint8_t* INONEINPUTOUTPUTPORT, uint8_t INONEPIN, volatile uint8_t* INTWOPORT, volatile uint8_t* INTWOINPUTOUTPUTPORT,
uint8_t INTWOPIN, volatile uint8_t* INTHREEPORT, volatile uint8_t* INTHREEINPUTOUTPUTPORT, uint8_t INTHREEPIN, volatile uint8_t* INFOURPORT, volatile uint8_t* INFOURINPUTOUTPUTPORT, uint8_t INFOURPIN) :
stepsPerRevolution(stepsPerRevolution), pulseFrequency(pulseFrequency), INONEPORT(INONEPORT), INONEINPUTOUTPUTPORT(INONEINPUTOUTPUTPORT),  INONEPIN(INONEPIN), INTWOINPUTOUTPUTPORT(INTWOINPUTOUTPUTPORT), INTWOPORT(INTWOPORT), INTWOPIN(INTWOPIN),
INTHREEINPUTOUTPUTPORT(INTHREEINPUTOUTPUTPORT), INTHREEPORT(INTHREEPORT), INTHREEPIN(INTHREEPIN), INFOURINPUTOUTPUTPORT(INFOURINPUTOUTPUTPORT), INFOURPORT(INFOURPORT), INFOURPIN(INFOURPIN){
	*INONEINPUTOUTPUTPORT |= (1 << INONEPIN);
	*INTWOINPUTOUTPUTPORT |= (1 << INTWOPIN);
	*INTHREEINPUTOUTPUTPORT |= (1 << INTHREEPIN);
	*INFOURINPUTOUTPUTPORT |= (1 << INFOURPIN);
}


//do a certain number of steps for stepper motor
void StepperMotor::DoSteps(uint32_t steps, uint8_t clockwise){
	if (clockwise){
		for (int i = 0; i < steps; i++){
			SetPinOnPort(INONEPORT, INONEPIN, 1);
			SetPinOnPort(INTWOPORT, INTWOPIN, 0);
			SetPinOnPort(INTHREEPORT, INTHREEPIN, 0);
			SetPinOnPort(INFOURPORT, INFOURPIN, 0);
			uint32_t startTime = millis();
			while (!millis_end(startTime, 1000 / pulseFrequency));
			SetPinOnPort(INONEPORT, INONEPIN, 0);
			SetPinOnPort(INTWOPORT, INTWOPIN, 1);
			SetPinOnPort(INTHREEPORT, INTHREEPIN, 0);
			SetPinOnPort(INFOURPORT, INFOURPIN, 0);
			startTime = millis();
			while (!millis_end(startTime, 1000 / pulseFrequency));
			SetPinOnPort(INONEPORT, INONEPIN, 0);
			SetPinOnPort(INTWOPORT, INTWOPIN, 0);
			SetPinOnPort(INTHREEPORT, INTHREEPIN, 1);
			SetPinOnPort(INFOURPORT, INFOURPIN, 0);
			startTime = millis();
			while (!millis_end(startTime,1000 / pulseFrequency));
			SetPinOnPort(INONEPORT, INONEPIN, 0);
			SetPinOnPort(INTWOPORT, INTWOPIN, 0);
			SetPinOnPort(INTHREEPORT, INTHREEPIN, 0);
			SetPinOnPort(INFOURPORT, INFOURPIN, 1);
			startTime = millis();
			while (!millis_end(startTime,1000 / pulseFrequency));
		}
	}
	else{
		for (int i = 0; i < steps; i++){
			SetPinOnPort(INONEPORT, INONEPIN, 0);
			SetPinOnPort(INTWOPORT, INTWOPIN, 0);
			SetPinOnPort(INTHREEPORT, INTHREEPIN, 0);
			SetPinOnPort(INFOURPORT, INFOURPIN, 1);
			uint32_t startTime = millis();
			while (!millis_end(startTime, 1000 / pulseFrequency));
			SetPinOnPort(INONEPORT, INONEPIN, 0);
			SetPinOnPort(INTWOPORT, INTWOPIN, 0);
			SetPinOnPort(INTHREEPORT, INTHREEPIN, 1);
			SetPinOnPort(INFOURPORT, INFOURPIN, 0);
			startTime = millis();
			while (!millis_end(startTime, 1000 / pulseFrequency));
			SetPinOnPort(INONEPORT, INONEPIN, 0);
			SetPinOnPort(INTWOPORT, INTWOPIN, 1);
			SetPinOnPort(INTHREEPORT, INTHREEPIN, 0);
			SetPinOnPort(INFOURPORT, INFOURPIN, 0);
			startTime = millis();
			while (!millis_end(startTime,1000 / pulseFrequency));
			SetPinOnPort(INONEPORT, INONEPIN, 1);
			SetPinOnPort(INTWOPORT, INTWOPIN, 0);
			SetPinOnPort(INTHREEPORT, INTHREEPIN, 0);
			SetPinOnPort(INFOURPORT, INFOURPIN, 0);
			startTime = millis();
			while (!millis_end(startTime,1000 / pulseFrequency));
		}
	}
	
}