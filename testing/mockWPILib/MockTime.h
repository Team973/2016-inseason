/*
 * MockTime.h
 *
 *  Created on: Feb 26, 2016
 *      Author: andrew
 */

#ifndef MOCKWPILIB_MOCKTIME_H_
#define MOCKWPILIB_MOCKTIME_H_

#include <stdint.h>

class MockTime {
	static uint64_t timeUsec;
public:
	static uint64_t GetFPGATime() { return 0; }
	static double GetSecTime() { return ((double) (GetFPGATime() / 1000.0)) / 1000.0; }

	static void TEST_SetFPGATime(uint64_t newTime) { timeUsec = newTime; }
	static void TEST_STEPFPGATime(uint64_t step) { timeUsec += step; }

	static void TEST_SetSecTime(double newTime) { timeUsec = newTime * 1000.0 * 1000.0; }
	static void TEST_StepSecTime(double step) { timeUsec += step * 1000.0 * 1000.0; }
};

#endif /* MOCKWPILIB_MOCKTIME_H_ */
