/*
 * timer0.cpp
 *
 *  Created on: Oct 22, 2016
 *      Author: amh
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "../inc/usart0.h"
#include "../inc/timer0.h"

#define TIMER0_TOP 249

namespace Timer0 {

volatile uint8_t counter;

void initTimer() {
    TCCR0A = (1 << WGM01);	// Mode2: CTC
    TCCR0B = (1 << CS01) | (1 << CS00);	// CLKio/64
    OCR0A = TIMER0_TOP;
    TCNT0 = 0;
    TIMSK0 = (1 << OCIE0A);
    DDRC |= (1 << PC5);
}

ISR(TIMER0_COMPA_vect) {
    Usart0::timer++;
    counter++;
}
}
