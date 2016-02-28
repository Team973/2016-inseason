/*
 * TestTaskMgr.cpp
 *
 *  Created on: Jan 21, 2016
 *      Author: andrew
 */

#include "TestTaskMgr.h"

#include "lib/TaskMgr.h"
#include "lib/CoopTask.h"

#include "MockTaskMgr.h"

using namespace frc973;

void TestTaskMgr::TestRegisterUnregister() {
	AllPublicTaskMgr m = AllPublicTaskMgr();
	MockCoopTask a = MockCoopTask();
	MockCoopTask b = MockCoopTask();
	MockCoopTask c = MockCoopTask();

	ASSERTM("Failed to register task a",
			m.RegisterTask("i don't know", &a, 0));
	ASSERTM("Failed to register task b",
			m.RegisterTask("meh", &b, 0x00000001 | 0x00000008));
	ASSERTM("Failed to register task c",
			m.RegisterTask("these values don't get read anyways...", &c, 0x00000010 | 0x00000008));

	m.TaskStartModeAll(RobotMode::MODE_TELEOP);

	ASSERTM("Task A's StartMode should not have been called",
			a.MatchesCallProfile(0, 0, 0, 0, 0, RobotMode::MODE_DISABLED));
	ASSERTM("Task B's StartMode should have been called",
			b.MatchesCallProfile(1, 0, 0, 0, 0, RobotMode::MODE_TELEOP));
	ASSERTM("TaskC's StartMode should not have been called",
			c.MatchesCallProfile(0, 0, 0, 0, 0, RobotMode::MODE_DISABLED));

	ASSERTM("Failed unregistering task b", m.UnregisterTask(&b));

	m.TaskPeriodicAll(RobotMode::MODE_TEST);

	ASSERTM("Task A's Periodic should not have been called",
			a.MatchesCallProfile(0, 0, 0, 0, 0, RobotMode::MODE_DISABLED));
	ASSERTM("Task B's Periodic should not have been called",
			b.MatchesCallProfile(1, 0, 0, 0, 0, RobotMode::MODE_TELEOP));
	ASSERTM("TaskC's Periodic should have been called",
			c.MatchesCallProfile(0, 0, 0, 1, 0, RobotMode::MODE_TEST));
}

cute::suite TestTaskMgr::MakeSuite() {
	cute::suite s;
	s.push_back(CUTE_SMEMFUN(TestTaskMgr, TestRegisterUnregister));
	return s;
}
