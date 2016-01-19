/*
 * Intake.cpp
 *
 *  Created on: Oct 9, 2015
 *      Author: Andrew
 */

#include <subsystems/Intake.h>
#include "RobotInfo.h"
#include "lib/TaskMgr.h"

Intake::Intake(TaskMgr *scheduler) :
	m_intakeMotor(nullptr),
	m_armExtendSol(nullptr),
	m_solenoidExtended(false),
	m_intakeMode(IntakeMode::off),
	m_scheduler(scheduler)
{
	this->m_scheduler->RegisterTask("Intake", this, TASK_PERIODIC);

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

void Intake::SetIntakeMode(IntakeMode mode) {
	m_intakeMode = mode;
}

void Intake::TaskPeriodic(RobotMode mode) {
	switch (m_intakeMode) {
	case IntakeMode::off:
		m_intakeMotor->Set(0.0);
		break;
	case IntakeMode::intake:
		m_intakeMotor->Set(INTAKE_FORWARD_SPEED);
		break;
	case IntakeMode::reverse:
		m_intakeMotor->Set(INTAKE_REVERSE_SPEED);
		break;
	}
}
