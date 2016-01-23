/*
 * ArcadeDrive.cpp
 *
 *  Created on: Oct 30, 2015
 *      Author: Andrew
 */

#include "controllers/ArcadeDriveController.h"
#include "lib/util/Util.h"
#include <stdio.h>

ArcadeDriveController::ArcadeDriveController():
	m_leftOutput(0.0),
	m_rightOutput(0.0)
{

}

ArcadeDriveController::~ArcadeDriveController() {

}

void ArcadeDriveController::CalcDriveOutput(AngleProvider *angle, DistProvider *dist,
		DriveOutput *out) {
	out->SetDriveOutput(m_leftOutput, m_rightOutput);
}

void ArcadeDriveController::SetJoysticks(double throttle, double turn) {
	throttle = Util::bound(throttle, -1.0, 1.0);
	turn = Util::bound(turn, -1.0, 1.0);

	/*
	if (throttle < 0.0) {
		if (turn > 0.0) {
			m_leftOutput = (-throttle - turn);
			m_rightOutput = max(-throttle, turn);
		}
		else {
			m_leftOutput = max(-throttle, -turn);
			m_rightOutput = -throttle + turn;
		}
	}
	else {
		if (turn > 0.0) {
			m_leftOutput = -max(throttle, turn);
			m_rightOutput = -throttle + turn;
		}
		else {
			m_leftOutput = -throttle - turn;
			m_rightOutput = -max(throttle, -turn);
		}
	}
	*/
	m_leftOutput = throttle + turn;
	m_rightOutput = throttle - turn;
	printf("left %lf  right %lf\n", m_leftOutput, m_rightOutput);
}
