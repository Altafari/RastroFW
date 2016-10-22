/*
 * usart.h
 *
 *  Created on: Oct 22, 2016
 *      Author: amh
 */

#ifndef SRC_USART0_H_
#define SRC_USART0_H_

#include <inttypes.h>

#define USART0_RX_BUFF_SIZE 32
#define USART0_BITMASK 31
#define USART0_RX_TIMEOUT 100

namespace Usart0 {

	void initUsart();
	void disableUsart();
	uint8_t available();
	uint8_t peek();
	uint8_t read();
	uint8_t readBytes(uint8_t *ptr, uint16_t cnt);
	void write(const uint8_t *ptr, uint8_t cnt);
	uint8_t isTxBusy();
	void onTimerTick();
}

#endif /* SRC_USART0_H_ */
