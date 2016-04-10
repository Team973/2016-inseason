/*
 * PixyVisionDriveController.cpp
 *
 *  Created on: Apr 10, 2016
 *      Author: Kyle Andrei
 */
#include "lib/filters/PID.h"
#include <controllers/PixyVisionDriveController.h>
#include "WPILib.h"
#include "lib/WrapDash.h"

namespace frc973 {

static constexpr double TURN_POS_KP = 0.08;
static constexpr double TURN_POS_KI = 0.0;
static constexpr double TURN_POS_KD = 0;

static constexpr double TURN_VEL_KP = 0.12;
static constexpr double TURN_VEL_KI = 0.0;
static constexpr double TURN_VEL_KD = 0.0;

static constexpr double POWER_CAP = 0.35;

static constexpr int PIXY_CAM_ANALOG_PORT = 4;
static constexpr int PIXY_CAM_DIGITAL_PORT = 3;

PixyVisionDriveController::PixyVisionDriveController()
		: m_leftOutput (0.0)
		,m_rightOutput (0.0)
		,m_onTarget (false)
		,m_prevAngleVel (0.0)
		,m_prevAnglePos (0.0)
		,m_targetAngleVel (0.0)
		,m_targetAnglePos (0.0)
		,m_velPid(new PID(TURN_VEL_KP, TURN_VEL_KI, TURN_VEL_KD, PID_SPEED_CTRL))
		,m_posPid(new PID(TURN_POS_KP, TURN_POS_KI, TURN_POS_KD))
		,m_offsetInput(new AnalogInput(PIXY_CAM_ANALOG_PORT))
		,m_targetFoundInput(new DigitalInput(PIXY_CAM_DIGITAL_PORT)){
	printf("Pixy Vision Drive Controller Has Initialized horray!!!\n");
	m_velPid->SetBounds(-MAX_VELOCITY, MAX_VELOCITY);
	m_posPid->SetBounds(-POWER_CAP, POWER_CAP);

	}

PixyVisionDriveController::~PixyVisionDriveController() {
	// TODO Auto-generated destructor stub
}

void PixyVisionDriveController::CalcDriveOutput(DriveStateProvider *state,
		DriveControlSignalReceiver *out) {
	m_prevAnglePos = state->GetAngle();
	m_prevAngleVel = state->GetAngularRate();

	double turn = 0.0;

	if (m_targetFoundInput->Get() == false) {
		printf("Waiting\n");
		DBStringPrintf(DBStringPos::DB_LINE4,
				"v waiting for data\n");
	}
	else {
		printf("Turning\n");

		m_posPid->SetTarget(m_offsetInput->GetValue());
		double velSetpt = m_posPid->CalcOutput(m_prevAnglePos);
		velSetpt = Util::bound(velSetpt, -MAX_VELOCITY, MAX_VELOCITY);

		m_velPid->SetTarget(velSetpt);
		turn = m_velPid->CalcOutput(m_prevAngleVel);
		turn = Util::signedIncrease(turn, velSetpt * 0.0);
		turn = Util::bound(turn, -0.35, 0.35);

		DBStringPrintf(DBStringPos::DB_LINE4,
				"v p %1.2lf a %2.1lf\n", turn, m_targetAnglePos - m_prevAnglePos);
	}

	out->SetDriveOutput(turn, -turn);
}

} /* namespace frc973 */
