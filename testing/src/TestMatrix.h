/*
 * TestMatrix.h
 *
 *  Created on: Jan 21, 2016
 *      Author: andrew
 */

#ifndef SRC_TESTMATRIX_H_
#define SRC_TESTMATRIX_H_

#include "cute.h"

struct TestMatrix {
	void TestMatrixSetGet();
	void TestMatrixFlash();
	void TestMatrixEquality();
	void TestMatrixMultiplication();
	void TestMatrixAddition();
	void TestMatrixSubtraction();

	cute::suite MakeSuite();
};

#endif /* SRC_TESTMATRIX_H_ */
