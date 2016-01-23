/*
 * TestMedianFilter.h
 *
 *  Created on: Jan 21, 2016
 *      Author: andrew
 */

#ifndef SRC_TESTFILTER_H_
#define SRC_TESTFILTER_H_

#include "cute.h"

struct TestFilters {
	void TestMedianFilter();
	void TestMedianOf3Filter();
	void TestMovingAverageFilter();

	cute::suite MakeSuite();
};

#endif /* SRC_TESTFILTER_H_ */
