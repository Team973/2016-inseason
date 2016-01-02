#include "lib/JoystickHelper.h"

void Robot::TeleopStart(void) {
    printf("***teleop start\n");
}

void Robot::TeleopStop(void) {
    printf("***teleop stop\n");
}

void Robot::TeleopContinuous(void) {
	/**
	 * extend intake on low-left trigger
	 * retract intake on up-left trigger
	 */
	if (m_operatorJoystick->GetRawButton(DualAction::LeftBumper)) {
		m_intake->ExtendIntake();
	}
	else if (m_operatorJoystick->GetRawButton(DualAction::LeftTrigger)) {
		m_intake->RetractIntake();
	}

	/**
	 * Run intake with GetY < -0.5
	 * Run vomit with GetY > 0.5
	 */
	if (m_operatorJoystick->GetY() < -0.5) {
		m_shooter->FeedReverse();
		m_intake->SetIntakeMode(Intake::IntakeMode::intake);
	}
	else if (m_operatorJoystick->GetY() > 0.5) {
		m_shooter->FeedReverse();
		m_intake->SetIntakeMode(Intake::IntakeMode::reverse);
	}
	else {
		m_shooter->FeedStop();
		m_intake->SetIntakeMode(Intake::IntakeMode::off);
	}

	/**
	 * cheater forward with right bumper
	 */
	if (m_operatorJoystick->GetRawButton(DualAction::RightBumper)) {
		m_shooter->FeedForward();
		m_intake->SetIntakeMode(Intake::IntakeMode::reverse);
	}
	else if (m_driverJoystick->GetRawButton(DualAction::RightTrigger)) {
		m_shooter->FeedForward();
		m_intake->SetIntakeMode(Intake::IntakeMode::reverse);
	}
	else if (m_operatorJoystick->GetRawButton(DualAction::Back)) {
		m_shooter->FeedReverse();
	}

	/**
	 * flywheel shoot with right trigger
	 * flywheel intake with Y button
	 * default state stop
	 */
	if (m_operatorJoystick->GetRawButton(DualAction::BtnA)) {
		m_shooter->SetFlywheelTeleopShoot();
	}
	else if (m_operatorJoystick->GetRawButton(DualAction::BtnY)) {
		m_shooter->SetFlywheelIntake();
	}
 	else if (m_operatorJoystick->GetRawButton(DualAction::BtnX)) {
		m_shooter->SetFlywheelStop();
	}
 	else if (m_operatorJoystick->GetRawButton(DualAction::BtnB)) {
 		m_shooter->SetFlywheelFullPower();
 	}

	for (int i = 1; i < 32 ; i++) {
		if (m_operatorJoystick->GetRawButton(i)) {
			printf("button %d pressed\n", i);
		}
	}

	/*
	m_drive->CheesyDrive(m_driverJoystick->GetRawAxis(DualAction::LeftYAxis),
			-m_driverJoystick->GetRawAxis(DualAction::RightXAxis), false,
			m_driverJoystick->GetRawButton(DualAction::RightBumper));
			*/
	m_drive->ArcadeDrive(m_driverJoystick->GetRawAxis(DualAction::LeftYAxis),
			-m_driverJoystick->GetRawAxis(DualAction::RightXAxis));

	printf("gyro reading %lf... raw counts %d\n", m_gyroEncoder->GetDistance(),
			m_gyroEncoder->GetRaw());
}
