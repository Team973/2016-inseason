/*
 * VisionDriveController.cpp
 *
 *  Created on: Feb 25, 2016
 *      Author: Andrew
 */

#include <controllers/VisionDriveController.h>
#include "lib/filters/PID.h"
#include "lib/SocketClient.h"

namespace frc973 {

static constexpr double TURN_PID_KP = 0.14;
static constexpr double TURN_PID_KI = 0.0;
static constexpr double TURN_PID_KD = 0;

VisionDriveController::VisionDriveController()
		 : m_leftOutput(0.0)
		 , m_rightOutput(0.0)
		 , m_targetAngle(0.0)
		 , m_prevAngle(0.0)
		 , m_anglePid(new PID(TURN_PID_KP, TURN_PID_KI, TURN_PID_KD))
		 , m_onTarget(false)
		 , m_targetFound(false) {
	SocketClient::AddListener("found", this);
	SocketClient::AddListener("xtheta", this);
}

VisionDriveController::~VisionDriveController() {
	;
}


void VisionDriveController::CalcDriveOutput(DriveStateProvider *state,
		DriveControlSignalReceiver *out) {
	m_prevAngle = state->GetAngle();

	if (m_targetFound) {
		double turn = Util::bound(m_anglePid->CalcOutput(m_prevAngle), -0.5, 0.5);

		printf("turnyness %lf\n", turn);
		//out->SetDriveOutput(-turn, turn);
	}
}

void VisionDriveController::OnValueChange(std::string name, std::string newValue) {
	if (!name.compare("found")) {
		m_targetFound = newValue == "true";
	}
	if (!name.compare("xtheta")) {
		double offset = ::atof(newValue.c_str());
		double m_targetAngle = m_prevAngle + offset;
		m_anglePid->SetTarget(m_targetAngle);
	}
}

} /* namespace frc973 */
