/*
 * Stepper.h
 *
 * Created: 2025-04-13 10:56:25 PM
 *  Author: mattm
 */ 


#ifndef STEPPER_H_
#define STEPPER_H_

#include <stdint.h>
class StepperMotor{
	public:
		StepperMotor();
		StepperMotor (uint8_t stepsPerRevolution, uint8_t pulseFrequency, volatile uint8_t* INONEPORT, volatile uint8_t* INONEINPUTOUTPUTPORT,  uint8_t INONEPIN, volatile uint8_t* INTWOPORT, volatile uint8_t* INTWOINPUTOUTPUTPORT,
		uint8_t INTWOPIN, volatile uint8_t* INTHREEPORT, volatile uint8_t* INTHREEINPUTOUTPUTPORT, uint8_t INTHREEPIN, volatile uint8_t* INFOURPORT, volatile uint8_t* INFOURINPUTOUTPUTPORT, uint8_t INFOURPIN);
		void DoSteps(uint32_t StepsToTake, uint8_t forwards);
	private:
		uint8_t stepsPerRevolution;
		uint8_t pulseFrequency;
		volatile uint8_t* INONEPORT;
		volatile uint8_t* INONEINPUTOUTPUTPORT;
		uint8_t INONEPIN;
		volatile uint8_t* INTWOPORT;
		volatile uint8_t* INTWOINPUTOUTPUTPORT;
		uint8_t INTWOPIN;
		volatile uint8_t* INTHREEPORT;
		volatile uint8_t* INTHREEINPUTOUTPUTPORT;
		uint8_t INTHREEPIN;
		volatile uint8_t* INFOURPORT;
		volatile uint8_t* INFOURINPUTOUTPUTPORT;
		uint8_t INFOURPIN;
	
	
		
	
	};
	





#endif /* STEPPER_H_ */