/*
 * quadrature.cpp
 *
 *  Created on: Apr 30, 2017
 *      Author: amh
 */

#include <inttypes.h>
#include <math.h>
#include "../inc/quadrature.h"
#include "../inc/core.h"

namespace Quadrature {

	static uint16_t iMax, iMin, qMax, qMin;
	static float pos, velocity, zeroPos;
	static int16_t iPos;
	static const float dt = 2.0E-5;
	static const int8_t pointsPerCycle = 12;
	static const float alpha = 0.1f * (0.5f / M_PI);
	static const float beta = 0.01f * (0.5f / M_PI) / dt;
	bool isCalibrationActive;

	void init() {
		iMax = 1;
		iMin = 0;
		qMax = 1;
		qMin = 0;
		isCalibrationActive = true;
	}

	void setZero() {
		iPos = 0;
		zeroPos = pos;
	}

	void updateReadings(uint16_t i, uint16_t q) {
		if (isCalibrationActive) {
			iMax = (iMax < i)? i : iMax;
			qMax = (qMax < q)? q : qMax;
			iMin = (iMin > i)? i : iMin;
			qMin = (qMin > q)? q : qMin;
		}

		int16_t iCentered = i - (iMax + iMin) / 2;
		int16_t qCentered = q - (qMax + qMin) / 2;

		float iNorm = iCentered / float(iMax - iMin);
		float qNorm = qCentered / float(qMax - qMin);

		pos += velocity * dt;

		float angle = 2.0f * M_PI * fmod(pos, 1.0f);
		float iRef = cos(angle);
		float qRef = sin(angle);

		float err = (iRef * iNorm + qRef * qNorm);
		pos += err * alpha;
		velocity += err * beta;

		int16_t nPos = int16_t((pos - zeroPos) * pointsPerCycle);

		if (nPos != iPos) {
			iPos = nPos;
			Core::onPositionChanged(iPos);
		}
	}
}
