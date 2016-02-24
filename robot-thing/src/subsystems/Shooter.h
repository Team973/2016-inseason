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

class TaskMgr;
class MovingAverageFilter;
class DelaySwitch;
class PID;
class LogSpreadsheet;
class MedianFilter;
class StateSpaceFlywheelController;
class LogCell;
class LogSpreadsheet;

class Thepcont{
public:
	Thepcont() {
		accErr = 0;
	};
	~Thepcont() {};

	double CalculateOutput(double vel) {
		double error = 5500 - vel;
		accErr = accErr + error;
		return error * 0.0008 + accErr * 0.0000010;
	}
	double accErr;
};


/**
 * Open loop control on flywheel at the moment... will do fine tuning
 * once it's shown that everything else works
 */
class Shooter : public CoopTask
{
public:
	enum ElevatorState {
		wayHigh,
		midHigh,
		midLow,
		wayLow
	};

	Shooter(TaskMgr *scheduler, LogSpreadsheet *logger);
	virtual ~Shooter();

	void SetFlywheelPIDShoot();
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

	void SetElevatorHeight(ElevatorState newState);
private:
	enum FlywheelState {
		openLoop,
		pidControl,
		ssControl
	};

	VictorSP *m_frontFlywheelMotor;
	VictorSP *m_backFlywheelMotorB;
	VictorSP *m_conveyor;

	bool m_flywheelRunning;
	Counter *m_frontFlywheelEncoder;
	Counter *m_backFlywheelEncoder;
	double m_flywheelTargetSpeed;
	double m_flywheelSetPower;

	TaskMgr *m_scheduler;

	FlywheelState m_flywheelState;

	bool m_flywheelReady;

	double m_maxObservedRPM;

	LogCell *m_frontFlywheelSpeed;
	LogCell *m_frontFlywheelFilteredSpeed;
	LogCell *m_shooterPow;
	LogCell *m_shooterTime;

	MovingAverageFilter *frontMeanFilter;
	MedianFilter *frontMedFilter;
	double frontOldSpeed;

	MovingAverageFilter *rearMeanFilter;
	MedianFilter *rearMedFilter;
	double rearOldSpeed;

	StateSpaceFlywheelController *m_frontController;
	uint64_t m_lastTime;

	Thepcont control = Thepcont();

	ElevatorState m_elevatorState;
	Solenoid *m_upperSolenoid;
	Solenoid *m_lowerSolenoid;

	static constexpr double SLOW_FLYWHEEL_SPEED_SCALEDOWN = 0.7;
};

#endif /* SRC_SUBSYSTEMS_SHOOTER_H_ */
