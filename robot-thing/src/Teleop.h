#include "lib/JoystickHelper.h"
#include "lib/WrapDash.h"

void Robot::TeleopStart(void) {
    printf("***teleop start\n");
}

void Robot::TeleopStop(void) {
    printf("***teleop stop\n");
}

static bool teleopDrive = true;
static bool needsStop = false;

void Robot::TeleopContinuous(void) {
	double armSpeed = -m_operatorJoystick->GetRawAxisWithDeadband(DualAction::RightYAxis, 0.2);
	if (armSpeed != 0.0) {
		m_arm->SetTargetSpeed(armSpeed);
		needsStop = true;
	}
	else if (needsStop) {
		m_arm->SetTargetSpeed(0.0);
		needsStop = false;
	}
	/*
	m_drive->CheesyDrive(m_driverJoystick->GetRawAxis(DualAction::LeftYAxis),
			-m_driverJoystick->GetRawAxis(DualAction::RightXAxis), false,
			m_driverJoystick->GetRawButton(DualAction::RightBumper));
			*/

	double y = m_driverJoystick->GetRawAxis(DualAction::LeftYAxis);
	double x = m_driverJoystick->GetRawAxis(DualAction::RightXAxis);

	if (m_driverJoystick->GetRawButton(DualAction::LeftBumper)) {
		y *= 0.3;
		x *= 0.3;
	}
	DBStringPrintf(DBStringPos::DB_LINE5,
			"slow: %d", m_driverJoystick->GetRawButton(DualAction::LeftBumper));

	if (teleopDrive) {
		m_drive->ArcadeDrive(y, -x);
	}

 	/*
	printf("gyro reading %lf... raw counts %d\n", m_gyroEncoder->GetDistance(),
			m_gyroEncoder->GetRaw());
			*/
}

void Robot::ObserveJoystickStateChange(uint32_t port, uint32_t button,
			bool pressedP) {
	static double power = 0.98;
	static double goal = 5000.0;

	printf("joystick state change port %d button %d state %d\n", port, button, pressedP);

	if (port == DRIVER_JOYSTICK_PORT) {
		switch (button) {
		case DualAction::DPadRightVirtBtn:
			if (pressedP) {
				m_shooter->SetElevatorHeight(Shooter::ElevatorHeight::midLow);
			}
			break;
		case DualAction::DPadUpVirtBtn:
			if (pressedP) {
				m_shooter->SetElevatorHeight(Shooter::ElevatorHeight::wayHigh);
			}
			break;
		case DualAction::DPadDownVirtBtn:
			if (pressedP) {
				m_shooter->SetElevatorHeight(Shooter::ElevatorHeight::wayLow);
			}
			break;
		case DualAction::DPadLeftVirtBtn:
			if (pressedP) {
				m_shooter->SetElevatorHeight(Shooter::ElevatorHeight::midHigh);
			}
			break;
		case DualAction::BtnB:
			if (pressedP) {
				m_drive->SetGearing(Drive::DriveGearing::LowGear);
			}
			break;
		case DualAction::BtnX:
			if (pressedP) {
				//teleopDrive = false;
				//m_drive->RampPIDTurn(-5.0);
			}
			break;
		case DualAction::BtnY:
			if (pressedP) {
				//teleopDrive = false;
				//m_drive->RampPIDDrive(5.0);
			}
			break;
		case DualAction::BtnA:
			if (pressedP) {
				m_drive->SetGearing(Drive::DriveGearing::HighGear);
			}
			break;
		case DualAction::RightTrigger:
			teleopDrive = true;
			break;
		}
	}
	else if (port == OPERATOR_JOYSTICK_PORT) {
		switch (button) {
		case DualAction::LXAxisVirtButton:
			if (pressedP) {
				m_intake->SetIntakePosition(Intake::IntakePosition::extended);
			}
			else {
				m_intake->SetIntakePosition(Intake::IntakePosition::retracted);
			}
			break;
		case DualAction::BtnA:
			if (pressedP) {
				m_intake->SetIntakeMode(Intake::IntakeMode::forward);
			}
			else {
				m_intake->SetIntakeMode(Intake::IntakeMode::off);
			}
			break;
		case DualAction::BtnB:
			if (pressedP) {
				m_intake->SetIntakeMode(Intake::IntakeMode::reverse);
			}
			else {
				m_intake->SetIntakeMode(Intake::IntakeMode::off);
			}
			break;
		case DualAction::BtnX:
			if (pressedP) {
				m_shooter->SetConveyerPower(-8.0);
				//m_intake->SetUpperIntakeMode(Intake::UpperIntakeMode::forward);
			}
			else {
				m_shooter->SetConveyerPower(0.0);
				//m_intake->SetUpperIntakeMode(Intake::UpperIntakeMode::off);
			}
			break;
		case DualAction::BtnY:
			if (pressedP) {
				m_shooter->SetConveyerPower(1.0);
				//m_intake->SetUpperIntakeMode(Intake::UpperIntakeMode::reverse);
			}
			else {
				m_shooter->SetConveyerPower(0.0);
				//m_intake->SetUpperIntakeMode(Intake::UpperIntakeMode::off);
			}
			break;
		case DualAction::LeftBumper:
			if (pressedP) {
				m_arm->SetTargetPosition(Arm::ARM_POS_UP);
			}
			break;
		case DualAction::LeftTrigger:
			if (pressedP) {
				m_arm->SetTargetPosition(Arm::ARM_POS_DOWN);
			}
			break;
		case DualAction::RightBumper:
			if (pressedP) {
				m_shooter->SetFlywheelPower(1.0);
			}
			else {
				m_shooter->SetFlywheelPower(0.0);
			}
			break;
		case DualAction::RightTrigger:
			m_shooter->Print();
			m_arm->PrintStats();
			break;
		case DualAction::DPadUpVirtBtn:
			if (pressedP) {
				power += 0.02;
				goal += 50.0;
				m_shooter->SetFlywheelPower(power);
				//m_shooter->SetFlywheelSSShoot(goal);
			}
			break;
		case DualAction::DPadDownVirtBtn:
			if (pressedP) {
				power -= 0.02;
				goal -= 50.0;
				m_shooter->SetFlywheelPower(power);
				//m_shooter->SetFlywheelSSShoot(goal);
			}
			break;
		case DualAction::DPadLeftVirtBtn:
			if (pressedP) {
				m_shooter->SetFlywheelStop();
			}
			break;
		}
	}


	DBStringPrintf(DBStringPos::DB_LINE4,
			"Flywheel pow: %lf", power);
			/*
	DBStringPrintf(DBStringPos::DB_LINE4,
			"Flywheel goal: %lf", goal);
			*/

}
