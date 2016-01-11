/*
 * SingleThreadTaskMgr.h
 *
 *  Created on: Sep 7, 2015
 *      Author: Andrew
 */

#ifndef FRCLIB_SINGLETHREADTASKMGR_H_
#define FRCLIB_SINGLETHREADTASKMGR_H_

#include "pthread.h"
#include "wpilib.h"
#include "TaskMgr.h"

#define DEFAULT_FREQUENCY	200.0
#define DEFAULT_PERIOD		(1.0 / (DEFAULT_FREQUENCY))

class SingleThreadTaskMgr: public TaskMgr {
public:
	/**
	 * Initialize a Single Threaded Task Manager.  The SingleThreadTaskMgr
	 * creates its own pthread in which to run all of its registeredCoopTasks.
	 * Registered CoopTasks are guaranteed to run sequentially (i.e., no two
	 * tasks will be run at the same time).  Registered CoopTasks will not
	 * start running until you call Start.
	 *
	 * @param stateProvider resource that can be used to determine robot state
	 * 		(can be drive station)
	 * @param loopPeriod interval (in seconds) at which to schedule each period
	 */
	SingleThreadTaskMgr(RobotStateInterface &stateProvider,
			double loopPeriod = DEFAULT_PERIOD);
	virtual ~SingleThreadTaskMgr();

	/**
	 * Start running registered CoopTasks until Stop is called (non-blocking)
	 */
	void Start(void);

	/**
	 * Stop running registered CoopTasks until Start is called (non-blocking)
	 */
	void Stop(void);

	/**
	 * Get the period of time (in seconds) between each call of registered
	 * tasks.
	 *
	 * @return number of seconds alloted to each period
	 */
	double GetLoopPeriodSec();

	/**
	 * Get the period of time (in milliseconds) between each call of registered
	 * tasks.
	 *
	 * @return number of seconds alloted to each period
	 */
	uint32_t GetLoopPeriodMs() {
		return (uint32_t) (this->GetLoopPeriodSec() * Constants::MSEC_PER_SEC);
	}


	/**
	 * Set the period of time (in seconds) to allocate for each loop of
	 * registered tasks.
	 *
	 * @param period (in seconds) to set aside for each loop
	 */
	void SetLoopPeriod(double periodSec);

	/**
	 * Set the frequency at which each task is run (equivalent to calling
	 * SetLoopPeriod(1.0 / frequency).
	 *
	 * @param frequency number of times per second to run each task
	 */
	void SetLoopFrequency(double frequency) {
		this->SetLoopPeriod(1.0 / frequency);
	}

	/**
	 * Get the frequency (in hertz) at which tasks are to be called
	 *
	 * @return frequency of task loop
	 */
	double GetLoopFrequency() {
		return 1.0 / this->GetLoopPeriodSec();
	}

	/**
	 * Check whether the Task Manager is running
	 */
	bool IsRunning();
private:
	static void *RunTasks(void*);

	pthread_mutex_t	m_mutex;
	double m_loopPeriodSec;
	bool m_actuallyRunning;
	bool m_shouldBeRunning;
	RobotStateInterface &m_stateProvider;
};

#endif /* FRCLIB_SINGLETHREADTASKMGR_H_ */
