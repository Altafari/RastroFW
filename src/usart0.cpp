/*
 * Usart.cpp
 *
 *  Created on: Oct 22, 2016
 *      Author: amh
 */

#include "../inc/usart0.h"
#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>

namespace Usart0 {

volatile static uint8_t rxIdx;
volatile static uint8_t rxBuff[USART0_RX_BUFF_SIZE];
volatile static const uint8_t *txPtr;
volatile static int8_t txBytesLeft;
volatile static uint8_t timer;
static uint8_t rdIdx;

void initUsart() {
	rxIdx = 0;
	rdIdx = 0;
	txBytesLeft = 0;
	memset((void*)rxBuff, 0, USART0_RX_BUFF_SIZE);
	UCSR0A = (1<<U2X0);	// U2X0 = 1
	UCSR0B = (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0) | (1<<UDRIE0);
	UCSR0C = (1<<UCSZ01) | (1<<UCSZ00);	// 8N1
	UBRR0H = 0;
	UBRR0L = 16;	// 11500kBps@16MHz
	DDRE |= (1<<PE1);
}

uint8_t available() {
	return rdIdx != rxIdx;
}

uint8_t peek() {
	while(rdIdx == rxIdx);
	return rxBuff[rdIdx];
}

uint8_t read() {
	while(rdIdx == rxIdx);
	uint8_t res = rxBuff[rdIdx];
	rdIdx = (rdIdx + 1) & USART0_BITMASK;
	return res;
}

uint8_t readBytes(uint8_t *ptr, uint16_t cnt) {
	uint16_t ctr = 0;
	timer = 0;
	while(ctr < cnt) {
		while(rdIdx != rxIdx) {
			*ptr++ = rxBuff[rdIdx];
			rdIdx = (rdIdx + 1) & USART0_BITMASK;
			ctr++;
			timer = 0;
		}
		if (timer >= USART0_RX_TIMEOUT) {
			break;
		}
	}
	return ctr;
}

void write(const uint8_t *ptr, uint8_t cnt) {
	if (!cnt) {
		return;
	}
	while (txBytesLeft != 0); // Wait while transmission completes
	txPtr = ptr;
	txBytesLeft = cnt;
}

uint8_t isTxBusy() {
	return txBytesLeft != 0;
}

ISR(USART0_RX_vect) {
	rxBuff[rxIdx] = UDR0;
	rxIdx = (rxIdx + 1) & USART0_BITMASK;	// No overrun detection needed yet
}

ISR(USART0_UDRE_vect) {
	if (txBytesLeft > 0) {
		UDR0 = *txPtr++;
		txBytesLeft--;
	}
}

void disableUsart() {
	UCSR0B = (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0) | (1<<UDRIE0);
}

void onTimerTick() {
	timer++;
}
}
