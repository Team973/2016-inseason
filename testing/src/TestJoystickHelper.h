/*
 * TestJoystickHelper.h
 *
 *  Created on: Jan 26, 2016
 *      Author: andrew
 */

#ifndef SRC_TESTJOYSTICKHELPER_H_
#define SRC_TESTJOYSTICKHELPER_H_

#include "cute.h"

struct TestJoystickHelper {
	void TestButtonEventCallback();
	void TestVirtualJoystickButtons();
	cute::suite MakeSuite();
};

#endif /* SRC_TESTJOYSTICKHELPER_H_ */
