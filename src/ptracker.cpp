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

#define UP_COUNTING 0
#define DOWN_COUNTING 3
#define INTERP_MASK 1
#define INTERP_FACTOR 2
#define INTERP_EXPRESSION (xPos * INTERP_FACTOR | (interpState & INTERP_MASK))
#define MAX_INTERP_INTERVAL 30000
#define MIN_INTERP_INTERVAL 64
#define MAX_ACCELERATION_FRACTION 8
#define INITIAL_INTERP_STATE 4

static volatile int16_t xPos;
static volatile bool isCw;
// This is a state machine used for interpolation
static const uint8_t stateTransitions[] = { 1, 1, 2, 2 };
static volatile uint8_t interpState;

void initTracker() {
    DDRD &= ~((1 << PORTD2) | (1 << PORTD3));
    EICRA = (1 << ISC00) | (1 << ISC10); // Interrupt on the rising and falling edges
    EICRB = 0;
    EIMSK |= (1 << INT0) | (1 << INT1);
    interpState = INITIAL_INTERP_STATE;
    OCR3A = MAX_INTERP_INTERVAL;
    TCCR3A = 0;
    TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
    TIMSK3 = (1 << OCIE3A);
    TCCR4A = 0;
    TCCR4B = (1 << CS41) | (1 << CS40);
}

void setZero() {
    cli();
    xPos = 0;
    isCw = false;
    sei();
}

int16_t getPos() {
    cli();
    int16_t res = INTERP_EXPRESSION;
    sei();
    return res;
}

bool getDir() {
    return isCw;
}

inline void updateInterpTimer() {
    uint16_t timeInterval = TCNT4;
    TCNT4 = 0;
    TCNT3 = 0;
    if ((TIFR4 & (1 << TOV4)) != 0) {
        timeInterval = MAX_INTERP_INTERVAL;
        TIFR4 = (1 << TOV4);
    } else {
        if (timeInterval < MIN_INTERP_INTERVAL) {
            timeInterval = MIN_INTERP_INTERVAL;
        }
        if (timeInterval > MAX_INTERP_INTERVAL) {
            timeInterval = MAX_INTERP_INTERVAL;
        }
    }
    TIFR3 = (1 << OCF3A);
    OCR3A = timeInterval / INTERP_FACTOR;
}

inline void onClockWise() {
    xPos++;
    updateInterpTimer();
    interpState = UP_COUNTING;
    if (!isCw) {
        isCw = true;
        Core::onDirChanged(isCw);
    }
    Core::onPositionChanged(INTERP_EXPRESSION);
}

inline void onCounterClockWise() {
    xPos--;
    updateInterpTimer();
    interpState = DOWN_COUNTING;
    if (isCw) {
        isCw = false;
        Core::onDirChanged(isCw);
    }
    Core::onPositionChanged(INTERP_EXPRESSION);
}

ISR(INT0_vect) {
    switch (PIND & 3) {
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
    switch (PIND & 3) {
    case 1:		// B0 == 1; B1-> 0 => cw
    case 2:		// B0 == 0; B1-> 1 => cw
        onClockWise();
        break;
    case 0:		// B0 == 0; B1-> 0 => ccw
    case 3:		// B0 == 1; B1-> 1 => ccw
        onCounterClockWise();
    }
}

ISR(TIMER3_COMPA_vect) {
    if (interpState != stateTransitions[interpState]) {
        Core::onPositionChanged(INTERP_EXPRESSION);
        interpState = stateTransitions[interpState];
    }
}
}
