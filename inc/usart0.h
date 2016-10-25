/*
 * usart.h
 *
 *  Created on: Oct 22, 2016
 *      Author: amh
 */

#ifndef SRC_USART0_H_
#define SRC_USART0_H_

#include <inttypes.h>

#define USART0_RX_BUFF_SIZE 256
#define USART0_BITMASK 255
#define USART0_RX_TIMEOUT 150

namespace Usart0 {

	extern volatile uint8_t timer;
	void initUsart();
	void disableUsart();
	uint8_t available();
	uint8_t peek();
	uint8_t read();
	uint16_t readBytes(uint8_t *ptr, uint16_t cnt);
	void write(const uint8_t *ptr, uint8_t cnt);
	uint8_t isTxBusy();
}

#endif /* SRC_USART0_H_ */
