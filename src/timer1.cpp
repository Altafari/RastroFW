/*
 * timer1.cpp
 *
 *  Created on: Oct 22, 2016
 *      Author: amh
 */

#include <avr/io.h>
#include <avr/interrupt.h>

namespace Timer1 {

void initTimer() {
	TCNT1 = 0;
	OCR1A = -1;
	TCCR1A = (1<<COM1A1) | (1<<WGM11) | (1<<WGM10);
	TCCR1B = (1<<WGM13) | (1<<WGM12); // stopped
	TIMSK1 = (1<<TOIE1);
	DDRB |= (1<<PB5);
}
void setPulseDuration(int16_t duration) {
	OCR1A = duration;
}
void pulse() {
	TCNT1 = 0;
	TCCR1B |= (1<<CS11);
}

ISR(TIMER1_OVF_vect) {
	TCCR1B &= ~(1<<CS11);
}
}
