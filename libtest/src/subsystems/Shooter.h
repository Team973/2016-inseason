/*
 * Shooter.h
 *
 *  Created on: Oct 9, 2015
 *      Author: Andrew
 */

#ifndef SRC_SUBSYSTEMS_SHOOTER_H_
#define SRC_SUBSYSTEMS_SHOOTER_H_

#include "WPILib.h"
#include "lib/TaskMgr.h"
#include "lib/CoopTask.h"
#include "lib/filters/MovingAverageFilter.h"
#include "lib/filters/DelaySwitch.h"
#include "lib/filters/PID.h"
#include "lib/filters/RampedOutput.h"
#include "lib/logging/LogSpreadsheet.h"
#include "lib/filters/MedianFilter.h"
#include "controllers/StateSpaceFlywheelController.h"

/**
 * Open loop control on flywheel at the moment... will do fine tuning
 * once it's shown that everything else works
 */
class Shooter : public CoopTask
{
public:
	Shooter(TaskMgr *scheduler, LogSpreadsheet *logger);
	virtual ~Shooter();

	void SetFlywheelPIDShoot();
	void SetFlywheelSSShoot();
	void SetFlywheelPower(double pow);
	void SetFlywheelStop();

	void TaskPeriodic(RobotMode mode);

	double GetFlywheelRate(void);
	double GetFlywheelMedianRate(void);

	bool FlywheelOnTarget() { return m_flywheelReady; }
private:
	enum FlywheelState {
		openLoop,
		pidControl,
		ssControl
	};

	VictorSP *m_flywheelMotorA;
	VictorSP *m_flywheelMotorB;

	bool m_flywheelRunning;
	Counter *m_flywheelEncoder;
	double m_flywheelTargetSpeed;
	double m_flywheelSetPower;

	TaskMgr *m_scheduler;

	PID	*m_pidCtrl;

	MovingAverageFilter m_speedFilter;
	FlywheelState m_flywheelState;
	RampedOutput *m_flywheelRampRate;

	bool m_flywheelReady;

	double m_maxObservedRPM;

	LogCell *m_shooterSpeed;
	LogCell *m_shooterPow;
	LogCell *m_shooterTime;

	Median5Filter *medFilter;
	double oldSpeed;

	StateSpaceFlywheelController *m_controller;
};

#endif /* SRC_SUBSYSTEMS_SHOOTER_H_ */
