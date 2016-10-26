/*
 * main.cpp
 *
 *  Created on: Oct 22, 2016
 *      Author: amh
 */

#include <avr/interrupt.h>
#include "../inc/usart0.h"
#include "../inc/timer0.h"
#include "../inc/timer1.h"
#include "../inc/ptracker.h"
#include "../inc/core.h"

int main() {
	cli();
	Usart0::initUsart();
	Timer0::initTimer();
	Ptracker::initTracker();
	Timer1::initTimer();
	Core::initCore();
	sei();
	while(1) {
		Core::loopCore();
	}
}
