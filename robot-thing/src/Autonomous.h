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
	if (m_selectedAutoRoutine == ONE_BALL_AUTO) {
		this->OneBallAuto();
	}
	else {
		this->TwoBallAuto();
	}
	printf("exiting auto continuous\n");
}

void Robot::OneBallAuto(void) {
	switch (m_autoState) {
	case 0:
		//Start driving
		m_intake->RetractIntake();
		m_drive->PIDDrive(15.5 * 12.0);
		m_autoTimer = GetMsecTime();
		m_autoState++;
		break;
	case 1:
		//continue when we're done driving or if we've hit a timeout
		if (m_drive->OnTarget() || GetMsecTime() - m_autoTimer > 4000) {
			m_autoState++;
		}
		break;
	case 2:
		//Start the shooter
		//m_shooter->SetFlywheelNearShoot();
		m_autoTimer = GetMsecTime();
		m_autoState++;
		break;
	case 3:
		//continue if we're up to speed (and we've taken at least 3 sec) or if we've hit a timeout
		if ((m_shooter->FlywheelOnTarget() && GetMsecTime() - m_autoTimer > 3000) ||
				GetMsecTime() - m_autoTimer > 6000) {
			m_autoState++;
		}
		break;
	case 4:
		//shoot
		m_intake->SetIntakeMode(Intake::IntakeMode::reverse);
		m_autoState++;
		break;
	default:
		//do nothing
		break;
	}
}

void Robot::TwoBallAuto() {
	switch (m_autoState) {
	case 0:
		//start driving (and rev up flywheel)
		m_intake->ExtendIntake();
		//m_shooter->SetFlywheelFarShoot();
		m_drive->PIDDrive(15.5 * 12.0);
		m_autoTimer = GetMsecTime();
		m_autoState++;
		break;
	case 1:
		//continue when we're done moving and flywheel done reving... or if timeout
		if ((m_drive->OnTarget() && m_shooter->FlywheelOnTarget()) ||
				GetMsecTime() - m_autoTimer > 6000) {
			m_autoState++;
		}
		break;
	case 2:
		//retract the intake
		m_intake->RetractIntake();
		m_intake->SetIntakeMode(Intake::IntakeMode::reverse);
		m_autoTimer = GetMsecTime();
		m_autoState++;
		break;
	case 3:
		if (GetMsecTime() - m_autoTimer > 500){
			m_autoState++;
		}
		break;
	case 4:
		//Fire first ball
		//m_intake->RetractIntake();
		m_intake->SetIntakeMode(Intake::IntakeMode::reverse);
		m_autoTimer = GetMsecTime();
		m_autoState++;
		break;
	case 5:
		//wait 2 seconds
		if (GetMsecTime() - m_autoTimer > 2000)
			m_autoState++;
		break;
	case 6:
		//pick up ball two
		m_intake->ExtendIntake();
		m_autoTimer = GetMsecTime();
		m_autoState++;
		break;
	case 7:
		//wait 2 seconds and for flywheel to ramp up again
		if ((m_shooter->FlywheelOnTarget() && GetMsecTime() - m_autoTimer > 2000)
					|| GetMsecTime() - m_autoTimer > 6000) {
			m_autoState++;
		}
		break;
	case 8:
		//retract intake
		m_intake->RetractIntake();
		m_intake->SetIntakeMode(Intake::IntakeMode::reverse);
		m_autoTimer = GetMsecTime();
		m_autoState++;
		break;
	case 9:
		//wait for intake to retract
		if (GetMsecTime() - m_autoTimer > 500){
			m_autoState++;
		}
		break;
	case 10:
		//fire ball 2
		m_intake->SetIntakeMode(Intake::IntakeMode::reverse);
		m_autoState++;
		break;
	default:
		break;
	}
}
