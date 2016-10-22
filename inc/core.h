/*
 * core.h
 *
 *  Created on: Oct 16, 2016
 *      Author: amh
 */

#ifndef CORE_H_
#define CORE_H_

namespace Core {

struct Settings {
	volatile uint8_t header[2];
	volatile int16_t lnLength;
	volatile int16_t offset;
	volatile int16_t expTime;
	volatile uint8_t mode;
	volatile int16_t crc16;
};

void initCore();
void loopCore();
inline void sendACK();
inline void sendNAK();
void onDirChanged(uint8_t dir);
void onPositionChanged(int16_t xPos);
inline uint8_t computeLaserState(int16_t xPos);
inline uint8_t takeBitFromBuffer(int16_t idx);
inline void swapBuffers();
uint8_t verifySettings();

}
#endif /* CORE_H_ */
