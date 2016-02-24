void Robot::AutonomousStart(void) {
    printf("***auto start\n");

    m_shooter->SetFlywheelStop();
	m_intake->SetLowerIntakeMode(Intake::LowerIntakeMode::off);

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
	this->TurnTest();
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
		m_drive->PIDDrive(53);
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
		m_drive->PIDDrive(97);
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
		m_drive->PIDTurn(180);
		m_autoState++;
		break;
	case 11:
		break;
	}
}

void Robot::TurnTest() {
	switch (m_autoState) {
	case 0:
		m_drive->PIDTurn(360);
		m_autoState++;
		break;
	case 1:
		if (m_drive->OnTarget()){
			m_autoState++;
		}
		break;
	case 2:
		m_autoTimer = GetMsecTime();
		m_autoState++;
		break;
	case 3:
		if (GetMsecTime() - m_autoTimer > 1000){
			m_autoState = 0;
		}
		break;
	}
}
