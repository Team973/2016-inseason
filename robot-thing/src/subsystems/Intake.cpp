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
	m_lowerIntakeMotor(new VictorSP(FRONT_INTAKE_PWM)),
	m_lowerIntakeMode(LowerIntakeMode::off),
	m_scheduler(scheduler)
{
	this->m_scheduler->RegisterTask("Intake", this, TASK_PERIODIC);
}

Intake::~Intake() {
	this->m_scheduler->UnregisterTask(this);
}

void Intake::SetLowerIntakeMode(LowerIntakeMode mode) {
	m_lowerIntakeMode = mode;
}

void Intake::TaskPeriodic(RobotMode mode) {
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
