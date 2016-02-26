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

namespace frc973 {

Shooter::Shooter(TaskMgr *scheduler, LogSpreadsheet *logger) :
		m_frontFlywheelMotor(new VictorSP(FRONT_SHOOTER_PWM)),
		m_backFlywheelMotor(new VictorSP(BACK_SHOOTER_PWM)),
		m_conveyor(new VictorSP(SHOOTER_CONVEYER_MOTOR_PWM)),
		m_frontFlywheelEncoder(new Counter(FLYWHEEL_FRONT_BANNERSENSOR_DIN)),
		m_backFlywheelEncoder(new Counter(FLYWHEEL_BACK_BANNERSENSOR_DIN)),
		m_flywheelState(FlywheelState::openLoop),
		m_flywheelTargetSpeed(0.0),
		m_frontController(new StateSpaceFlywheelController(FlywheelGainsValentines::MakeGains())),
		m_flywheelSetPower(0.0),
		m_flywheelReady(false),
		frontMeanFilter(new MovingAverageFilter(0.85)),
		frontMedFilter(new MedianFilter()),
		frontOldSpeed(0.0),
		rearMeanFilter(new MovingAverageFilter(0.85)),
		rearMedFilter(new MedianFilter()),
		rearOldSpeed(0.0),
		m_elevatorState(ElevatorHeight::wayLow),
		m_longSolenoid(new Solenoid(SHOOTER_ANGLE_UPPER_SOL)),
		m_shortSolenoid(new Solenoid(SHOOTER_ANGLE_LOWER_SOL)),
		m_frontFlywheelSpeed(new LogCell("front shooter speed (RPM)")),
		m_frontFlywheelFilteredSpeed(new LogCell("front shooter filtered speed (RPM)")),
		m_shooterPow(new LogCell("shooter power")),
		m_shooterTime(new LogCell("Shooter Time (ms)")),
		m_scheduler(scheduler)
{
	m_scheduler->RegisterTask("Shooter", this, TASK_PERIODIC);

	logger->RegisterCell(m_shooterPow);
	logger->RegisterCell(m_frontFlywheelSpeed);
	logger->RegisterCell(m_frontFlywheelFilteredSpeed);
	//logger->RegisterCell(m_shooterTime);
}

Shooter::~Shooter() {
	m_scheduler->UnregisterTask(this);
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

	switch(m_flywheelState) {
	case FlywheelState::ssControl:
		frontMotorOutput = m_frontController->Update(
				this->GetFrontFlywheelFilteredRate() * Constants::PI / 30.0);

		m_frontFlywheelMotor->Set(frontMotorOutput);
		m_backFlywheelMotor->Set(frontMotorOutput * 0.666);
		break;
	case FlywheelState::openLoop:
		frontMotorOutput = m_flywheelSetPower;

		m_frontFlywheelMotor->Set(frontMotorOutput);
		m_backFlywheelMotor->Set(frontMotorOutput);
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

	return frontMeanFilter->Update(frontMedFilter->Update(s));
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

	return rearMeanFilter->Update(rearMedFilter->Update(s));
}

void Shooter::SetElevatorHeight(ElevatorHeight newHeight) {
	if (newHeight != m_elevatorState) {
		m_elevatorState = newHeight;

		switch (m_elevatorState) {
		case ElevatorHeight::wayHigh:
			m_longSolenoid->Set(true);
			m_shortSolenoid->Set(true);
			break;
		case ElevatorHeight::midHigh:
			m_longSolenoid->Set(true);
			m_shortSolenoid->Set(false);
			break;
		case ElevatorHeight::midLow:
			m_longSolenoid->Set(false);
			m_shortSolenoid->Set(true);
			break;
		case ElevatorHeight::wayLow:
			m_longSolenoid->Set(false);
			m_shortSolenoid->Set(false);
			break;
		}
	}
}

}
