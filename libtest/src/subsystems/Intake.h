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
	enum IntakeMode {
		off,
		intake,
		reverse
	};

	Intake(TaskMgr *scheduler);
	virtual ~Intake();

	void ExtendIntake();
	void RetractIntake();

	void SetTriggerSpeed(double pow);

	void SetIntakeMode(double pow);

	void TaskPeriodic(RobotMode mode);
private:
	VictorSP *m_intakeMotor;
	VictorSP *m_triggerMotor;
	Solenoid *m_armExtendSol;

	bool m_solenoidExtended;
	IntakeMode m_intakeMode;

	TaskMgr *m_scheduler;
	double m_pow;
	double m_trigPow;
};

#endif /* SRC_SUBSYSTEMS_INTAKE_H_ */
