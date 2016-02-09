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
	m_upperIntakeMotor(new VictorSP(FRONT_INTAKE_PWM)),
	m_lowerIntakeMotor(new VictorSP(BACK_INTAKE_PWM)),
	m_upperIntakeMode(UpperIntakeMode::off),
	m_lowerIntakeMode(LowerIntakeMode::off),
	m_scheduler(scheduler)
{
	this->m_scheduler->RegisterTask("Intake", this, TASK_PERIODIC);
}

Intake::~Intake() {
	this->m_scheduler->UnregisterTask(this);
}

void Intake::SetUpperIntakeMode(UpperIntakeMode mode) {
	m_upperIntakeMode = mode;
}

void Intake::SetLowerIntakeMode(LowerIntakeMode mode) {
	m_lowerIntakeMode = mode;
}

void Intake::TaskPeriodic(RobotMode mode) {
	switch (m_upperIntakeMode) {
	case UpperIntakeMode::off:
		m_upperIntakeMotor->Set(0.0);
		break;
	case UpperIntakeMode::forward:
		m_upperIntakeMotor->Set(UPPER_INTAKE_FORWARD_SPEED);
		break;
	case UpperIntakeMode::reverse:
		m_upperIntakeMotor->Set(UPPER_INTAKE_REVERSE_SPEED);
		break;
	}

	switch (m_lowerIntakeMode) {
	case LowerIntakeMode::off:
		m_lowerIntakeMotor->Set(0.0);
		break;
	case LowerIntakeMode::forward:
		m_lowerIntakeMotor->Set(LOWER_INTAKE_FORWARD_SPEED);
		break;
	case LowerIntakeMode::reverse:
		m_lowerIntakeMotor->Set(LOWER_INTAKE_REVERSE_SPEED);
		break;
	}
}
