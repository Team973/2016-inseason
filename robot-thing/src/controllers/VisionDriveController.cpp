/*
 * VisionDriveController.cpp
 *
 *  Created on: Feb 25, 2016
 *      Author: Andrew
 */

#include <controllers/VisionDriveController.h>
#include "lib/filters/PID.h"
#include "lib/SocketServer.hpp"
#include "lib/WrapDash.h"
#include "lib/filters/Debouncer.h"

namespace frc973 {

//static constexpr double TURN_PID_KP = 0.08;
static constexpr double TURN_PID_KP = 0.06;
static constexpr double TURN_PID_KI = 0.0;
static constexpr double TURN_PID_KD = 0.000;

static constexpr double MIN_TURN_POWER = 0.06;

VisionDriveController::VisionDriveController()
		 : m_leftOutput(0.0)
		 , m_rightOutput(0.0)
		 , m_targetAngle(0.0)
		 , m_prevAngle(0.0)
		 , m_prevAngleVel(0.0)
		 , m_anglePid(new PID(TURN_PID_KP, TURN_PID_KI, TURN_PID_KD))
		 , m_onTarget(false)
		 , m_targetFound(false)
		 , m_readyForFrame(true)
		 , m_readyFilter(new Debouncer(0.5))
		 , m_linprof(new RampedOutput(10))
{
	SocketServer::AddListener("found", this);
	SocketServer::AddListener("xtheta", this);
}

VisionDriveController::~VisionDriveController() {
	;
}


void VisionDriveController::CalcDriveOutput(DriveStateProvider *state,
		DriveControlSignalReceiver *out) {
	m_prevAngle = state->GetAngle();
	m_prevAngleVel = state->GetAngularRate();

	double turn = 0.0;
	m_readyForFrame = Util::abs(m_prevAngleVel) < 1.0;

	if (m_targetFound) {
		turn = m_anglePid->CalcOutput(m_prevAngle);
		turn = Util::antideadband(turn, MIN_TURN_POWER);
		turn = Util::bound(turn, -0.5, 0.5);

		printf("turnyness %lf\n", turn);
		out->SetDriveOutput(turn, -turn);

		DBStringPrintf(DBStringPos::DB_LINE4,
				"er %2.2lf tn %2.2lf", m_targetAngle - m_prevAngle, turn);

	}

	DBStringPrintf(DBStringPos::DB_LINE5,
			"err %2.2lf p %1.1lf", m_targetAngle - m_prevAngle,
			turn);
}

static int frames = 9;
void VisionDriveController::OnValueChange(std::string name, std::string newValue) {
	printf("**************OnValueChange observed %s = %s\n", name.c_str(),
			newValue.c_str());

	if (!name.compare("found")) {
		m_targetFound = (newValue == "true");
	}

	if (m_readyFilter->Update(m_readyForFrame)) {
		printf("%s.compate('xtheta') = %d\n", name.c_str(), name.compare("xtheta"));
		if (!name.compare("xtheta")) {
			printf("xtheta handler happened\n");
			double offset = ::atof(newValue.c_str());
			printf("Got our new xtheta value!   It is %lf\n", offset);
			m_targetAngle = m_prevAngle + offset;
			m_anglePid->SetTarget(m_targetAngle);

			DBStringPrintf(DBStringPos::DB_LINE9,
					"a %2.2lf f %d", offset, ++frames);
			m_readyFilter->Update(false);
			m_readyForFrame = false;
		}
	}
}

} /* namespace frc973 */
