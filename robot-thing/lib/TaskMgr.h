/*
 * TaskMgr.h
 *
 *  Created on: Sep 5, 2015
 *      Author: Andrew
 *
 * TaskMgr - Task Manager manages a collection of CoopTask objects
 * and calls their periodic functions when appropriate.  Similar to
 * 254's MultiLooper class.
 *
 * Notable subclasses would be CoopMTRobot (which calls the periodic methods
 * once every robot cycle), and ThreadedTaskMgr (which calls the periodic
 * methods once every time period regardless of robot state).
 */

#ifndef FRCLIB_TASKMGR_H_
#define FRCLIB_TASKMGR_H_

#include "stdint.h"
#include "lib/util/Util.h"

#define MAX_NUM_TASKS			32
#define MAX_TASK_NAME_LEN		31

#define TASK_START_MODE			0x00000001
#define TASK_STOP_MODE			0x00000002
#define TASK_PRE_PERIODIC		0x00000004
#define TASK_PERIODIC			0x00000008
#define TASK_POST_PERIODIC		0x00000010

namespace frc973 {

class CoopTask;

class TaskMgr {
protected:
	TaskMgr();
public:
	virtual ~TaskMgr();

	/**
	 * Register CoopTask object.  If the task is already registered, the flags
	 * 		passed here are added to those flags for which the task was already
	 * 		registered.  If the task registry is already full, return  false.
	 *
	 * @param taskName Specifies the name of the task being registered (for
	 * 		debug purposes)
	 * @param task Specifies the task to be registered
	 * @param flags Specifies which callbacks should be called for the given
	 * 		task
	 *
	 * @return Returns true if the task was successfully registered.  False
	 * 		otherwise.
	 */
	bool RegisterTask(const char *taskName, CoopTask *task, uint32_t flags);

	/**
	 * Function to unregister CoopTask
	 *
	 * @param task Specifies the CoopTask to unregister
	 *
	 * @return Retuns true if CoopTask successfully unregistered, false
	 * 		otherwise (for example if the task was not registered).
	 */
	bool UnregisterTask(CoopTask *task);

protected:
	/**
	 * Calls the TaskStartMode method of all CoopTask objects registered with
	 * 		the TASK_START_MODE flag
	 *
	 * @param mode Specifies robot operation mode
	 */
	void TaskStartModeAll(RobotMode mode);

	/**
	 * Calls the TaskStopMode method of all CoopTask objects registered with
	 * 		the TASK_STOP_MODE flag
	 *
	 * @param mode Specifies robot operation mode
	 */
	void TaskStopModeAll(RobotMode mode);

	/**
	 * Calls the TaskPrePreiodic method of all CoopTask objects registered with
	 * 		the TASK_PRE_PERIODIC flag
	 *
	 * @param mode Specifies robot operation mode
	 */
	void TaskPrePeriodicAll(RobotMode mode);

	/**
	 * Calls the TaskPreiodic method of all CoopTask objects registered with
	 * 		the TASK_PERIODIC flag
	 *
	 * @param mode Specifies robot operation mode
	 */
	void TaskPeriodicAll(RobotMode mode);

	/**
	 * Calls the TaskPostPeriodic method of all CoopTask objects registered
	 * 		with the TASK_POST_PERIODIC flag
	 *
	 * @param mode Specifies robot operation mode
	 */
	void TaskPostPeriodicAll(RobotMode mode);

private:
	/**
	 * Returns the index of the given task int he register tasks list
	 *
	 * @param Task to look up
	 *
	 * @return If found, the task index is returned.  Otherwise it returns -1
	 */
	int FindTask(CoopTask *task);

	int			m_numTasks;
	char	    m_taskNames[MAX_NUM_TASKS][MAX_TASK_NAME_LEN + 1];
	CoopTask   *m_tasks[MAX_NUM_TASKS];
	uint32_t	m_taskFlags[MAX_NUM_TASKS];
};

}

#endif /* FRCLIB_TASKMGR_H_ */
