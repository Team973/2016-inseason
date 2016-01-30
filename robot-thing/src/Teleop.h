#include "lib/JoystickHelper.h"
#include "lib/WrapDash.h"

void Robot::TeleopStart(void) {
    printf("***teleop start\n");
}

void Robot::TeleopStop(void) {
    printf("***teleop stop\n");
}

void Robot::TeleopContinuous(void) {
 	m_intake->SetIntakeMode(m_operatorJoystick->GetRawAxis(DualAction::RightYAxis));
 	m_intake->SetTriggerSpeed(m_operatorJoystick->GetRawAxis(DualAction::LeftYAxis));


	/*
	m_drive->CheesyDrive(m_driverJoystick->GetRawAxis(DualAction::LeftYAxis),
			-m_driverJoystick->GetRawAxis(DualAction::RightXAxis), false,
			m_driverJoystick->GetRawButton(DualAction::RightBumper));
			*/
	/*
	m_drive->ArcadeDrive(m_driverJoystick->GetRawAxis(DualAction::LeftYAxis),
			-m_driverJoystick->GetRawAxis(DualAction::RightXAxis));
			*/

 	/*
	printf("gyro reading %lf... raw counts %d\n", m_gyroEncoder->GetDistance(),
			m_gyroEncoder->GetRaw());
			*/
}

void Robot::ObserveJoystickStateChange(uint32_t port, uint32_t button,
			bool pressedP) {
	static double goal = 5500.0;

	if (port == DRIVER_JOYSTICK_PORT) {
		/* Add any bindings for the driver's joystick here */
	}
	else if (port == OPERATOR_JOYSTICK_PORT) {
		switch (button) {
			case DualAction::BtnA:
				if (pressedP) {
					goal -= 50.0;
					m_shooter->SetFlywheelSSShoot(goal);
				}
				break;
			case DualAction::BtnB:
				if (pressedP) {
					goal += 50.0;
					m_shooter->SetFlywheelSSShoot(goal);
				}
				break;
			case DualAction::BtnX:
				m_shooter->SetFlywheelPower(0.0);
				break;
			case DualAction::BtnY:
				m_shooter->SetFlywheelPower(0.7);
				printf("pressed Y");
				break;
			case DualAction::LeftBumper:
				break;
			case DualAction::RightBumper:
				break;
		}
	}

	DBStringPrintf(DBStringPos::DB_LINE4,
			"Flywheel goal: %lf", goal);

}
