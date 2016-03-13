/*
 * Arm.cpp
 *
 *  Created on: Jan 29, 2016
 *      Author: Andrew
 */

#include <subsystems/Arm.h>
#include "lib/TaskMgr.h"
#include "lib/filters/PID.h"
#include "RobotInfo.h"
#include "WPILib.h"
#include "lib/WrapDash.h"

namespace frc973 {

Arm::Arm(TaskMgr *scheduler, PowerDistributionPanel* pdp)
		 : m_scheduler(scheduler)
		 , m_pdp(pdp)
		 , m_lastTimeSec(GetSecTime())
		 , m_armMotor(new VictorSP(ARM_MOTOR_PWM))
		 , m_armEncoder(new Encoder(ARM_ENCODER_A_DIN, ARM_ENCODER_B_DIN, true))
		 , m_mode(ArmMode::position_control)
		 , m_targetSpeed(0.0)
		 , m_targetPos(ARM_OFFSET)
		 , m_armPow(0.0)
		 , m_pid(new PID(ARM_PID_KP, ARM_PID_KI, ARM_PID_KD))
		 , m_zeroOffsetDeg(ARM_OFFSET) {
	m_armEncoder->SetDistancePerPulse(ARM_DIST_PER_CLICK);
	m_scheduler->RegisterTask("Arm", this, TASK_PERIODIC);
	m_pid->SetTarget(m_targetPos);
}

Arm::~Arm() {
	m_scheduler->UnregisterTask(this);
}

void Arm::SetTargetSpeed(double speed) {
	m_targetSpeed = speed * MAX_ARM_SPEED;
	m_mode = ArmMode::velocity_control;
}

void Arm::SetTargetPosition(double pos) {
	m_mode = ArmMode::position_control;

	m_targetPos = Util::bound(pos, ARM_SOFT_MIN_POS, ARM_SOFT_MAX_POS);
	m_pid->SetTarget(m_targetPos);
}

void Arm::SetPower(double power) {
	m_armPow = power;
	m_mode = ArmMode::openLoop_control;
}

double Arm::GetArmAngle() {
	return m_armEncoder->GetDistance() + m_zeroOffsetDeg;
}

double Arm::GetArmVelocity() {
	return m_armEncoder->GetRate();
}

double Arm::GetArmCurrent() {
	return m_pdp->GetCurrent(ARM_PDB);
}

/**
 * positionStep is only used in velocity_control but because of C++ weirdness
 * it can't be declared inside the case statement.  I declare it outside and
 * only use it in that one case.
 *
 * in velocity_control we do position pid but we move the position setpoint by
 * some increment every iteration.
 */
void Arm::TaskPeriodic(RobotMode mode) {
	static double lastTime = 0;
	double currTimeSec = GetSecTime();
	double timeStepSec = currTimeSec - lastTime;
	double positionStep;

	switch (m_mode) {
	case ArmMode::velocity_control:
		positionStep = m_targetSpeed * timeStepSec;
		m_targetPos = Util::bound(m_targetPos + positionStep, ARM_SOFT_MIN_POS, ARM_SOFT_MAX_POS);

		m_pid->SetTarget(m_targetPos);
		//m_armMotor->Set(m_pid->CalcOutput(GetArmAngle()));
		break;

	case ArmMode::position_control:
		//m_armMotor->Set(m_pid->CalcOutput(GetArmAngle()));
		break;

	case ArmMode::openLoop_control:
		m_armMotor->Set(m_armPow);
		break;

	case ArmMode::zeroing:
		if (GetArmCurrent() < STALL_CURRENT) {
			m_armMotor->Set(ZEROING_POWER);
		}
		else {
			m_zeroOffsetDeg = 0.0;
			m_armMotor->Set(0.0);
			m_armEncoder->Reset();
			m_mode = openLoop_control;
			m_pid->SetTarget(0.0);
			m_targetPos = 0.0;
			m_armPow = 0.0;
		}
		break;
	}

	DBStringPrintf(DBStringPos::DB_LINE7, "arm setpt %lf", m_targetPos);

	lastTime = currTimeSec;
}

void Arm::Zero() {
	m_mode = ArmMode::zeroing;
}

}
