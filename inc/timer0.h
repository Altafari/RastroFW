/*
 * timer0.h
 *
 *  Created on: Oct 22, 2016
 *      Author: amh
 */

#ifndef TIMER0_H_
#define TIMER0_H_

namespace Timer0 {

typedef void (*FVoid)(void);
void initTimer(void);
extern FVoid timerHandlers[5];
}

#endif /* TIMER0_H_ */
