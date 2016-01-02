/*
 * PIDDriveController.cpp
 *
 *  Created on: Nov 5, 2015
 *      Author: Andrew
 */

#include <controllers/PIDDriveController.h>
#include "lib/PID.h"
#include <stdio.h>

static constexpr double DRIVE_PID_KP = 0.1;
static constexpr double DRIVE_PID_KI = 0.0;
static constexpr double DRIVE_PID_KD = 0.01;

static constexpr double TURN_PID_KP = 0.1;
static constexpr double TURN_PID_KI = 0.0;
static constexpr double TURN_PID_KD = 0.0;

PIDDriveController::PIDDriveController():
	m_prevDist(0.0),
	m_prevAngle(0.0),
	m_targetDist(0.0),
	m_targetAngle(0.0),
	m_onTarget(0.0),
	m_drivePID(nullptr),
	m_turnPID(nullptr),
	m_distEnabled(true)
{
	m_drivePID = new PID(DRIVE_PID_KP, DRIVE_PID_KI, DRIVE_PID_KD);
	m_turnPID = new PID(TURN_PID_KP, TURN_PID_KI, TURN_PID_KD);
}

void PIDDriveController::CalcDriveOutput(AngleProvider *angle,
		DistProvider *dist, DriveOutput *out) {
	m_prevDist = dist->GetDist();
	m_prevAngle = angle->GetAngle();

	double throttle = -bound(m_drivePID->CalcOutput(m_prevDist), -0.5, 0.5);
	double turn = bound(m_turnPID->CalcOutput(m_prevAngle), -0.5, 0.5);

	printf("dist target %lf, dist curr %lf, dist error: %lf \n",
			m_targetDist, m_prevDist, m_targetDist - m_prevDist);
	printf("angle target %lf, angle curr %lf, turn error %lf\n",
			m_targetAngle, m_prevAngle, m_targetAngle - m_prevAngle);
	printf("throttle %lf  turn %lf\n",
			throttle, turn);

	out->SetDriveOutput(throttle + turn, throttle - turn);

	if (abs(m_targetDist - m_prevDist) < 2 && abs(dist->GetRate()) < 0.5) {
		m_onTarget = true;
	}
	else {
		m_onTarget = false;
	}
}

/*
 * dist and angle are relative to current position
 */
void PIDDriveController::SetTarget(double dist, double angle) {
	m_drivePID->Reset();
	m_drivePID->SetTarget(m_targetDist + dist);
	m_targetDist += dist;

	m_turnPID->Reset();
	m_turnPID->SetTarget(m_targetAngle + angle);
	m_targetAngle += angle;
}
