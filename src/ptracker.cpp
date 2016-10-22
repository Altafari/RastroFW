/*
 * ptracker.cpp
 *
 *  Created on: Oct 22, 2016
 *      Author: amh
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "../inc/ptracker.h"

#define PTRACKER_DIR_PORT PINB
#define PTRACKER_ENA_PORT PINL
#define PTRACKER_DIR_PIN (1<<PB0)
#define PTRACKER_ENA_PIN (1<<PL0)

namespace Ptracker {

void (*onDirChangedPtr)(uint8_t);
void (*onPositionChangedPtr)(int16_t);
static volatile int16_t xPos;
static volatile uint8_t dir;

void initTracker() {
	EICRA = (1<<ISC01) | (1<<ISC00);	// Interrupt on the rising edge
	EICRB = 0;
	EIMSK = (1<<INT0);
}

void setZero() {
	cli();
	xPos = 0;
	sei();
}

int16_t getPos() {
	cli();
	int16_t res = xPos;	// int16_t is not atomic on 8-bit mcu
	sei();
	return res;
}

ISR(INT0_vect) {
	if (PTRACKER_ENA_PORT & PTRACKER_ENA_PIN) {
		if (PTRACKER_DIR_PORT & PTRACKER_DIR_PIN) {
			xPos++;
			if (!dir) {
				onDirChangedPtr(1);
			}
			dir = 1;
		}
		else {
			xPos--;
			if (dir) {
				onDirChangedPtr(0);
			}
			dir = 0;
		}
		onPositionChangedPtr(xPos);
	}
}
}
