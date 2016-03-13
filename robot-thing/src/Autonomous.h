namespace frc973 {

//2 -
//3 -
//4 -
//5 -

static constexpr int VISION_START = 1000;

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
		//m_arm->SetTargetPosition(30);
		m_autoState++;
		break;
	case 1:
		m_drive->PIDDrive(46, Drive::RelativeTo::SetPoint);
		m_autoState++;
		break;
	case 2:
		if (m_drive->OnTarget()){
			m_autoState++;
		}
		break;
	case 3:
		//m_arm->SetTargetPosition(6);
		m_arm->SetPower(-0.4);
		m_autoState++;
		break;
	case 4:
		m_autoTimer = GetMsecTime();
		m_autoState++;
		break;
	case 5:
		if ((GetMsecTime() - m_autoTimer) > 2000){
			m_autoState++;
		}
		break;
	case 6:
		m_drive->PIDDrive(97, Drive::RelativeTo::SetPoint);
		m_autoTimer = GetMsecTime();
		m_arm->SetPower(0.3);
		m_autoState++;
		break;
	case 7:
		if ((GetMsecTime() - m_autoTimer) > 1500){
			m_autoState++;
		}
		break;
	case 8:
		m_arm->SetPower(0.0);
		m_autoState++;
		break;
	case 9:
		if (m_drive->OnTarget()){
			m_autoState++;
		}
		break;
	case 10:
		if (m_selectedDirection == AutoSearchDirection::None){
			m_autoState += 2;
			m_arm->SetPower(-0.2);
		}
		else if (m_selectedDirection == AutoSearchDirection::Left){
			m_drive->PIDTurn(-25.0, Drive::RelativeTo::Now);
			m_arm->SetPower(-0.2);
			m_autoState ++;
		}
		else if (m_selectedDirection == AutoSearchDirection::Right){
			m_drive->PIDTurn(25.0, Drive::RelativeTo::Now);
			m_arm->SetPower(-0.2);
			m_autoState ++;
		}
		else if (m_selectedDirection == AutoSearchDirection::NoVision){
			m_autoState += 100;
		}
		m_autoTimer = GetMsecTime();
		m_poseManager->ChooseNthPose(PoseManager::NEAR_DEFENSE_SHOT_POSE);
		break;
	case 11:
		if (m_drive->OnTarget() || GetMsecTime() - m_autoTimer > 2000) {
			m_autoState ++;
		}
		break;
	case 12:
		m_arm->SetPower(0.0);
		m_autoTimer = GetMsecTime();
		//m_drive->SetVisionTargeting();
		m_shooter->SetFlywheelEnabled(true);
		m_autoState++;
		break;
	case 13:
		if (m_drive->OnTarget() || GetMsecTime() - m_autoTimer > 3000){
			m_autoState ++;
		}
		break;
	case 14:
		m_shooter->SetConveyerPower(1.0);
		m_autoState++;
		break;
	default:
		break;
	}
}

static int repeats = 0;

void Robot::TurnTest() {
	switch (m_autoState) {
	case 0:
		m_drive->PIDTurn(10, Drive::RelativeTo::Now);
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
	default:
		break;
	}
	DBStringPrintf(DBStringPos::DB_LINE4, "repeats %d", repeats);
}

void Robot::PortcullisAuto() {
	switch (m_autoState){
	case 0:
		//m_intake->SetIntakePosition(Intake::IntakePosition::extended);
		m_autoTimer = GetSecTime();
		//m_arm->Zero();0
		m_arm->SetPower(-0.4);
		m_autoState++;
		break;
	case 1:
		if (GetSecTime() - m_autoTimer > 2.0){
			m_arm->SetPower(0.0);
			m_autoState ++;
		}
		break;
	case 2:
		m_drive->PIDDrive(52.0, Drive::RelativeTo::SetPoint);
		m_autoState ++;
		break;
	case 3:
		if (m_drive->OnTarget()) {
			m_autoState++;
		}
		break;
	case 4:
		m_arm->SetPower(0.3);
		m_autoTimer = GetSecTime();
		m_autoState ++;
		break;
	case 5:
		if (GetSecTime() - m_autoTimer > 1.0) {
			m_arm->SetPower(0.0);
			m_autoState++;
		}
		break;
	case 6:
		m_drive->PIDDrive(72.0, Drive::RelativeTo::SetPoint);
		m_autoState++;
		break;
	case 7:
		if (m_selectedDirection == AutoSearchDirection::None){
			m_autoState += 2;
		}
		else if (m_selectedDirection == AutoSearchDirection::Left){
			m_drive->PIDTurn(-25.0, Drive::RelativeTo::Now);
			m_autoState ++;
		}
		else if (m_selectedDirection == AutoSearchDirection::Right){
			m_drive->PIDTurn(25.0, Drive::RelativeTo::Now);
			m_autoState ++;
		}
		else if (m_selectedDirection == AutoSearchDirection::NoVision){
			m_autoState += 100;
		}
		m_poseManager->ChooseNthPose(PoseManager::NEAR_DEFENSE_SHOT_POSE);
		break;
	case 8:
		if (m_drive->OnTarget()){
			m_autoState ++;
		}
		break;
	case 9:
		//0m_drive->SetVisionTargeting();
		m_shooter->SetFlywheelEnabled(true);
		m_autoState++;
		break;
	case 10:
		if (m_drive->OnTarget()){
			m_autoState ++;
		}
		break;
	case 11:
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
		m_drive->PIDDrive(180.0, Drive::RelativeTo::Now);
		//remember 120 failed for almost everything
		m_autoState++;
		break;
	case 1:
		if (m_drive->OnTarget()){
			m_autoState ++;
		}
		break;
	case 2:
		if (m_selectedDirection == AutoSearchDirection::None){
			m_autoState += 2;
		}
		else if (m_selectedDirection == AutoSearchDirection::Left){
			m_drive->PIDTurn(-5.0, Drive::RelativeTo::Now);
			m_autoState ++;
		}
		else if (m_selectedDirection == AutoSearchDirection::Right){
			m_drive->PIDTurn(13.0, Drive::RelativeTo::Now);
			m_autoState ++;
		}
		else if (m_selectedDirection == AutoSearchDirection::NoVision){
			m_autoState += 100;
		}
		m_autoTimer = GetMsecTime();
		m_poseManager->ChooseNthPose(PoseManager::NEAR_DEFENSE_SHOT_POSE);
		m_arm->SetPower(-0.2);
		m_shooter->SetFlywheelEnabled(true);
		break;
	case 3:
		if (m_drive->OnTarget() || GetMsecTime() - m_autoTimer > 2000){
			m_autoState ++;
		}
		break;
	case 4:
		//m_drive->SetVisionTargeting();
		m_autoState++;
		m_autoTimer = GetMsecTime();
		break;
	case 5:
		if (GetMsecTime() - m_autoTimer > 2500 &&
				(m_drive->OnTarget() || GetMsecTime() - m_autoTimer > 4000)){
			m_autoState ++;
		}
		break;
	case 6:
		m_arm->SetPower(0.0);
		m_shooter->SetConveyerPower(1.0);
		m_autoState++;
		break;
	default:
		m_arm->SetPower(0.0);
		break;
	}
}


