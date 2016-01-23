/*
 * TestTaskMgr.h
 *
 *  Created on: Jan 21, 2016
 *      Author: andrew
 */

#ifndef SRC_TESTTASKMGR_H_
#define SRC_TESTTASKMGR_H_

#include "cute.h"

struct TestTaskMgr {
	void TestRegisterUnregister();

	cute::suite MakeSuite();
};

#endif /* SRC_TESTTASKMGR_H_ */
