#include "lib/JoystickHelper.h"
#include "lib/WrapDash.h"

namespace frc973 {

void Robot::TeleopStart(void) {
    printf("***teleop start\n");
}

void Robot::TeleopStop(void) {
    printf("***teleop stop\n");
}

static bool teleopDrive = true;
static bool armNeedsStop = false;
static bool conveyorNeedsStop = false;
static bool intakeNeedsStop = false;

void Robot::TeleopContinuous(void) {
	double armPower = -m_operatorJoystick->GetRawAxisWithDeadband(DualAction::RightYAxis, 0.2);
	if (armPower != 0.0) {
		m_arm->SetPower(armPower * 0.5);
		armNeedsStop = true;
	}
	else if (armNeedsStop) {
		m_arm->SetPower(0.0);
		armNeedsStop = false;
	}

	double intakePower = m_operatorJoystick->GetRawAxisWithDeadband(
			DualAction::LeftYAxis, 0.2);
	if (intakePower != 0.0) {
		m_intake->SetIntakePower(intakePower);
		intakeNeedsStop = true;
	}
	else if (intakeNeedsStop) {
		m_intake->SetIntakePower(0.0);
		intakeNeedsStop = false;
	}


	double conveyorPower = m_operatorJoystick->GetRawAxisWithDeadband(
			DualAction::LeftXAxis, 0.2);
	if (conveyorPower != 0.0) {
		m_shooter->SetConveyerPower(conveyorPower);
		conveyorNeedsStop = true;
	}
	else if (conveyorNeedsStop) {
		m_shooter->SetConveyerPower(0.0);
		conveyorNeedsStop = false;
	}
	/*
	m_drive->CheesyDrive(m_driverJoystick->GetRawAxis(DualAction::LeftYAxis),
			-m_driverJoystick->GetRawAxis(DualAction::RightXAxis), false,
			m_driverJoystick->GetRawButton(DualAction::RightBumper));
			*/

	double y = m_driverJoystick->GetRawAxis(DualAction::LeftYAxis);
	double x = m_driverJoystick->GetRawAxis(DualAction::RightXAxis);

	if (Util::abs(x) > 0.2) {
		teleopDrive = true;
	}

	if (teleopDrive) {
		if (m_driverJoystick->GetRawButton(DualAction::LeftBumper)) {
			y *= 0.3;
			x *= 0.3;
		}

		if (teleopDrive) {
			m_drive->ArcadeDrive(y, -x);
		}
	}

 	/*
	printf("gyro reading %lf... raw counts %d\n", m_gyroEncoder->GetDistance(),
			m_gyroEncoder->GetRaw());
			*/
}

void Robot::ObserveJoystickStateChange(uint32_t port, uint32_t button,
			bool pressedP) {

	printf("joystick state change port %d button %d state %d\n", port, button, pressedP);

	if (port == DRIVER_JOYSTICK_PORT) {
		switch (button) {
		case DualAction::BtnA:
			if (pressedP) {
				m_drive->PIDTurn(5.0, Drive::RelativeTo::Now);
			}
			break;
		case DualAction::BtnB:
			if (pressedP) {
				m_drive->PIDTurn(-5.0, Drive::RelativeTo::Now);
			}
			break;
		case DualAction::BtnX:
			if (pressedP) {
				teleopDrive = false;
				m_drive->SetVisionTargeting();
			}
			break;
		case DualAction::BtnY:
			if (pressedP) {
				teleopDrive = true;
				m_drive->ArcadeDrive(0.0, 0.0);
			}
			break;
		case DualAction::LeftBumper:
			if (pressedP) {
				m_drive->SetGearing(Drive::DriveGearing::LowGear);
			}
			break;
		case DualAction::LeftTrigger:
			if (pressedP) {
				m_drive->SetGearing(Drive::DriveGearing::HighGear);
			}
			break;
		case DualAction::RightBumper:
			if (pressedP) {
				m_poseManager->AssumePose();
			}
			break;
		case DualAction::RightTrigger:
			if (pressedP) {
				m_intake->SetIntakeMode(Intake::IntakeMode::forward);
				m_shooter->SetConveyerPower(1.0);
			}
			break;
		}
	}
	else if (port == OPERATOR_JOYSTICK_PORT) {
		switch (button) {
		case DualAction::BtnA:
			if (pressedP) {
				m_poseManager->ChooseNthPose(PoseManager::BATTER_SHOT_PPOSE);
			}
			break;
		case DualAction::BtnB:
			if (pressedP) {
				m_poseManager->ChooseNthPose(PoseManager::STOW_POSE);
			}
			break;
		case DualAction::BtnX:
			if (pressedP) {
				m_poseManager->ChooseNthPose(PoseManager::FAR_DEFENSE_SHOT_POSE);
			}
			break;
		case DualAction::BtnY:
			if (pressedP) {
				m_poseManager->ChooseNthPose(PoseManager::NEAR_DEFENSE_SHOT_POSE);
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
				m_intake->SetIntakeMode(Intake::IntakeMode::forward);
				m_shooter->SetConveyerPower(-8.0);
			}
			else {
				m_intake->SetIntakeMode(Intake::IntakeMode::off);
				m_shooter->SetConveyerPower(0.0);
			}
			break;
		case DualAction::RightTrigger:
			if (pressedP) {
				m_intake->SetIntakeMode(Intake::IntakeMode::forward);
				m_shooter->SetConveyerPower(1.0);
			}
			else {
				m_intake->SetIntakeMode(Intake::IntakeMode::off);
				m_shooter->SetConveyerPower(0.0);
			}
			break;
		case DualAction::DPadUpVirtBtn:
			if (pressedP) {
				m_shooter->SetFlywheelEnabled(false);
			}
			break;
		case DualAction::DPadDownVirtBtn:
			if (pressedP) {
				m_shooter->SetFlywheelEnabled(true);
			}
			break;
		case DualAction::DPadLeftVirtBtn:
			m_intake->SetIntakePosition(Intake::IntakePosition::extended);
			break;
		case DualAction::DPadRightVirtBtn:
			m_intake->SetIntakePosition(Intake::IntakePosition::retracted);
			break;
		}
	}
			/*
	DBStringPrintf(DBStringPos::DB_LINE4,
			"Flywheel goal: %lf", goal);
			*/
}

}
