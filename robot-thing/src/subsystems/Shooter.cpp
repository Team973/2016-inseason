/*
 * Shooter.cpp
 *
 *  Created on: Oct 9, 2015
 *      Author: Andrew
 */

#include "subsystems/Shooter.h"
#include "RobotInfo.h"
#include "lib/util/Util.h"
#include "lib/WrapDash.h"
#include "controllers/ShooterGainsValentines.h"
#include "lib/TaskMgr.h"
#include "lib/filters/MovingAverageFilter.h"
#include "lib/filters/DelaySwitch.h"
#include "lib/filters/PID.h"
#include "lib/logging/LogSpreadsheet.h"
#include "lib/filters/MedianFilter.h"
#include "controllers/StateSpaceFlywheelController.h"

static constexpr double FLYWHEEL_TELEOP_TARGET_SPEED = 6000;

Shooter::Shooter(TaskMgr *scheduler, LogSpreadsheet *logger) :
		m_frontFlywheelMotor(nullptr),
		m_backFlywheelMotorB(nullptr),
		m_conveyor(nullptr),
		m_flywheelRunning(false),
		m_frontFlywheelEncoder(nullptr),
		m_backFlywheelEncoder(nullptr),
		m_flywheelTargetSpeed(0.0),
		m_flywheelSetPower(0.0),
		m_scheduler(scheduler),
		m_flywheelState(FlywheelState::openLoop),
		m_flywheelReady(false),
		m_maxObservedRPM(0.0),
		m_frontFlywheelSpeed(nullptr),
		m_shooterPow(nullptr),
		m_shooterTime(nullptr),
		frontMeanFilter(nullptr),
		frontMedFilter(nullptr),
		frontOldSpeed(0.0),
		rearMeanFilter(nullptr),
		rearMedFilter(nullptr),
		rearOldSpeed(0.0),
		m_frontController(nullptr),
		m_lastTime(),
		m_elevatorState(ElevatorState::wayLow),
		m_upperSolenoid(new Solenoid(SHOOTER_ANGLE_UPPER_SOL)),
		m_lowerSolenoid(new Solenoid(SHOOTER_ANGLE_LOWER_SOL))
{
	m_scheduler->RegisterTask("Shooter", this, TASK_PERIODIC);

	m_frontFlywheelMotor = new VictorSP(FRONT_SHOOTER_PWM);
	m_backFlywheelMotorB = new VictorSP(BACK_SHOOTER_PWM);
	m_conveyor = new VictorSP(SHOOTER_CONVEYER);

	m_frontFlywheelEncoder= new Counter(FLYWHEEL_FRONT_BANNERSENSOR_DIN);
	m_backFlywheelEncoder = new Counter(FLYWHEEL_BACK_BANNERSENSOR_DIN);

	m_shooterPow = new LogCell("shooter power");
	logger->RegisterCell(m_shooterPow);

	m_frontFlywheelSpeed = new LogCell("front shooter speed (RPM)");
	//logger->RegisterCell(m_frontFlywheelSpeed);

	m_frontFlywheelFilteredSpeed = new LogCell("front shooter filtered speed (RPM)");
	logger->RegisterCell(m_frontFlywheelFilteredSpeed);

	m_shooterTime = new LogCell("Shooter Time (ms)");
	//logger->RegisterCell(m_shooterTime);

	frontMeanFilter = new MovingAverageFilter(0.85);
	frontMedFilter = new MedianFilter();
	rearMeanFilter = new MovingAverageFilter(0.95);
	rearMedFilter = new MedianFilter();

	this->m_frontController = new StateSpaceFlywheelController(FlywheelGainsValentines::MakeGains());

	m_lastTime = GetUsecTime();
}

Shooter::~Shooter() {
	m_scheduler->UnregisterTask(this);
}

void Shooter::SetFlywheelPIDShoot() {
	m_flywheelState = FlywheelState::pidControl;
	m_flywheelTargetSpeed = FLYWHEEL_TELEOP_TARGET_SPEED;
	printf("standard shoot\n");
}

void Shooter::SetFlywheelSSShoot(double goal) {
	m_flywheelState = FlywheelState::ssControl;
	m_flywheelTargetSpeed = goal;
	m_frontController->SetVelocityGoal(m_flywheelTargetSpeed * Constants::PI / 30.0);
}

void Shooter::SetFlywheelPower(double pow) {
	m_flywheelState = FlywheelState::openLoop;
	m_flywheelSetPower = pow;

}

void Shooter::SetFlywheelStop() {
	m_flywheelState = FlywheelState::openLoop;
	m_flywheelSetPower = 0.0;
	printf("stop\n");
}

