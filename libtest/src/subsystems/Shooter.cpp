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
#include "controllers/FlywheelGains.h"
#include "lib/TaskMgr.h"
#include "lib/filters/MovingAverageFilter.h"
#include "lib/filters/DelaySwitch.h"
#include "lib/filters/PID.h"
#include "lib/logging/LogSpreadsheet.h"
#include "lib/filters/MedianFilter.h"
#include "controllers/StateSpaceFlywheelController.h"

static constexpr double FLYWHEEL_TELEOP_TARGET_SPEED = 6000;

Shooter::Shooter(TaskMgr *scheduler, LogSpreadsheet *logger) :
		m_flywheelMotorA(nullptr),
		m_flywheelMotorB(nullptr),
		m_flywheelRunning(false),
		m_flywheelEncoder(nullptr),
		m_flywheelTargetSpeed(0.0),
		m_flywheelSetPower(0.0),
		m_scheduler(scheduler),
		m_pidCtrl(nullptr),
		m_flywheelState(FlywheelState::openLoop),
		m_flywheelReady(false),
		m_maxObservedRPM(0.0),
		m_shooterSpeed(nullptr),
		m_shooterPow(nullptr),
		m_shooterTime(nullptr),
		medFilter(nullptr),
		oldSpeed(0.0),
		m_controller(nullptr)
{
	m_scheduler->RegisterTask("Shooter", this, TASK_PERIODIC);

	m_flywheelMotorA = new VictorSP(FLYWHEEL_ONE_PWM);
	m_flywheelMotorB = new VictorSP(FLYWHEEL_TWO_PWM);

	m_flywheelEncoder= new Counter(FLYWHEEL_BANNERSENSOR_DIN);

	m_pidCtrl = new PID(0.0005 / 60.0, 0.0, 0.0, PID_SPEED_CTRL);

	m_shooterPow = new LogCell("shooter power");
	logger->RegisterCell(m_shooterPow);

	m_shooterSpeed = new LogCell("shooter speed (RPM)");
	logger->RegisterCell(m_shooterSpeed);

	m_shooterTime = new LogCell("Shooter Time (ms)");
	logger->RegisterCell(m_shooterTime);

	medFilter = new MedianFilter();

	this->m_controller = new StateSpaceFlywheelController(FlywheelGains::MakeGains());
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
	m_controller->SetVelocityGoal(m_flywheelTargetSpeed * Constants::PI / 30.0);
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
	double motorOutput;

	switch(m_flywheelState) {
	case FlywheelState::pidControl:
		m_maxObservedRPM = Util::max(m_maxObservedRPM, this->GetFlywheelRate());
		if (this->GetFlywheelRate() < m_flywheelTargetSpeed) {
			motorOutput = 1.0;
		}
		else {
			motorOutput = m_flywheelTargetSpeed / (m_maxObservedRPM + 1000.0);
		}
		printf("Max observed RPM %lf... target RPM %lf\n", m_maxObservedRPM, m_flywheelTargetSpeed);
		m_maxObservedRPM -= 5.0;

		m_flywheelMotorA->Set(motorOutput);
		m_flywheelMotorB->Set(motorOutput);
		break;
	case FlywheelState::ssControl:
		motorOutput = m_controller->Update(
				this->GetFlywheelMedianRate() * Constants::PI / 30.0);

		m_flywheelMotorA->Set(motorOutput);
		m_flywheelMotorB->Set(motorOutput);
		break;
	case FlywheelState::openLoop:
		motorOutput = m_flywheelSetPower;

		m_flywheelMotorA->Set(motorOutput);
		m_flywheelMotorB->Set(motorOutput);
		break;
	}

	printf("Flywheel rate: %lf motor out: %lf\n", GetFlywheelRate(), motorOutput);
	DBStringPrintf(DBStringPos::DB_LINE0,
			"fw med rate: %lf", GetFlywheelMedianRate());
	DBStringPrintf(DBStringPos::DB_LINE1,
			"fw raw rate: %lf", GetFlywheelRate());
	DBStringPrintf(DBStringPos::DB_LINE2,
			"Flywheel power: %lf", motorOutput);

	SmartDashboard::PutNumber("flywheel", GetFlywheelRate());
	SmartDashboard::PutNumber("flyfilt", GetFlywheelMedianRate());
	SmartDashboard::PutNumber("sooterMotorOut", motorOutput);

	m_shooterPow->LogDouble(motorOutput);
	m_shooterSpeed->LogDouble(GetFlywheelMedianRate());
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
double Shooter::GetFlywheelRate(void) {
	return (1.0 / m_flywheelEncoder->GetPeriod()) * 60.0;
}

double Shooter::GetFlywheelMedianRate(void) {
	double s = GetFlywheelRate();
	if (s > 9000.0) {
		s = oldSpeed;
	}
	else {
		oldSpeed = s;
	}

	return medFilter->Update(s);
}
