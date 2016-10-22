/*
 * ptracker.h
 *
 *  Created on: Oct 22, 2016
 *      Author: amh
 */

#ifndef PTRACKER_H_
#define PTRACKER_H_

namespace Ptracker{

void initTracker();

void setZero();

int16_t getPos();

extern void (*onDirChangedPtr)(uint8_t);
extern void (*onPositionChangedPtr)(int16_t);
}

#endif /* PTRACKER_H_ */
