#include "lib/JoystickHelper.h"
#include "lib/WrapDash.h"

void Robot::TeleopStart(void) {
    printf("***teleop start\n");
}

void Robot::TeleopStop(void) {
    printf("***teleop stop\n");
}

void Robot::TeleopContinuous(void) {
	double armSpeed = m_operatorJoystick->GetRawAxisWithDeadband(DualAction::RightYAxis, 0.2);
	if (armSpeed != 0.0) {
		m_arm->SetTargetSpeed(armSpeed);
	}
	/*
	m_drive->CheesyDrive(m_driverJoystick->GetRawAxis(DualAction::LeftYAxis),
			-m_driverJoystick->GetRawAxis(DualAction::RightXAxis), false,
			m_driverJoystick->GetRawButton(DualAction::RightBumper));
			*/

	m_drive->ArcadeDrive(m_driverJoystick->GetRawAxis(DualAction::LeftYAxis),
			-m_driverJoystick->GetRawAxis(DualAction::RightXAxis));

 	/*
	printf("gyro reading %lf... raw counts %d\n", m_gyroEncoder->GetDistance(),
			m_gyroEncoder->GetRaw());
			*/
}

void Robot::ObserveJoystickStateChange(uint32_t port, uint32_t button,
			bool pressedP) {
	static double goal = 5500.0;

	printf("joystick state change port %d button %d state %d\n", port, button, pressedP);

	if (port == DRIVER_JOYSTICK_PORT) {
		/* Add any bindings for the driver's joystick here */
	}
	else if (port == OPERATOR_JOYSTICK_PORT) {
		switch (button) {
			case DualAction::BtnA:
				if (pressedP) {
					m_intake->SetLowerIntakeMode(Intake::LowerIntakeMode::forward);
				}
				else {
					m_intake->SetLowerIntakeMode(Intake::LowerIntakeMode::off);
				}
				break;
			case DualAction::BtnB:
				if (pressedP) {
					m_intake->SetLowerIntakeMode(Intake::LowerIntakeMode::reverse);
				}
				else {
					m_intake->SetLowerIntakeMode(Intake::LowerIntakeMode::off);
				}
				break;
			case DualAction::BtnX:
				if (pressedP) {
					m_intake->SetUpperIntakeMode(Intake::UpperIntakeMode::forward);
				}
				else {
					m_intake->SetUpperIntakeMode(Intake::UpperIntakeMode::off);
				}
				break;
			case DualAction::BtnY:
				if (pressedP) {
					m_intake->SetUpperIntakeMode(Intake::UpperIntakeMode::reverse);
				}
				else {
					m_intake->SetUpperIntakeMode(Intake::UpperIntakeMode::off);
				}
				break;
			case DualAction::LeftBumper:
				m_arm->SetTargetPosition(Arm::ARM_POS_UP);
				break;
			case DualAction::LeftTrigger:
				m_arm->SetTargetPosition(Arm::ARM_POS_DOWN);
				break;
			case DualAction::RightBumper:
				break;
			case DualAction::RightTrigger:
				break;
			case DualAction::DPadUpVirtBtn:
				if (pressedP) {
					goal += 50.0;
					//m_shooter->SetFlywheelSSShoot(goal);
				}
				break;
			case DualAction::DPadDownVirtBtn:
				if (pressedP) {
					goal -= 50.0;
					//m_shooter->SetFlywheelSSShoot(goal);
				}
				break;
			case DualAction::DPadLeftVirtBtn:
				if (pressedP) {
					//m_shooter->SetFlywheelStop();
				}
				break;
		}
	}

	DBStringPrintf(DBStringPos::DB_LINE4,
			"Flywheel goal: %lf", goal);

}
