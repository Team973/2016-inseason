/*
 * VisionDriveController.cpp
 *
 *  Created on: Feb 25, 2016
 *      Author: Andrew
 */

#include <controllers/VisionDriveController.h>
#include "lib/filters/PID.h"
#include "lib/WrapDash.h"
#include "lib/filters/Debouncer.h"
#include "stdio.h"

namespace frc973 {

//static constexpr double TURN_PID_KP = 0.08;
/*
static constexpr double TURN_PID_KP = 0.11;
static constexpr double TURN_PID_KI = 0.0;
static constexpr double TURN_PID_KD = 0.000;

static constexpr double MIN_TURN_POWER = 0.1;*/

static constexpr double TURN_PID_KP = 0.19;
static constexpr double TURN_PID_KI = 0.002;
static constexpr double TURN_PID_KD = 0.0015;

//static constexpr double MIN_TURN_POWER = 0.08;
static constexpr double MIN_TURN_POWER = 0.16;

VisionDriveController::VisionDriveController()
		 : VisionDataReceiver()
		 ,  m_leftOutput(0.0)
		 , m_rightOutput(0.0)
		 , m_targetAngle(0.0)
		 , m_prevAngle(0.0)
		 , m_prevAngleVel(0.0)
		 , m_anglePid(new PID(TURN_PID_KP, TURN_PID_KI, TURN_PID_KD))
		 , m_state(WAITING)
		 , m_visionTask(new VisionTask()) {
	printf("Vision Drive Controller Has Initialized horray!!!\n");
}

VisionDriveController::~VisionDriveController() {
	;
}

void VisionDriveController::VisionReceiveXAngle(double angle) {
	printf("Received vision angle %lf\n", angle);
	m_state = TURNING;
	m_targetAngle = m_prevAngle + angle;
	m_anglePid->SetTarget(m_targetAngle);
}

void VisionDriveController::CalcDriveOutput(DriveStateProvider *state,
		DriveControlSignalReceiver *out) {
	m_prevAngle = state->GetAngle();
	m_prevAngleVel = state->GetAngularRate();

	double turn = 0.0;

	if (m_state == WAITING) {
		printf("Waiting\n");
		DBStringPrintf(DBStringPos::DB_LINE4,
				"v waiting for data\n");
	}
	else if (m_state == TURNING) {
		printf("Turning\n");
		turn = m_anglePid->CalcOutput(m_prevAngle);
		turn = Util::antideadband(turn, MIN_TURN_POWER);
		turn = Util::bound(turn, -0.35, 0.35);

		//printf("turnyness %lf\n", turn);
		out->SetDriveOutput(turn, -turn);

		DBStringPrintf(DBStringPos::DB_LINE4,
				"v p %1.2lf a %2.1lf\n", turn, m_targetAngle - m_prevAngle);
	}
	else if (m_state == FAILING) {
		printf("Failing\n");
		DBStringPrintf(DBStringPos::DB_LINE4,
				"v failed to grab input\n");
	}
}

} /* namespace frc973 */
