namespace frc973 {

static constexpr double POS_2_TURN = 30.0;
static constexpr double POS_3_TURN = 5.0;
static constexpr double POS_4_TURN = -16.0;
static constexpr double POS_5_TURN = -23.22;

static constexpr double POS_2_DIST = 3.0 * 12.0;
static constexpr double POS_3_DIST = 0.0;
static constexpr double POS_4_DIST = 0.0;
static constexpr double POS_5_DIST = 0.0 * 12.0;

double AutonomousExtraDist (Robot::AutoStartPosition selectedDirection){
	double EXTRA_DIST = 0.0;
	if (selectedDirection == Robot::AutoStartPosition::Pos2){
		EXTRA_DIST = POS_2_DIST;
	}
	else if (selectedDirection == Robot::AutoStartPosition::Pos3){
		EXTRA_DIST = POS_3_DIST;
	}
	else if (selectedDirection == Robot::AutoStartPosition::Pos4){
		EXTRA_DIST = POS_4_DIST;
	}
	else if (selectedDirection == Robot::AutoStartPosition::Pos5){
		EXTRA_DIST = POS_5_DIST;
	}
	else if (selectedDirection == Robot::AutoStartPosition::NoVision){
		EXTRA_DIST = 0.0;
	}
	return EXTRA_DIST;
}

double AutonomousTurn (Robot::AutoStartPosition selectedDirection){
	double AUTO_TURN = 0.0;
	if (selectedDirection == Robot::AutoStartPosition::Pos2){
		AUTO_TURN = POS_2_TURN;
	}
	else if (selectedDirection == Robot::AutoStartPosition::Pos3){
		AUTO_TURN = POS_3_TURN;
	}
	else if (selectedDirection == Robot::AutoStartPosition::Pos4){
		AUTO_TURN = POS_4_TURN;
	}
	else if (selectedDirection == Robot::AutoStartPosition::Pos5){
		AUTO_TURN = POS_5_TURN;
	}
	else if (selectedDirection == Robot::AutoStartPosition::NoVision){
		AUTO_TURN = 0.0;
	}
	return AUTO_TURN;
}

void Robot::AutonomousStart(void) {
    printf("***auto start\n");

    m_shooter->SetFlywheelStop();
	m_intake->SetIntakeMode(Intake::IntakeMode::off);

	m_drive->Zero();
	m_drive->SetBraking(false);

	m_autoState = 0;
}

void Robot::AutonomousStop(void) {
	printf("***auto stop\n");
}

void Robot::AutonomousContinuous(void) {
	switch (m_selectedRoutine){
	case AutoRoutine::Portcullis:
		PortcullisAuto();
		break;
	case AutoRoutine::ChevaldeFrise:
		Flappers();
		break;
	case AutoRoutine::Drawbridge:
		DrawbridgeAuto();
		break;
	case AutoRoutine::Go:
		Moat();
		break;
	case AutoRoutine::SallyPort:
		SallyPortAuto();
		break;
	case AutoRoutine:: SpyBot:
		SpyBotAuto();
		break;
	case AutoRoutine::NoAuto:
		//Don't do any auto
		break;
	}
}

//m_autoState
//m_autoTimer
void Robot::Flappers(void) {
	switch(m_autoState){
	case 0:
		m_poseManager->ChooseNthPose(m_poseManager->CHIVAL_POSE);
		m_drive->PIDDrive(143 + AutonomousExtraDist(m_selectedDirection), Drive::RelativeTo::SetPoint);
		m_autoState++;
		break;
	case 1:
		if (m_drive->OnTarget()){
			m_autoState++;

			if (m_selectedDirection == NoVision) {
				m_autoState += 1000;
			}
		}
		break;
	case 2:
		m_drive->PIDTurn(AutonomousTurn(m_selectedDirection), Drive::RelativeTo::Now);
		m_poseManager->ChooseNthPose(PoseManager::NEAR_DEFENSE_SHOT_POSE);
		m_shooter->SetFlywheelEnabled(true);
		m_autoTimer = GetMsecTime();
		m_autoState++;
		break;
	case 3:
		if (m_drive->OnTarget() || GetMsecTime() - m_autoTimer > 2000) {
			m_drive->SetVisionTargeting();

			m_autoTimer = GetMsecTime();
			m_autoState ++;
		}
		break;
	case 4:
		if (m_drive->OnTarget()){
			m_shooter->SetConveyerPower(1.0);

			m_autoState ++;
		}
		break;
	default:
		break;
	}
}

void Robot::PortcullisAuto() {
	switch (m_autoState){
	case 0:
		m_autoTimer = GetMsecTime();
		//m_arm->Zero();0
		//m_arm->SetTargetPosition(Arm::ARM_POS_PORTCULLIS);
		m_autoState++;
		break;
	case 1:
		if (GetMsecTime() - m_autoTimer > 2000){
			m_autoState ++;
		}
		break;
	case 2:
		m_drive->PIDDrive(148.0 + AutonomousExtraDist(m_selectedDirection), Drive::RelativeTo::SetPoint);
		m_autoState ++;
		break;
	case 3:
		if (m_drive->OnTarget()) {
			//m_arm->SetTargetPosition(20.0);
			m_autoState++;

			if (m_selectedDirection == AutoStartPosition::NoVision) {
				m_autoState += 1000;
			}
		}
		break;
	case 4:
		m_drive->PIDTurn(AutonomousTurn(m_selectedDirection), Drive::RelativeTo::Now);
		m_poseManager->ChooseNthPose(PoseManager::NEAR_DEFENSE_SHOT_POSE);
		m_shooter->SetFlywheelEnabled(true);
		m_autoTimer = GetMsecTime();
		m_autoState ++;
		break;
	case 5:
		if (m_drive->OnTarget() && GetMsecTime() - m_autoTimer > 5000){
			m_autoState ++;
		}
		break;
	case 6:
		m_drive->SetVisionTargeting();
		m_autoState++;
		break;
	case 7:
		if (m_drive->OnTarget()){
			m_autoState ++;
		}
		break;
	case 8:
		m_shooter->SetConveyerPower(1.0);
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
		m_drive->PIDDrive(180 + AutonomousExtraDist(m_selectedDirection), Drive::RelativeTo::Now);
		//remember 120 failed for almost everything
		m_autoState++;
		break;
	case 1:
		if (m_drive->OnTarget()){
			m_autoState ++;

			if (m_selectedDirection == NoVision){
				m_autoState += 1000;
			}
		}
		break;
	case 2:
		m_drive->PIDTurn(AutonomousTurn(m_selectedDirection), Drive::RelativeTo::Now);
		m_autoTimer = GetMsecTime();
		m_autoState ++;
		break;
	case 3:
		if (m_drive->OnTarget() || GetMsecTime() - m_autoTimer > 4000){
			m_autoState ++;
		}
		break;
	case 4:
		m_autoTimer = GetMsecTime();
		m_poseManager->ChooseNthPose(PoseManager::NEAR_DEFENSE_SHOT_POSE);
		//m_arm->SetTargetPosition(Arm::ARM_POS_SHOOT);
		m_shooter->SetFlywheelEnabled(true);
		m_autoState ++;
		break;
	case 5:
		if (m_drive->OnTarget() || GetMsecTime() - m_autoTimer > 2000){
			m_autoState ++;
		}
		break;
	case 6:
		m_drive->SetVisionTargeting();
		m_autoState++;
		m_autoTimer = GetMsecTime();
		break;
	case 7:
		if (GetMsecTime() - m_autoTimer > 2700 && m_drive->OnTarget()) {
			m_autoState ++;
		}
		break;
	case 8:
		m_shooter->SetConveyerPower(1.0);
		m_autoState++;
		break;
	default:
		break;
	}
}


void Robot::DrawbridgeAuto() {
	switch (m_autoState) {
		case 0:
			m_drive->PIDDrive(72.0, Drive::RelativeTo::Now);
			//m_arm->SetTargetPosition(45.0);
			m_autoState ++;
			break;
		case 1:
			if (m_drive->OnTarget()){
				m_autoState ++;
			}
			break;
		case 2:
			//m_arm->SetTargetPosition(0.0);
			m_drive->PIDDrive(-24.0, Drive::RelativeTo::SetPoint);
			m_autoState ++;
			break;
		case 3:
			if (m_drive->OnTarget()){
				m_autoState ++;
			}
			break;
		case 4:
			m_drive->PIDDrive(132.0, Drive::RelativeTo::SetPoint);
			m_autoState ++;
			break;
		case 5:
			m_drive->PIDTurn(AutonomousTurn(m_selectedDirection), Drive::RelativeTo::Now);
			if (m_selectedDirection == AutoStartPosition::NoVision){
				m_autoState += 100;
			}
			m_poseManager->ChooseNthPose(PoseManager::NEAR_DEFENSE_SHOT_POSE);
			break;
		case 6:
			if (m_drive->OnTarget()){
				m_autoState ++;
			}
			break;
		case 7:
			m_drive->SetVisionTargeting();
			m_shooter->SetFlywheelEnabled(true);
			m_autoState++;
			break;
		case 8:
			if (m_drive->OnTarget()){
				m_autoState ++;
			}
			break;
		case 9:
			m_shooter->SetConveyerPower(1.0);
			m_autoState++;
			break;
		default:
			break;
	}
}

void Robot::SallyPortAuto() {
	switch (m_autoState) {
		case 0:
			m_drive->PIDDrive(72.0, Drive::RelativeTo::Now);
			//m_arm->SetTargetPosition(45.0);
			m_autoState ++;
			break;
		case 1:
			if (m_drive->OnTarget()){
				m_autoState ++;
			}
			break;
		case 2:
			//m_arm->SetTargetPosition(0.0);
			m_autoState ++;
			break;
		case 3:
			m_drive->PIDDrive(-24.0, Drive::RelativeTo::SetPoint);
			m_autoState ++;
			break;
		case 4:
			if (m_drive->OnTarget()){
				m_autoState ++;
			}
			break;
		case 5:
			m_drive->PIDDrive(132.0, Drive::RelativeTo::SetPoint);
			m_autoState ++;
			break;
		case 6:

			m_drive->PIDTurn(AutonomousTurn(m_selectedDirection), Drive::RelativeTo::Now);

			if (m_selectedDirection == AutoStartPosition::NoVision){
				m_autoState += 100;
			}
			m_poseManager->ChooseNthPose(PoseManager::NEAR_DEFENSE_SHOT_POSE);
			break;
		case 7:
			if (m_drive->OnTarget()){
				m_autoState ++;
			}
			break;
		case 8:
			m_drive->SetVisionTargeting();
			m_shooter->SetFlywheelEnabled(true);
			m_autoState++;
			break;
		case 9:
			if (m_drive->OnTarget()){
				m_autoState ++;
			}
			break;
		case 10:
			m_shooter->SetConveyerPower(1.0);
			m_autoState++;
			break;
		default:
			break;
	}
}

void Robot::SpyBotAuto(){
	switch (m_autoState){
	case 0:
		m_poseManager->ChooseNthPose(PoseManager::NEAR_DEFENSE_SHOT_POSE);
		//m_arm->SetTargetPosition(Arm::ARM_POS_SHOOT);
		m_shooter->SetFlywheelEnabled(true);
		m_autoTimer = GetSecTime();
		m_autoState ++;
		break;
	case 1:
		if (GetSecTime() - m_autoTimer > 6) {
			m_autoState ++;
		}
		break;
	case 2:
		m_shooter->SetConveyerPower(1.0);
		break;
	}
}

}
