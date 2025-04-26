/*
 * DiodeDrawing.h
 *
 * Created: 2025-04-14 4:02:32 PM
 *  Author: mattm
 */ 


#ifndef DIODEDRAWING_H_
#define DIODEDRAWING_H_

#include "Stepper.h"

struct DiodeLaser{
	public:
	DiodeLaser(volatile uint8_t* inputOutputPort, volatile uint8_t* port, uint8_t pin);
	DiodeLaser();
	volatile uint8_t* InputOutputPort;
	volatile uint8_t* Port;
	uint8_t Pin;
	};


class DiodeLaserDrawingClass{
	public:
		DiodeLaserDrawingClass(StepperMotor horizontalMotor, StepperMotor herticalMotor, uint8_t stepsPerPixel, DiodeLaser theDiode, uint32_t drawingTime);
		void DrawHorizontalLine(uint32_t width, uint32_t* points);
		void Draw2DImage(uint32_t width, uint32_t height, uint32_t* points);
		void DrawingLoop();
		~DiodeLaserDrawingClass();
		void TestExposureTimes(uint8_t* exposureTimes, uint8_t numberOfDifferentTimes);
	private:
		StepperMotor HorizontalMotor;
		StepperMotor VerticalMotor;
		DiodeLaser TheDiode;
		uint8_t StepsPerPixel;
		uint32_t DrawingTime;
	};






#endif /* DIODEDRAWING_H_ */