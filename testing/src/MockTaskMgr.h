/*
 * MockTaskMgr.h
 *
 *  Created on: Jan 21, 2016
 *      Author: andrew
 */

#ifndef SRC_MOCKTASKMGR_H_
#define SRC_MOCKTASKMGR_H_

#include "lib/CoopTask.h"
#include "lib/TaskMgr.h"

namespace frc973 {
/**
 * MockCoopTask that is super easy to test.
 */
class MockCoopTask : public frc973::CoopTask {
public:
	frc973::RobotMode m_lastMode;
	int m_startCount;
	int m_stopCount;
	int m_prePeriodicCount;
	int m_periodicCount;
	int m_postPeriodicCount;
	MockCoopTask()
			 : m_lastMode(frc973::RobotMode::MODE_DISABLED)
			 , m_startCount(0)
			 , m_stopCount(0)
			 , m_prePeriodicCount(0)
			 , m_periodicCount(0)
			 , m_postPeriodicCount(0) {}

	void TaskStartMode(frc973::RobotMode mode) {
		m_startCount++;
		m_lastMode = mode;
	}
	void TaskStopMode(frc973::RobotMode mode) {
		m_stopCount++;
		m_lastMode = mode;
	}
	void TaskPrePeriodic(frc973::RobotMode mode) {
		m_prePeriodicCount++;
		m_lastMode = mode;
	}
	void TaskPeriodic(frc973::RobotMode mode) {
		m_periodicCount++;
		m_lastMode = mode;
	}
	void TaskPostPeriodic(frc973::RobotMode mode) {
		m_postPeriodicCount++;
		m_lastMode = mode;
	}
	bool MatchesCallProfile(int start, int stop, int pre, int med, int post, frc973::RobotMode m) {
		return m_startCount == start &&
				m_stopCount == stop &&
				m_prePeriodicCount == pre &&
				m_periodicCount == med &&
				m_postPeriodicCount == post &&
				m_lastMode == m;
	}
};

/**
 * Make a child of TaskMgr where all the protected methods are public
 */
class AllPublicTaskMgr : public TaskMgr {
public:
	AllPublicTaskMgr() {}
	using TaskMgr::TaskStartModeAll;
	using TaskMgr::TaskStopModeAll;
	using TaskMgr::TaskPrePeriodicAll;
	using TaskMgr::TaskPeriodicAll;
	using TaskMgr::TaskPostPeriodicAll;
};
}

#endif /* SRC_MOCKTASKMGR_H_ */
