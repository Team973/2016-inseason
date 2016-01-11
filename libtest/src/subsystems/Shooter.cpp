/*
 * Shooter.cpp
 *
 *  Created on: Oct 9, 2015
 *      Author: Andrew
 */

#include <subsystems/Shooter.h>
#include "RobotInfo.h"

/*
 * Units are motor output units per sec
 * Motor should be able to go from 0.0 to 1.0 in about half a second?
 * 1.0 / 0.5 = 2
 */
static constexpr double FLYWHEEL_RAMP_RATE = 2;
static constexpr double FLYWHEEL_TELEOP_TARGET_SPEED = 3500;

Shooter::Shooter(TaskMgr *scheduler, LogSpreadsheet *logger) :
		m_flywheelMotorA(nullptr),
		m_flywheelMotorB(nullptr),
		m_cheatMotor(nullptr),
		m_readyLightRelay(nullptr),
		m_readyLightDelayFilter(),
		m_flywheelRunning(false),
		m_flywheelEncoder(nullptr),
		m_flywheelTargetSpeed(0.0),
		m_feedState(FeedState::feedStop),
		m_scheduler(scheduler),
		m_pidCtrl(nullptr),
		m_speedFilter(0.9),
		m_flywheelState(FlywheelState::stopping),
		m_flywheelRampRate(nullptr),
		m_flywheelReady(false),
		m_maxObservedRPM(0.0),
		m_shooterSpeed(nullptr)
{
	m_scheduler->RegisterTask("Shooter", this, TASK_PERIODIC);

	m_flywheelMotorA = new VictorSP(FLYWHEEL_ONE_PWM);
	m_flywheelMotorB = new VictorSP(FLYWHEEL_TWO_PWM);
	m_cheatMotor = new VictorSP(CHEATER_PWM);

	m_readyLightRelay = new Relay(READY_LIGHT_RELAY);

	m_flywheelEncoder= new Counter(FLYWHEEL_BANNERSENSOR_DIN);

	m_pidCtrl = new PID(0.0005 / 60.0, 0.0, 0.0, PID_SPEED_CTRL);

	m_flywheelRampRate = new RampedOutput(FLYWHEEL_RAMP_RATE);

	m_shooterSpeed = new LogCell("shooter speed (RPM)");
	logger->RegisterCell(m_shooterSpeed);
}

Shooter::~Shooter() {
	m_scheduler->UnregisterTask(this);
}

void Shooter::SetFlywheelTeleopShoot() {
	m_flywheelState = FlywheelState::closedLoop;
	m_flywheelTargetSpeed = FLYWHEEL_TELEOP_TARGET_SPEED;
	printf("standard shoot\n");
}
/*
void Shooter::SetFlywheelNearShoot() {
	m_flywheelState = FlywheelState::closedLoop;
	m_flywheelTargetSpeed = FLYWHEEL_NEAR_TARGET_SPEED;
	printf("near shoot\n");
}

void Shooter::SetFlywheelFarShoot() {
	m_flywheelState = FlywheelState::closedLoop;
	m_flywheelTargetSpeed = FLYWHEEL_FAR_TARGET_SPEED;
	printf("far shoot\n");
}

void Shooter::SetFlywheelFullPower() {
	m_flywheelState = FlywheelState::fullPower;
	printf("full power\n");
}

void Shooter::SetFlywheelIntake() {
	m_flywheelState = FlywheelState::inbound;
	printf("intake\n");
}
*/
void Shooter::SetFlywheelStop() {
	m_flywheelState = FlywheelState::stopping;
	printf("stop\n");
}

void Shooter::FeedForward() {
	m_feedState = FeedState::feedForward;
}

void Shooter::FeedReverse() {
	m_feedState = FeedState::feedReverse;
}

void Shooter::FeedStop() {
	m_feedState = FeedState::feedStop;
}

void Shooter::TaskPeriodic(RobotMode mode) {
	double motorOutput;

	switch(m_flywheelState) {
	case FlywheelState::closedLoop:
		m_maxObservedRPM = max(m_maxObservedRPM, this->GetFlywheelRate());
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
	case FlywheelState::fullPower:
		motorOutput = m_flywheelRampRate->GetValue(1.0);

		m_flywheelMotorA->Set(motorOutput);
		m_flywheelMotorB->Set(motorOutput);
		break;
	case FlywheelState::stopping:
		motorOutput = m_flywheelRampRate->GetValue(0.0);

		m_flywheelMotorA->Set(motorOutput);
		m_flywheelMotorB->Set(motorOutput);
		break;
	case FlywheelState::inbound:
		motorOutput = m_flywheelRampRate->GetValue(-0.7);

		m_flywheelMotorA->Set(motorOutput);
		m_flywheelMotorB->Set(motorOutput);
		break;
	}

	if (m_readyLightDelayFilter.GetValue(GetFlywheelRate() > m_flywheelTargetSpeed - 100.0)) {
		m_readyLightRelay->Set(Relay::kForward);
		//printf("flywheel up to speed\n");
		m_flywheelReady = true;
	}
	else {
		m_readyLightRelay->Set(Relay::kOff);
		m_flywheelReady = false;
	}

	switch (m_feedState) {
	case FeedState::feedForward:
		m_cheatMotor->Set(FEED_FORWARD_SPEED);
		break;
	case FeedState::feedReverse:
		m_cheatMotor->Set(FEED_REVERSE_SPEED);
		break;
	case FeedState::feedStop:
		m_cheatMotor->Set(0.0);
		break;
	}
	//printf("Flywheel rate: %lf motor out: %lf\n", GetFlywheelRate(), motorOutput);
	SmartDashboard::PutNumber("flywheel", GetFlywheelRate());
	SmartDashboard::PutNumber("sooterMotorOut", motorOutput);
	m_shooterSpeed->LogDouble(GetFlywheelRate());
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
	double currSample = (1.0 / m_flywheelEncoder->GetPeriod()) * 60.0;

	if (currSample > 4000.0) {
		currSample = 4000.0;
	}

	if (m_speedFilter.GetLatestValue() <= 0.0001 ||
			currSample < m_speedFilter.GetLatestValue() * 1000) {
		return m_speedFilter.GetValue(currSample);
	}
	return m_speedFilter.GetLatestValue();
}
