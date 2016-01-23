#include "lib/JoystickHelper.h"
#include "lib/WrapDash.h"

void Robot::TeleopStart(void) {
    printf("***teleop start\n");
}

void Robot::TeleopStop(void) {
    printf("***teleop stop\n");
}

void Robot::TeleopContinuous(void) {
	/**
	 * flywheel shoot with right trigger
	 * flywheel intake with Y button
	 * default state stop
	 */


	static bool wasAPressed = false;
	static bool wasBPressed = false;
	static double goal = 3500.0;

	if (m_operatorJoystick->GetRawButton(DualAction::BtnA)) {
		if (!wasAPressed) {
			printf("increment power\n");
			goal += 50.0;
			m_shooter->SetFlywheelSSShoot(goal);
			wasAPressed = true;
		}
	}
	else {
		wasAPressed = false;
	}


	if (m_operatorJoystick->GetRawButton(DualAction::BtnB)) {
		if (!wasBPressed) {
			printf("decrement power\n");
			goal -= 50.0;
			m_shooter->SetFlywheelSSShoot(goal);
			wasBPressed = true;
		}
	}
	else {
		wasBPressed = false;
	}

	DBStringPrintf(DBStringPos::DB_LINE4,
			"Flywheel goal: %lf", goal);


	if (m_operatorJoystick->GetRawButton(DualAction::BtnB)) {
		m_shooter->SetFlywheelPower(0.7);
	}

	/*
	if (m_operatorJoystick->GetRawButton(DualAction::BtnY)) {
		printf("state space\n");
		m_shooter->SetFlywheelSSShoot();
	}
	*/
 	if (m_operatorJoystick->GetRawButton(DualAction::BtnX)) {
 		printf("stop\n");
 		//pow = 0.0;
		m_shooter->SetFlywheelPower(0.0);
	}

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
