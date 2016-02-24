/*
 * Intake.h
 *
 *  Created on: Oct 9, 2015
 *      Author: Andrew
 *
 * Intake module handles extending and retracting the the intake arm, and
 * running the roller on the end of that intake arm.
 *
 * Arm extends with the powering of one solenoid.  It retracts with the
 * unpowering of that solenoid because there's surgical tubing pulling
 * it in.
 */

#ifndef SRC_SUBSYSTEMS_INTAKE_H_
#define SRC_SUBSYSTEMS_INTAKE_H_

#include "WPILib.h"
#include "lib/CoopTask.h"

class Intake : public CoopTask {
public:
	enum class LowerIntakeMode {
		off,
		forward,
		reverse
	};

	Intake(TaskMgr *scheduler);
	virtual ~Intake();

	void SetLowerIntakeMode(LowerIntakeMode mode);

	void TaskPeriodic(RobotMode mode);
private:
	VictorSP *m_lowerIntakeMotor;

	LowerIntakeMode m_lowerIntakeMode;

	TaskMgr *m_scheduler;

	static constexpr int UPPER_INTAKE_FORWARD_SPEED = 1.0;
	static constexpr int UPPER_INTAKE_REVERSE_SPEED = -1.0;
	static constexpr int LOWER_INTAKE_FORWARD_SPEED = 1.0;
	static constexpr int LOWER_INTAKE_REVERSE_SPEED = -1.0;
};

#endif /* SRC_SUBSYSTEMS_INTAKE_H_ */
