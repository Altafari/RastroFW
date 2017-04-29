/*
 * cordic8.cpp
 *
 *  Created on: Apr. 29, 2017
 *      Author: amh
 */

namespace Cordic8 {

	struct Vector2 {
		int8_t x;
		int8_t y;
	};

	struct RotMatrix {
		int8_t cos;
		int8_t sin;
	};

	const RotMatrix rotations[6] = {
		{91, 91},
		{118, 49},
		{126, 25},
		{127, 13},
		{127, 6},
		{127, 3}
	};

	inline Vector2 rotateVector(Vector2 a, RotMatrix r) {
		Vector2 res;
		res.x = ((int16_t)r.cos * a.x - (int16_t)r.sin * a.y) >> 7;
		res.y = ((int16_t)r.sin * a.x + (int16_t)r.cos * a.y) >> 7;
		return res;
	}

	int8_t computeAngle (int8_t i, int8_t q) {
		int8_t res = 0;	// Extract quadrature signs - two MSB
		Vector2 vect;
		if (i < 0) {
			vect.x = -i;
			res |= 2;
		}
		else {
			vect.x = i;
		}
		if (q < 0) {
			vect.y = -q;
			res |= 1;
		}
		else {
			vect.y = q;
		}
		for (int i = 0; i < 6; i++) {
			Vector2 rot = rotateVector(vect, rotations[i]);
			res <<= 1;
			if (rot.y > 0) {
				res |= 1;
				vect = rot;
			}
			else {
				res &= ~1;
			}
		}
		return res;
	}
}
