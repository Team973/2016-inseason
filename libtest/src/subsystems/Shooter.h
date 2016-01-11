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
#include "lib/MovingAverageFilter.h"
#include "lib/DelaySwitch.h"
#include "lib/PID.h"
#include "lib/RampedOutput.h"
#include "lib/LogSpreadsheet.h"

/**
 * Open loop control on flywheel at the moment... will do fine tuning
 * once it's shown that everything else works
 */
class Shooter : public CoopTask
{
public:
	Shooter(TaskMgr *scheduler, LogSpreadsheet *logger);
	virtual ~Shooter();

	void SetFlywheelTeleopShoot();
	//void SetFlywheelNearShoot();
	//void SetFlywheelFarShoot();
	//void SetFlywheelFullPower();
	//void SetFlywheelIntake();
	void SetFlywheelStop();

	void FeedForward();
	void FeedReverse();
	void FeedStop();

	void TaskPeriodic(RobotMode mode);

	double GetFlywheelRate(void);

	bool FlywheelOnTarget() { return m_flywheelReady; }
private:
	enum FeedState {
		feedForward,
		feedReverse,
		feedStop
	};

	enum FlywheelState {
		closedLoop,
		fullPower,
		inbound,
		stopping
	};

	VictorSP *m_flywheelMotorA;
	VictorSP *m_flywheelMotorB;
	VictorSP *m_cheatMotor;

	Relay	 *m_readyLightRelay;
	DelaySwitch	m_readyLightDelayFilter;

	bool m_flywheelRunning;
	Counter *m_flywheelEncoder;
	double m_flywheelTargetSpeed;
	FeedState m_feedState;

	TaskMgr *m_scheduler;

	PID	*m_pidCtrl;

	MovingAverageFilter m_speedFilter;
	FlywheelState m_flywheelState;
	RampedOutput *m_flywheelRampRate;

	bool m_flywheelReady;

	double m_maxObservedRPM;

	LogCell *m_shooterSpeed;
};

#endif /* SRC_SUBSYSTEMS_SHOOTER_H_ */
