#include "lib/JoystickHelper.h"

namespace frc973 {

void Robot::DisabledStart(void) {
    printf("***disable start\n");

    //m_shooter->SetFlywheelStop();
	m_intake->SetIntakeMode(Intake::IntakeMode::off);
	printf("disable start end \n");
}

void Robot::DisabledStop(void) {

}

void Robot::DisabledContinuous(void) {
	/*
	printf("distance: %lf, speed: %lf\n", m_drive->GetDist(),
			m_drive->GetRate());
	printf("angle: %lf, angular rate: %lf\n", m_drive->GetAngle(),
			m_drive->GetAngularRate());
			*/

/*
	if (m_operatorJoystick->GetRawButton(DualAction::BtnA)) {
		m_selectedAutoRoutine = ONE_BALL_AUTO;
		printf("one ball auto selected\n");
		SmartDashboard::PutString("DB/String 0", "One ball auto");
	}
	else if (m_operatorJoystick->GetRawButton(DualAction::BtnB)) {
		m_selectedAutoRoutine = TWO_BALL_AUTO;
		printf("two ball auto selected\n");
		SmartDashboard::PutString("DB/String 0", "Two ball auto");
	}
	*/
}
void Robot::HandleDisabledButton(uint32_t port, uint32_t button,
		bool pressedP){
	if (port == DRIVER_JOYSTICK_PORT) {
		switch (button) {
		case DualAction::BtnA:
			if (pressedP) {
				m_selectedRoutine = AutoRoutine::Portcullis;
				DBStringPrintf(DBStringPos::DB_LINE6, "Portcullis Auto");
			}
			break;
		case DualAction::BtnB:
			if (pressedP) {
				m_selectedRoutine = AutoRoutine::ChevaldeFrise;
				DBStringPrintf(DBStringPos::DB_LINE6, "ChevaldeFrise Auto");
			}
			break;
		case DualAction::BtnX:
			if (pressedP) {
				m_selectedRoutine = AutoRoutine::Drawbridge;
				DBStringPrintf(DBStringPos::DB_LINE6, "Drawbridge Auto");
			}
			break;
		case DualAction::BtnY:
			if (pressedP) {
				m_selectedRoutine = AutoRoutine::Go;
				DBStringPrintf(DBStringPos::DB_LINE6, "Forward Auto");
			}
			break;
		case DualAction::RightBumper:
			if (pressedP) {
				m_selectedRoutine = AutoRoutine::SpyBot;
				DBStringPrintf(DBStringPos::DB_LINE6, "SpyBot Auto");
			}
			break;
		case DualAction::Back:
			if (pressedP) {
				m_selectedRoutine = AutoRoutine::NoAuto;
				DBStringPrintf(DBStringPos::DB_LINE6, "No Auto (Just sit)");
			}
			break;
		case DualAction::DPadUpVirtBtn:
			if (pressedP) {
				m_selectedDirection = AutoStartPosition::Pos2;
				DBStringPrintf(DBStringPos::DB_LINE9, "Position 2");
			}
			break;
		case DualAction::DPadRightVirtBtn:
			if (pressedP) {
				m_selectedDirection = AutoStartPosition::Pos3;
				DBStringPrintf(DBStringPos::DB_LINE9, "Position 3");
			}
			break;
		case DualAction::DPadDownVirtBtn:
			if (pressedP) {
				m_selectedDirection = AutoStartPosition::Pos4;
				DBStringPrintf(DBStringPos::DB_LINE9, "Position 4");
			}
			break;
		case DualAction::DPadLeftVirtBtn:
			if (pressedP) {
				m_selectedDirection = AutoStartPosition::Pos5;
				DBStringPrintf(DBStringPos::DB_LINE9, "Position 5");
			}
			break;
		case DualAction::LeftBumper:
			if (pressedP) {
				m_selectedDirection = AutoStartPosition::NoVision;
				DBStringPrintf(DBStringPos::DB_LINE9, "No Vision");
			}
			break;
		}
	}

}

}
