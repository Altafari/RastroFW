/*
 * ptracker.cpp
 *
 *  Created on: Oct 22, 2016
 *      Author: amh
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "../inc/ptracker.h"
#include "../inc/core.h"

namespace Ptracker {

static volatile int16_t xPos;
static volatile bool isCw;

void initTracker() {
	DDRB &= ~((1<<PORTB0) | (1<<PORTB1));
	EICRA = (1<<ISC00);	// Interrupt on the rising and falling edges
	EICRB = 0;
	EIMSK = (1<<INT0) | (1<<INT1);
}

void setZero() {
	cli();
	xPos = 0;
	isCw = false;
	sei();
}

int16_t getPos() {
	cli();
	int16_t res = xPos;	// int16_t is not atomic on 8-bit mcu
	sei();
	return res;
}

bool getDir() {
	return isCw;
}

void onClockWise() {
	xPos++;
	if (!isCw) {
		isCw = true;
		Core::onDirChanged(isCw);
	}
	Core::onPositionChanged(xPos);
}

void onCounterClockWise() {
	xPos--;
	if (isCw) {
		isCw = false;
		Core::onDirChanged(isCw);
	}
	Core::onPositionChanged(xPos);
}

ISR(INT0_vect) {
	switch(PINB & 3) {
	case 0:		// B0 -> 0; B1 == 0 => cw
	case 3:		// B0 -> 1; B1 == 1 => cw
		onClockWise();
		break;
	case 1:		// B0 -> 1; B1 == 0 => ccw
	case 2:		// B0 -> 0; B1 == 1 => ccw
		onCounterClockWise();
	}
}

ISR(INT1_vect) {
	switch(PINB & 3) {
	case 1:		// B0 == 1; B1-> 0 => cw
	case 2:		// B0 == 0; B1-> 1 => cw
		onClockWise();
		break;
	case 0:		// B0 == 0; B1-> 0 => ccw
	case 3:		// B0 == 1; B1-> 1 => ccw
		onCounterClockWise();
	}
}
}
