/*
 * core.cpp
 *
 *  Created on: Oct 22, 2016
 *      Author: amh
 */

#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#include "../inc/core.h"
#include "../inc/usart0.h"
#include "../inc/crc16.h"
#include "../inc/ptracker.h"
#include "../inc/timer1.h"

#define LASER_ON 1
#define LASER_OFF 0
#define BUFF_SIZE 2000
#define MAX_OFFSET 4096
#define MAX_EXP_TIME 20000	// 10ms per pixel

namespace Core {

uint8_t defaultLaserState = LASER_OFF;

uint8_t buffer[2][BUFF_SIZE];
uint16_t dataSize;
uint8_t *rxBuff;
uint8_t *wBuff;
uint8_t isEnabled;

const char *nak = "NAK";
const char *ack = "ACK";

struct Settings settings;

void initCore() {
	rxBuff = buffer[0];
	wBuff = buffer[1];
	memset(wBuff, 0, BUFF_SIZE);
}

void loopCore() {
	if (Usart0::available()) {
		uint8_t inByte = Usart0::peek();
		switch(inByte) {
		case 'L':
			if (Usart0::readBytes(rxBuff, dataSize) == dataSize &&
					rxBuff[1] == 'N' &&
					checkCrc16(rxBuff, dataSize)) {
				swapBuffers();
				sendACK();
			}
			else {
				sendNAK();
			}
			break;
		case 'C':
			if (Usart0::readBytes((uint8_t*)&settings, sizeof(settings)) == sizeof(settings) &&
					settings.header[1] == 'F' &&
					checkCrc16((uint8_t*)&settings, sizeof(settings)) &&
					verifySettings()) {
				Ptracker::setZero();
				isEnabled = 1;
				Timer1::setPulseDuration(settings.expTime);
				dataSize = ((settings.lnLength + 7) >> 3) + 4;
				sendACK();
			}
			else {
				sendNAK();
			}
			break;
		default:
			Usart0::read();
			break;
		}
	}
}

inline void sendACK() {
	Usart0::write((uint8_t*)ack, 3);
}

inline void sendNAK() {
	Usart0::write((uint8_t*)nak, 3);
}

void onDirChanged(uint8_t dir) {
	if (settings.mode) {
		isEnabled = (dir == Ptracker::Forward);
	}
	else {
		isEnabled = 1;
	}
}

void onPositionChanged(int16_t xPos) {
	if (isEnabled && computeLaserState(xPos)) {
		Timer1::pulse();
	}
}

inline uint8_t computeLaserState(int16_t xPos) {
	if ((xPos < settings.offset) || (xPos >= settings.offset + settings.lnLength)) {
		return defaultLaserState;
	}
	return takeBitFromBuffer(xPos - settings.offset);
}


inline uint8_t takeBitFromBuffer(int16_t idx) {
	uint8_t shift = idx & 7; //three LSBs
	uint8_t dbyte = wBuff[2 + (idx >> 3)];	// Header is 2 bytes long!
	return (dbyte >> shift) & 1;
}

inline void swapBuffers() {
	uint8_t *t = rxBuff;
	rxBuff = wBuff;
	wBuff = t;
}

uint8_t verifySettings() {
	if (settings.lnLength > BUFF_SIZE * 8 ||
			settings.lnLength < 0 ||
			settings.offset > MAX_OFFSET ||
			settings.offset < 0 ||
			settings.expTime > MAX_EXP_TIME ||
			settings.expTime < 0) {
		settings.lnLength = 0;
		settings.offset = 0;
		settings.expTime = 0;
		return 0;
	}
	return 1;
}
}
