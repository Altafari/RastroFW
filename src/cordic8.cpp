/*
 * cordic8.cpp
 *
 *  Created on: Apr. 29, 2017
 *      Author: amh
 */

#include "../inc/cordic8.h"

namespace Cordic8 {
	
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
		res.x = ((int16_t)r.cos * a.x + (int16_t)r.sin * a.y) >> 7;
		res.y = (-(int16_t)r.sin * a.x + (int16_t)r.cos * a.y) >> 7;
		return res;
	}

	int8_t computeAngle(int8_t i, int8_t q) {
		int8_t res;
		Vector2 vect;
		int8_t quadrant = (i < 0) + 2 * (q < 0);
		switch(quadrant) {
			case 0:		// First
				vect.x = i;
				vect.y = q;
				res = 0;
			break;
			case 1:		// Second - rotate 90 deg
				vect.x = q;
				vect.y = -i;
				res = 1;
			break;
			case 2:		// Fourth - rotate 270 deg
				vect.x = -q;
				vect.y = i;
				res = 3;
			break;
			case 3:		// Third rotate 180 deg
				vect.x = -i;
				vect.y = -q;
				res = 2;
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
