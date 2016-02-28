/*
 * PoseManager.cpp
 *
 *  Created on: Feb 26, 2016
 *      Author: andrew
 */

#include <PoseManager.h>
#include <fstream>
#include "subsystems/Arm.h"
#include "subsystems/Shooter.h"
#include "subsystems/Intake.h"

#include <iostream>
namespace frc973 {

PoseManager::PoseManager(Arm *arm, Shooter *shooter, Intake *intake)
		 : m_arm(arm)
		 , m_shooter(shooter)
		 , m_intake(intake)
		 , m_currPose(0) {
	ReloadConfiguration();
}

PoseManager::~PoseManager() {
	// TODO Auto-generated destructor stub
}

void PoseManager::ReloadConfiguration() {
	std::ifstream fileStream;// ("~/presets.json", std::ifstream::in);
	fileStream.open("presets.json");
	Json::Reader reader;

	if (fileStream) {
		reader.parse(fileStream, m_configRoot);
		printf("parsed config file correcly!\n");
	}
	else {
		printf("No file found!\n");
	}
}

void PoseManager::NextPose() {
	m_currPose = (m_currPose + 1) % m_configRoot["poses"].size();

	printf("Pose now selected: %s\n",
			m_configRoot["poses"][m_currPose]["name"].asCString());

	/*
	 * You might want to print the currently selected pose on the dash...
	 * idk which line is open, though...
	DBStringPrintf(DBStringPos::DB_LINE4,
			"curr Pose%s", m_configRoot["poses"][m_currPose]["name"].asCString());
	 */
}

void PoseManager::AssumePose() {
	Json::Value pose = m_configRoot["poses"][m_currPose];
	std::cout << "there are this many " << m_configRoot["poses"].size();
	std::cout << "my name is " << pose["name"];

	if (pose["armControl"].asBool()) {
		m_arm->SetTargetPosition(pose["armTarget"].asDouble());
	}
	else {
		m_arm->SetPower(0.0);
	}

	if (pose["intakeExtended"].asBool()) {
		m_intake->SetIntakePosition(Intake::IntakePosition::extended);
	}
	else {
		m_intake->SetIntakePosition(Intake::IntakePosition::retracted);
	}

	char shooterHeight =
			(pose["longCylinderExtended"].asBool() << 1) |
			pose["shortCylinderExtended"].asBool();
	switch(shooterHeight) {
	case 0x00:
		m_shooter->SetElevatorHeight(Shooter::ElevatorHeight::wayLow);
		break;
	case 0x01:
		m_shooter->SetElevatorHeight(Shooter::ElevatorHeight::midLow);
		break;
	case 0x02:
		m_shooter->SetElevatorHeight(Shooter::ElevatorHeight::midHigh);
		break;
	case 0x03:
		m_shooter->SetElevatorHeight(Shooter::ElevatorHeight::wayHigh);
		break;
	}

	std::cout << "observing %s\n" << pose["superiorFlywheelControl"].asString();
	if (pose["superiorFlywheelControl"].asString() == "closed") {
		m_shooter->SetFrontFlywheelSSShoot(pose["superiorFlywheelRPM"].asDouble());
		printf("oui oui!\n");
	}
	else {
		m_shooter->SetFrontFlywheelPower(pose["superiorFlywheelPower"].asDouble());
		printf("nont\n");
	}

	if (pose["inferiorFlywheelControl"].asString() == "closed") {
		m_shooter->SetBackFlywheelSSShoot(pose["inferiorFlywheelRPM"].asDouble());
	}
	else {
		m_shooter->SetBackFlywheelPower(pose["inferiorFlywheelPower"].asDouble());
	}
}

void PoseManager::Chill() {
	m_arm->SetPower(0.0);
	m_shooter->SetFlywheelPower(0.0);
}

} /* namespace frc973 */
