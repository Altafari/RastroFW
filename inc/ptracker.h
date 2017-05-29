/*
 * ptracker.h
 *
 *  Created on: Oct 22, 2016
 *      Author: amh
 */

#ifndef PTRACKER_H_
#define PTRACKER_H_

namespace Ptracker{

enum Direction {
	Forward,
	Backward
};

void initTracker();

void setZero();

int16_t getPos();

bool getDir();

}

#endif /* PTRACKER_H_ */
