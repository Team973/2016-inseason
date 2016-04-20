/*
 * Hanger.cpp
 *
 *  Created on: Apr 17, 2016
 *      Author: Andrew
 */

#include <subsystems/Hanger.h>
#include "subsystems/Drive.h"
#include "subsystems/Shooter.h"
#include "lib/WrapDash.h"

#include "WPILib.h"
#include "RobotInfo.h"

namespace frc973 {

static constexpr double DEFAULT_HANG_POWER = 0.4;

Hanger::Hanger(TaskMgr *scheduler, Drive *drive, VictorSP *crankMotor, Shooter *shooter)
		 : CoopTask()
		 , m_scheduler(scheduler)
		 , m_drive(drive)
		 , m_shooter(shooter)
		 , m_ptoRelease(new Solenoid(POWER_TAKEOFF_SOL))
		 , m_crankMotor(crankMotor)
		 , m_leftHookSensor(new DigitalInput(LEFT_HOOK_HALL_DIN))
		 , m_rightHookSensor(new DigitalInput(RIGHT_HOOK_HALL_DIN))
		 , m_hooksReleased(false)
		 , m_state(HangerState::PreHanging) {
	m_scheduler->RegisterTask("Hanger", this, TASK_PERIODIC);
}

Hanger::~Hanger() {
	m_scheduler->UnregisterTask(this);
}

void Hanger::SetAutoHang(bool enabledP) {
	TryReleaseHooks(true);
	TakeConveyorMotor();

	if (enabledP) {
		m_state = HangerState::AutoHanging;
	}
	else {
		m_state = HangerState::PostHanging;
	}
}

void Hanger::SetManualHang(bool enabledP) {
	TryReleaseHooks(true);
	TakeConveyorMotor();

	if (enabledP) {
		m_state = HangerState::ManualHanging;
	}
	else {
		m_state = HangerState::PostHanging;
	}
}

void Hanger::TryReleaseHooks(bool settt) {
	printf("PTO Setting: %d -> %d\n", m_ptoRelease->Get(), settt);
	m_ptoRelease->Set(settt);
}

void Hanger::TakeConveyorMotor() {
	m_shooter->SetConveyorControl(false);
}

void Hanger::TaskPeriodic(RobotMode mode) {
	DBStringPrintf(DBStringPos::DB_LINE6,
			"left %s, right %s\n",
			(!m_leftHookSensor->Get()? "hit" : "mis"),
			(!m_rightHookSensor->Get()? "hit" : "mis"));

	switch (m_state) {
	case HangerState::PreHanging:
		//Don't do anything
		break;
	case HangerState::AutoHanging:
		if (!m_leftHookSensor->Get() || !m_rightHookSensor->Get()) {
			m_crankMotor->Set(0.0);
		}
		else {
			m_crankMotor->Set(DEFAULT_HANG_POWER);
		}
		break;
	case HangerState::ManualHanging:
		m_crankMotor->Set(DEFAULT_HANG_POWER);
		break;
	case HangerState::PostHanging:
		m_crankMotor->Set(0.0);
		break;
	}
}

} /* namespace frc973 */