void Shooter::TaskPeriodic(RobotMode mode) {
	double frontMotorOutput;
	double rearMotorOutput;

	switch(m_flywheelState) {
	case FlywheelState::ssControl:
		frontMotorOutput = m_frontController->Update(
				this->GetFrontFlywheelFilteredRate() * Constants::PI / 30.0);

		m_frontFlywheelMotor->Set(frontMotorOutput);
		m_backFlywheelMotorB->Set(frontMotorOutput * 0.666);
		break;
	case FlywheelState::openLoop:
		frontMotorOutput = m_flywheelSetPower;

		m_frontFlywheelMotor->Set(frontMotorOutput);
		m_backFlywheelMotorB->Set(frontMotorOutput);
		break;
	case FlywheelState::pidControl:
		double vvel = this->GetFrontFlywheelFilteredRate();
		double motorpower = control.CalculateOutput(vvel);
		m_frontFlywheelMotor->Set(motorpower);
		break;
	}

	//printf("Flywheel rate: %lf motor out: %lf\n", GetFlywheelRate(), motorOutput);
	DBStringPrintf(DBStringPos::DB_LINE0,
			"ff med rate: %lf", GetFrontFlywheelFilteredRate());
	DBStringPrintf(DBStringPos::DB_LINE1,
			"rf raw rate: %lf", GetRearFlywheelFilteredRate());
	DBStringPrintf(DBStringPos::DB_LINE2,
			"f Flywheel power: %lf", frontMotorOutput);

	SmartDashboard::PutNumber("front flywheel raw", GetFrontFlywheelRate());
	SmartDashboard::PutNumber("rear flywheel raw", GetRearFlywheelRate());
	SmartDashboard::PutNumber("front flywheel med", GetFrontFlywheelFilteredRate());
	SmartDashboard::PutNumber("rear flywheel med", GetRearFlywheelFilteredRate());
	//SmartDashboard::PutNumber("flyfilt", GetFlywheelMedianRate());
	//SmartDashboard::PutNumber("sooterMotorOut", motorOutput);

	m_shooterPow->LogDouble(frontMotorOutput);
	m_frontFlywheelSpeed->LogDouble(GetFrontFlywheelRate());
	m_frontFlywheelFilteredSpeed->LogDouble(GetFrontFlywheelFilteredRate());
	m_shooterTime->LogPrintf("%ld", (long) GetUsecTime());
}
/**

 * GetFlywheelRate takes raw flywheel rate data and filters it becauze it's jittery
 *
 * There are 3 types of filtering happening
 *  * If we read something more than 6000.0, it must be false... cap it at 6000.0
 *  * If we read anything more than 1000 times our previous value, it must be false... ignore it
 *  * If this value looks good, we do a moving average filter on it
 */
double Shooter::GetFrontFlywheelRate(void) {
	return (1.0 / m_frontFlywheelEncoder->GetPeriod()) * 60.0;
}

double Shooter::GetFrontFlywheelFilteredRate(void) {
	double s = GetFrontFlywheelRate();
	if (s > 9000.0) {
		s = frontOldSpeed;
	}
	else if (s == 0.0) {
		s = frontOldSpeed;
	}
	else {
		frontOldSpeed = s;
	}

	return frontMeanFilter->GetValue(frontMedFilter->Update(s));
}

double Shooter::GetRearFlywheelRate(void) {
	return (1.0 / m_backFlywheelEncoder->GetPeriod()) * 60.0;
}

double Shooter::GetRearFlywheelFilteredRate(void) {
	double s = GetRearFlywheelRate();
	if (s > 9000.0) {
		s = rearOldSpeed;
	}
	else {
		rearOldSpeed = s;
	}

	return rearMeanFilter->GetValue(rearMedFilter->Update(s));
}

void Shooter::SetElevatorHeight(ElevatorState newHeight) {
	if (newHeight != m_elevatorState) {
		m_elevatorState = newHeight;

		switch (m_elevatorState) {
		case ElevatorState::wayHigh:
			m_upperSolenoid->Set(true);
			m_lowerSolenoid->Set(true);
			break;
		case ElevatorState::midHigh:
			m_upperSolenoid->Set(true);
			m_lowerSolenoid->Set(false);
			break;
		case ElevatorState::midLow:
			m_upperSolenoid->Set(false);
			m_lowerSolenoid->Set(true);
			break;
		case ElevatorState::wayLow:
			m_upperSolenoid->Set(false);
			m_lowerSolenoid->Set(false);
			break;
		}
	}
}
