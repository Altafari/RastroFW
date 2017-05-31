/*
 * main.cpp
 *
 *  Created on: Oct 22, 2016
 *      Author: amh
 */

#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include "../inc/usart0.h"
#include "../inc/timer0.h"
#include "../inc/timer1.h"
#include "../inc/ptracker.h"
#include "../inc/core.h"

char textBuffer[128];

int main() {
    cli();
    Usart0::initUsart();
    Timer0::initTimer();
    Ptracker::initTracker();
    Timer1::initTimer();
    Core::initCore();
    sei();
    int16_t prevPos = -1;
    while (1) {
        //Core::loopCore();
        int16_t newPos = Ptracker::getPos();
       // if (prevPos != newPos) {
            sprintf(textBuffer, "P=%d\n\r", newPos);
            Usart0::write((uint8_t*) textBuffer, (uint8_t) strlen(textBuffer));
            prevPos = newPos;
       // }
        while(Timer0::counter != 0);
    }
}