void Robot::DrawbridgeAuto() {
	switch (m_autoState) {
		case 0:
			m_drive->PIDDrive(72.0, Drive::RelativeTo::Now);
			m_arm->SetTargetPosition(45.0);
			m_autoState ++;
			break;
		case 1:
			if (m_drive->OnTarget()){
				m_autoState ++;
			}
			break;
		case 2:
			m_arm->SetTargetPosition(0.0);
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
			if (m_selectedDirection == AutoSearchDirection::None){
				m_autoState += 2;
			}
			else if (m_selectedDirection == AutoSearchDirection::Left){
				m_drive->PIDTurn(-25.0, Drive::RelativeTo::Now);
				m_autoState ++;
			}
			else if (m_selectedDirection == AutoSearchDirection::Right){
				m_drive->PIDTurn(25.0, Drive::RelativeTo::Now);
				m_autoState ++;
			}
			else if (m_selectedDirection == AutoSearchDirection::NoVision){
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
			m_arm->SetTargetPosition(45.0);
			m_autoState ++;
			break;
		case 1:
			if (m_drive->OnTarget()){
				m_autoState ++;
			}
			break;
		case 2:
			m_arm->SetTargetPosition(0.0);
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
			if (m_selectedDirection == AutoSearchDirection::None){
				m_autoState += 2;
			}
			else if (m_selectedDirection == AutoSearchDirection::Left){
				m_drive->PIDTurn(-25.0, Drive::RelativeTo::Now);
				m_autoState ++;
			}
			else if (m_selectedDirection == AutoSearchDirection::Right){
				m_drive->PIDTurn(25.0, Drive::RelativeTo::Now);
				m_autoState ++;
			}
			else if (m_selectedDirection == AutoSearchDirection::NoVision){
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

void Robot::VisionPortion (){
	switch(m_autoState){
	case 0:
		if (m_selectedDirection == AutoSearchDirection::None){
			m_autoState += 2;
		}
		else if (m_selectedDirection == AutoSearchDirection::Left){
			m_drive->PIDTurn(-5.0, Drive::RelativeTo::Now);
			m_autoState ++;
		}
		else if (m_selectedDirection == AutoSearchDirection::Right){
			m_drive->PIDTurn(13.0, Drive::RelativeTo::Now);
			m_autoState ++;
		}
		else if (m_selectedDirection == AutoSearchDirection::NoVision){
			m_autoState += 100;
		}
		m_autoTimer = GetMsecTime();
		m_poseManager->ChooseNthPose(PoseManager::NEAR_DEFENSE_SHOT_POSE);
		m_arm->SetPower(-0.2);
		m_shooter->SetFlywheelEnabled(true);
		break;
	case 1:
		if (m_drive->OnTarget() || GetMsecTime() - m_autoTimer > 2000){
			m_autoState ++;
		}
		break;
	case 2:
		//m_drive->SetVisionTargeting();
		m_autoState++;
		m_autoTimer = GetMsecTime();
		break;
	case 3:
		if (GetMsecTime() - m_autoTimer > 2500 &&
				(m_drive->OnTarget() || GetMsecTime() - m_autoTimer > 4000)){
			m_autoState ++;
		}
		break;
	case 4:
		m_arm->SetPower(0.0);
		m_shooter->SetConveyerPower(1.0);
		m_autoState++;
		break;
	default:
		m_arm->SetPower(0.0);
		break;
	}
}

}
