/*
 * DriveBase.cpp
 *
 *  Created on: Oct 29, 2015
 *      Author: Andrew
 */

#include <lib/DriveBase.h>
#include "lib/util/Util.h"

#include "WPILib.h"

DriveBase::DriveBase(TaskMgr *scheduler, VictorSP *leftMotor,
		VictorSP *rightMotor, AngleProvider *angle, DistProvider *dist,
		DriveController *controller):
	m_scheduler(scheduler),
	m_leftPower(0.0),
	m_rightPower(0.0),
	m_leftMotor(leftMotor),
	m_rightMotor(rightMotor),
	m_angleProvider(angle),
	m_distProvider(dist),
	m_controller(controller)
{
	m_scheduler->RegisterTask("DriveBase", this, TASK_POST_PERIODIC);
}

DriveBase::~DriveBase() {
	m_scheduler->UnregisterTask(this);
}


void DriveBase::TaskPostPeriodic(RobotMode mode) {
	if (m_controller != nullptr) {
		m_controller->CalcDriveOutput(m_angleProvider,
				m_distProvider, this);
	}

	m_leftMotor->Set(Util::bound(m_leftPower, -1.0, 1.0));
	m_rightMotor->Set(-Util::bound(m_rightPower, -1.0, 1.0));
}

void DriveBase::SetDriveOutput(double left, double right) {
	m_leftPower = left;
	m_rightPower = right;
}

void DriveBase::SetDriveController(DriveController *controller) {
	m_controller = controller;
}

bool DriveBase::OnTarget() {
	if (m_controller) {
		return m_controller->OnTarget();
	}
	else {
		return false;
	}
}
