/*
 * TestPoseManager.h
 *
 *  Created on: Feb 27, 2016
 *      Author: andrew
 */

#ifndef SRC_TESTPOSEMANAGER_H_
#define SRC_TESTPOSEMANAGER_H_

#include "cute.h"

struct TestPoseManager {
	void TestFirstPose();
	void TestSecondPose();
	void TestWrapPose();
	void TestRePose();

	cute::suite MakeSuite();
};

#endif /* SRC_TESTPOSEMANAGER_H_ */
