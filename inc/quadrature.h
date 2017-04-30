/*
 * quadrature.h
 *
 *  Created on: Apr 30, 2017
 *      Author: amh
 */

#ifndef QUADRATURE_H_
#define QUADRATURE_H_

namespace Quadrature {

	extern bool isCalibrationActive;

	void init();
	void setZero();
	void updateReadings(uint16_t i, uint16_t q);
}

#endif /* QUADRATURE_H_ */
