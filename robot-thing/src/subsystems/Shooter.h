/*
 * Shooter.h
 *
 *  Created on: Oct 9, 2015
 *      Author: Andrew
 */

#ifndef SRC_SUBSYSTEMS_SHOOTER_H_
#define SRC_SUBSYSTEMS_SHOOTER_H_

#include "WPILib.h"
#include "lib/CoopTask.h"

namespace frc973 {

class TaskMgr;
class MovingAverageFilter;
class DelaySwitch;
class PID;
class LogSpreadsheet;
class MedianFilter;
class StateSpaceFlywheelController;
class LogCell;
class LogSpreadsheet;

/**
 * Open loop control on flywheel at the moment... will do fine tuning
 * once it's shown that everything else works
 */
class Shooter : public CoopTask
{
public:
	enum ElevatorHeight {
		wayHigh,
		midHigh,
		midLow,
		wayLow
	};

	Shooter(TaskMgr *scheduler, LogSpreadsheet *logger);
	virtual ~Shooter();

	void SetFlywheelSSShoot(double goal);
	void SetFlywheelPower(double pow);
	void SetFlywheelStop();

	void SetConveyerPower(double pow) {
		m_conveyor->Set(pow);
	}

	void TaskPeriodic(RobotMode mode);

	double GetFrontFlywheelRate(void);
	double GetFrontFlywheelFilteredRate(void);

	double GetRearFlywheelRate(void);
	double GetRearFlywheelFilteredRate(void);

	bool FlywheelOnTarget() { return m_flywheelReady; }
	
	void Print() {
		printf("front flywheel dist %d speed %lf\n", m_frontFlywheelEncoder->Get(), GetFrontFlywheelFilteredRate());
		printf("back flywheel dist %d\n", m_backFlywheelEncoder->Get());
	}

	void SetElevatorHeight(ElevatorHeight newHeight);
private:
	enum FlywheelState {
		openLoop,
		ssControl
	};

	VictorSP *m_frontFlywheelMotor;
	VictorSP *m_backFlywheelMotor;
	VictorSP *m_conveyor;

	Counter *m_frontFlywheelEncoder;
	Counter *m_backFlywheelEncoder;

	FlywheelState m_flywheelState;

	double m_flywheelTargetSpeed;
	StateSpaceFlywheelController *m_frontController;
	double m_flywheelSetPower;

	bool m_flywheelReady;

	MovingAverageFilter *frontMeanFilter;
	MedianFilter *frontMedFilter;
	double frontOldSpeed;

	MovingAverageFilter *rearMeanFilter;
	MedianFilter *rearMedFilter;
	double rearOldSpeed;


	ElevatorHeight m_elevatorState;
	Solenoid *m_longSolenoid;
	Solenoid *m_shortSolenoid;

	LogCell *m_frontFlywheelSpeed;
	LogCell *m_frontFlywheelFilteredSpeed;
	LogCell *m_shooterPow;
	LogCell *m_shooterTime;

	TaskMgr *m_scheduler;

	static constexpr double SLOW_FLYWHEEL_SPEED_SCALEDOWN = 0.7;
};

}

#endif /* SRC_SUBSYSTEMS_SHOOTER_H_ */
