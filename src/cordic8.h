/*
 * cordic8.h
 *
 *  Created on: Apr. 29, 2017
 *      Author: amh
 */

#ifndef CORDIC8_H_
#define CORDIC8_H_
#include <inttypes.h>

namespace Cordic8 {

	struct Vector2 {
		int8_t x;
		int8_t y;
	};

	struct RotMatrix {
		int8_t cos;
		int8_t sin;
	};

	int8_t computeAngle(int8_t i, int8_t q);
}
#endif /* CORDIC8_H_ */