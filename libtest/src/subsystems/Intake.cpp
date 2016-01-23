/*
 * Intake.cpp
 *
 *  Created on: Oct 9, 2015
 *      Author: Andrew
 */

#include <subsystems/Intake.h>
#include "RobotInfo.h"
#include "lib/TaskMgr.h"
#include "lib/WrapDash.h"

Intake::Intake(TaskMgr *scheduler) :
	m_intakeMotor(nullptr),
	m_triggerMotor(nullptr),
	m_armExtendSol(nullptr),
	m_solenoidExtended(false),
	m_intakeMode(IntakeMode::off),
	m_scheduler(scheduler),
	m_pow(0.0),
	m_trigPow(0.0)
{
	this->m_scheduler->RegisterTask("Intake", this, TASK_PERIODIC);

	m_triggerMotor = new VictorSP(TRIGGER_PWM);
	m_intakeMotor = new VictorSP(INTAKE_ROLLER_LEFT_PWM);
	m_armExtendSol = new Solenoid(INTAKE_EXTEND_SOL_CHANNEL);
}

Intake::~Intake() {
	this->m_scheduler->UnregisterTask(this);
}

void Intake::ExtendIntake() {
	if (m_solenoidExtended == false) {
		m_solenoidExtended = true;
		m_armExtendSol->Set(true);
	}
}

/**
 * There's surgical tubing pulling the arm back in so unpowering that solenoid
 * should retract it.
 */
void Intake::RetractIntake() {
	if (m_solenoidExtended == true) {
		m_solenoidExtended = false;
		m_armExtendSol->Set(false);
	}
}

void Intake::SetTriggerSpeed(double triggrPow) {
	m_trigPow = triggrPow;
}

void Intake::SetIntakeMode(double pow) {
	m_pow = pow;
}

void Intake::TaskPeriodic(RobotMode mode) {
	m_triggerMotor->Set(m_trigPow);
	DBStringPrintf(DBStringPos::DB_LINE5,
			"trigger pow: %lf", m_trigPow);
	m_intakeMotor->Set(m_pow);
}
