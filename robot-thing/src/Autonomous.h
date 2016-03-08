namespace frc973 {

void Robot::AutonomousStart(void) {
    printf("***auto start\n");

    m_shooter->SetFlywheelStop();
	m_intake->SetIntakeMode(Intake::IntakeMode::off);

	m_drive->Zero();

	m_autoState = 0;
}

void Robot::AutonomousStop(void) {
	printf("***auto stop\n");
}

void Robot::AutonomousContinuous(void) {

	printf("entering auto continuous\n");
	//if (m_selectedAutoRoutine == ONE_BALL_AUTO) {
	//this->Flappers();
	this->Portcullis();
	/*}
	else {
		this->TwoBallAuto();
	}*/
	printf("exiting auto continuous\n");
}

//m_autoState
//m_autoTimer
void Robot::Flappers(void) {
	switch(m_autoState){
	case 0:
		m_arm->SetTargetPosition(30);
		m_autoState++;
		break;
	case 1:
		m_drive->PIDDrive(53, Drive::RelativeTo::SetPoint);
		m_autoState++;
		break;
	case 2:
		if (m_drive->OnTarget()){
			m_autoState++;
		}
		break;
	case 3:
		m_arm->SetTargetPosition(6);
		m_autoState++;
		break;
	case 4:
		m_autoTimer = GetMsecTime();
		m_autoState++;
		break;
	case 5:
		if ((GetMsecTime() - m_autoTimer) > 1000){
			m_autoState++;
		}
		break;
	case 6:
		m_drive->PIDDrive(97, Drive::RelativeTo::SetPoint);
		m_autoTimer = GetMsecTime();
		m_autoState++;
		break;
	case 7:
		if ((GetMsecTime() - m_autoTimer) > 500){
			m_autoState++;
		}
		break;
	case 8:
		m_arm->SetTargetPosition(50);
		m_autoState++;
		break;
	case 9:
		if (m_drive->OnTarget()){
			m_autoState++;
		}
		break;
	case 10:
		m_drive->PIDTurn(180, Drive::RelativeTo::Absolute);
		m_autoState++;
		break;
	case 11:
		break;
	}
}

static int repeats = 0;

void Robot::TurnTest() {
	switch (m_autoState) {
	case 0:
		m_drive->PIDDrive(48, Drive::RelativeTo::Now);
		m_autoState++;
		break;
	case 1:
		if (m_drive->OnTarget()){
			m_autoState++;
		}
		break;
	case 2:
		m_drive -> PIDTurn (180, Drive::RelativeTo::SetPoint);
		m_autoState++;
		break;
	case 3:
		if (m_drive->OnTarget()){
			m_autoState = 0;
			repeats = repeats + 1;
		}
		break;
	}
	DBStringPrintf(DBStringPos::DB_LINE3, "repeats %d", repeats);
}

void Robot::Portcullis() {
	switch (m_autoState){
	case 0:
		//m_intake->SetIntakePosition(Intake::IntakePosition::extended);
		m_drive->PIDDrive(52.0, Drive::RelativeTo::SetPoint);
		m_autoState++;
		break;
	case 1:
		if (m_drive->OnTarget()) {
			m_autoState++;
		}
		break;
	case 2:
		m_arm->SetTargetPosition(45.0);
		m_autoTimer = GetSecTime();
		m_autoState ++;
		break;
	case 3:
		if (GetSecTime() - m_autoTimer > 0.2) {
			m_autoState++;
		}
		break;
	case 4:
		m_drive->PIDDrive(72.0, Drive::RelativeTo::SetPoint);
		m_autoState++;
		break;
	default:
		break;
	}
}

void Robot::Moat() {
	switch (m_autoState){
	case 0:
		m_drive->SetGearing(Drive::DriveGearing::LowGear);
		m_drive->PIDDrive(120.0, Drive::RelativeTo::Absolute);
		m_autoState++;
		break;
	default:
		break;
	}
}

}
