/*
 * MockVictorSP.h
 *
 *  Created on: Jan 20, 2016
 *      Author: andrew
 */

#ifndef MOCKWPILIB_MOCKVICTORSP_H_
#define MOCKWPILIB_MOCKVICTORSP_H_

#include <MockSpeedController.h>

class VictorSP : public SpeedController {
public:
	VictorSP(int chan);
	virtual ~VictorSP();
};

#endif /* MOCKWPILIB_MOCKVICTORSP_H_ */
