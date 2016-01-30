#include "lib/JoystickHelper.h"

void Robot::DisabledStart(void) {
    printf("***disable start\n");

    m_shooter->SetFlywheelStop();
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

