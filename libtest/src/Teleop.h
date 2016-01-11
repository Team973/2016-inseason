#include "lib/JoystickHelper.h"

void Robot::TeleopStart(void) {
    printf("***teleop start\n");
}

void Robot::TeleopStop(void) {
    printf("***teleop stop\n");
}

void Robot::TeleopContinuous(void) {

	/**
	 * cheater forward with right bumper
	 */
	if (m_operatorJoystick->GetRawButton(DualAction::RightBumper)) {
		m_shooter->FeedForward();
	}
	else if (m_operatorJoystick->GetRawButton(DualAction::Back)) {
		m_shooter->FeedReverse();
	}
	else {
		m_shooter->FeedStop();
	}

	/**
	 * flywheel shoot with right trigger
	 * flywheel intake with Y button
	 * default state stop
	 */
	if (m_operatorJoystick->GetRawButton(DualAction::BtnA)) {
		m_shooter->SetFlywheelTeleopShoot();
	}
 	else if (m_operatorJoystick->GetRawButton(DualAction::BtnX)) {
		m_shooter->SetFlywheelStop();
	}



	/*
	m_drive->CheesyDrive(m_driverJoystick->GetRawAxis(DualAction::LeftYAxis),
			-m_driverJoystick->GetRawAxis(DualAction::RightXAxis), false,
			m_driverJoystick->GetRawButton(DualAction::RightBumper));
			*/
	/*
	m_drive->ArcadeDrive(m_driverJoystick->GetRawAxis(DualAction::LeftYAxis),
			-m_driverJoystick->GetRawAxis(DualAction::RightXAxis));
			*/

	printf("gyro reading %lf... raw counts %d\n", m_gyroEncoder->GetDistance(),
			m_gyroEncoder->GetRaw());
}
