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

Arm::Arm(TaskMgr *scheduler)
		 : m_scheduler(scheduler)
		 , m_lastTimeSec(GetSecTime())
		 , m_armMotor(new VictorSP(ARM_MOTOR_PWM))
		 , m_armEncoder(new Encoder(ARM_ENCODER_A_DIN, ARM_ENCODER_B_DIN, true))
		 , m_mode(ArmMode::position_control)
		 , m_targetSpeed(0.0)
		 , m_targetPos(0.0)
		 , m_pid(new PID(ARM_PID_KP, ARM_PID_KI, ARM_PID_KD))
		 , m_zeroOffsetDeg(ARM_OFFSET) {
	m_armEncoder->SetDistancePerPulse(ARM_DIST_PER_CLICK);
	m_scheduler->RegisterTask("Arm", this, TASK_PERIODIC);
}

Arm::~Arm() {
	m_scheduler->UnregisterTask(this);
}

void Arm::SetTargetSpeed(double speed) {
	m_targetSpeed = speed * MAX_ARM_SPEED;
	m_mode = ArmMode::velocity_control;
}

void Arm::SetTargetPosition(double pos) {
	m_targetPos = pos;
	m_mode = ArmMode::position_control;
}

double Arm::GetArmAngle() {
	return m_armEncoder->GetDistance() + m_zeroOffsetDeg;
}

double Arm::GetArmVelocity() {
	return m_armEncoder->GetRate();
}

void Arm::TaskPeriodic(RobotMode mode) {
	static double lastTime = 0;
	double currTimeSec = GetSecTime();


	if (m_mode == ArmMode::velocity_control) {
		double timeStepSec = currTimeSec - lastTime;
		double positionStep = m_targetSpeed * timeStepSec;
		m_targetPos += positionStep;
	}

	m_targetPos = Util::bound(m_targetPos, ARM_SOFT_MIN_POS, ARM_SOFT_MAX_POS);

	m_pid->SetTarget(m_targetPos);
	double motorPower = m_pid->CalcOutput(GetArmAngle());

	DBStringPrintf(DBStringPos::DB_LINE6, "arm pos %lf", GetArmAngle());
	DBStringPrintf(DBStringPos::DB_LINE7, "arm setpt %lf", m_targetPos);

	m_armMotor->Set(motorPower);

	lastTime = currTimeSec;
}

void Arm::Zero() {
	m_zeroOffsetDeg = 0.0;
	m_armEncoder->Reset();
}

}
