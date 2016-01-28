/*
 * DriveBase.cpp
 *
 *  Created on: Oct 29, 2015
 *      Author: Andrew
 */

#include <lib/DriveBase.h>
#include "lib/util/Util.h"

#include "WPILib.h"

DriveBase::DriveBase(TaskMgr *scheduler, AngleProvider *angle, DistProvider *dist,
		DriveOutput *outpt, DriveController *controller):
	m_scheduler(scheduler),
	m_angleProvider(angle),
	m_distProvider(dist),
	m_driveOutput(outpt),
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
				m_distProvider, m_driveOutput);
	}

	m_driveOutput->UpdateDriveOutput();
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
