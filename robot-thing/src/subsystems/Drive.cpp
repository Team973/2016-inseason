#include <stdio.h>

#include "RobotInfo.h"

#include "subsystems/Drive.h"
#include "WPILib.h"
#include "lib/util/Util.h"
#include "lib/SPIGyro.h"

#include "lib/filters/RampedOutput.h"

#include "lib/logging/LogSpreadsheet.h"

#include "src/controllers/ArcadeDriveController.h"
#include "src/controllers/CheesyDriveController.h"
#include "src/controllers/PIDDriveController.h"
#include "src/controllers/RampedPIDDriveController.h"
#include "src/controllers/VisionDriveController.h"

namespace frc973 {

Drive::Drive(TaskMgr *scheduler, VictorSP *left, VictorSP *right,
			Encoder *leftEncoder, Encoder *rightEncoder,
#ifdef PROTO_BOT_PINOUT
			Encoder *gyro,
#else
			SPIGyro *gyro,
#endif
			LogSpreadsheet *logger
			)
		 : DriveBase(scheduler, this, this, nullptr)
		 , m_leftEncoder(leftEncoder)
		 , m_rightEncoder(rightEncoder)
		 , m_gyro(gyro)
		 , m_gearing(DriveGearing::LowGear)
		 , m_gearingSolenoid(new Solenoid(DRIVE_SHIFT_SOL))
		 , m_leftPower(0.0)
		 , m_rightPower(0.0)
		 , m_leftMotor(left)
		 , m_rightMotor(right)
		 , m_leftMotorPowerFilter(dynamic_cast<FilterBase*>(new RampedOutput(10.0)))
		 , m_rightMotorPowerFilter(dynamic_cast<FilterBase*>(new RampedOutput(10.0)))
		 , m_arcadeDriveController(new ArcadeDriveController())
		 , m_cheesyDriveController(new CheesyDriveController())
		 , m_pidDriveController(new PIDDriveController())
		 , m_rampPidDriveController(new RampPIDDriveController())
		 , m_visionDriveController(new VisionDriveController())
		 , m_brakes(new DoubleSolenoid(DRIVE_BREAK_SOL_A, DRIVE_BREAK_SOL_B))
		 , m_spreadsheet(logger)
		 , m_angleLog(new LogCell("Angle"))
		 , m_angularRateLog(new LogCell("Angular Rate"))
		 , m_leftDistLog(new LogCell("Left Encoder Distance"))
		 , m_leftDistRateLog(new LogCell("Left Encoder Rate"))
		 , m_leftPowerLog(new LogCell("Left motor power"))
		 , m_rightPowerLog(new LogCell("Right motor power"))
{
	printf("Initializing Drive Subsystem\n");
	m_leftEncoder->SetDistancePerPulse(1.0);
	this->SetDriveController(m_arcadeDriveController);

	bool loggingEnabled = true;
	if (loggingEnabled) {
		m_spreadsheet->RegisterCell(m_angleLog);
		m_spreadsheet->RegisterCell(m_angularRateLog);
		m_spreadsheet->RegisterCell(m_leftDistLog);
		m_spreadsheet->RegisterCell(m_leftDistRateLog);
		m_spreadsheet->RegisterCell(m_leftPowerLog);
		m_spreadsheet->RegisterCell(m_rightPowerLog);
	}

	scheduler->RegisterTask("DriveBase", this, TASK_PERIODIC);
}

void Drive::SetGearing(DriveGearing newGearing) {
	if (newGearing != m_gearing) {
		switch (newGearing) {
		case DriveGearing::HighGear:
			m_gearingSolenoid->Set(true);
			break;
		case DriveGearing::LowGear:
			m_gearingSolenoid->Set(false);
		}
		m_gearing = newGearing;
	}
}

void Drive::Zero() {
	m_pidDriveController->Zero();
	if (m_leftEncoder)
		m_leftEncoder->Reset();
	if (m_rightEncoder)
		m_rightEncoder->Reset();
	if (m_gyro)
		m_gyro->Reset();
	m_leftEncoder->SetDistancePerPulse(1.0);
}

void Drive::CheesyDrive(double throttle, double turn) {
	this->SetDriveController(m_cheesyDriveController);
	m_cheesyDriveController->SetJoysticks(throttle, turn);
}

void Drive::SetCheesyQuickTurn(bool quickturn) {
	m_cheesyDriveController->SetQuickTurn(quickturn);
}

void Drive::SetVisionTargeting() {
	this->SetDriveController(m_visionDriveController);
}

void Drive::ArcadeDrive(double throttle, double turn) {
	this->SetDriveController(m_arcadeDriveController);
	m_arcadeDriveController->SetJoysticks(throttle, turn);
}

void Drive::PIDDrive(double distance, RelativeTo relativity) {
	this->SetDriveController(m_pidDriveController);
	m_pidDriveController->EnableDist();
	m_pidDriveController->SetTarget(distance, 0.0, relativity, this);
}

void Drive::PIDTurn(double degrees, RelativeTo relativity) {
	this->SetDriveController(m_pidDriveController);
	m_pidDriveController->DisableDist();
	m_pidDriveController->SetTarget(0.0, degrees, relativity, this);
}

void Drive::RampPIDDrive(double distance, RelativeTo relativity) {
	this->SetDriveController(m_rampPidDriveController);
	m_rampPidDriveController->EnableDist();
	m_rampPidDriveController->SetTarget(distance, 0.0);
}

void Drive::RampPIDTurn(double angle, RelativeTo relativity) {
	this->SetDriveController(m_rampPidDriveController);
	m_rampPidDriveController->DisableDist();
	m_rampPidDriveController->SetTarget(0.0, angle);
}

double Drive::GetLeftDist() {
	return -m_leftEncoder->Get() * 24.5 / 360.0 * 0.95;
}

double Drive::GetRightDist() {
	printf("Someone didn't get the memo this robot only has one encoder\n");
	return -GetLeftDist();
}

double Drive::GetLeftRate() {
	return m_leftEncoder->GetRate();
}

double Drive::GetRightRate() {
	printf("someone didn't get the memo this robot only has one encoder\n");
	return GetLeftRate();
}

double Drive::GetDist() {
	return GetLeftDist();
}

double Drive::GetRate() {
	return GetLeftRate();
}

double Drive::GetAngle() {
#ifdef PROTO_BOT_PINOUT
	return -m_gyro->Get();
#else
	return -m_gyro->GetDegrees();
#endif
}

double Drive::GetAngularRate() {
#ifdef PROTO_BOT_PINOUT
	return -m_gyro->GetRate();
#else
	return -m_gyro->GetDegreesPerSec();
#endif
}

void Drive::SetDriveOutput(double left, double right) {
	m_leftPower = left;
	m_rightPower = right;

	if (isnan(m_leftPower) || isnan(m_rightPower)) {
		m_leftMotor->Set(0.0);
		m_rightMotor->Set(0.0);
	}
	else {
		m_leftMotor->Set(
				m_leftMotorPowerFilter->Update(
						Util::bound(-m_leftPower, -1.0, 1.0)));
		m_rightMotor->Set(
				m_rightMotorPowerFilter->Update(
						Util::bound(-m_rightPower, -1.0, 1.0)));
	}
}

void Drive::SetBraking(bool enabledP) {
	if (enabledP) {
		m_brakes->Set(DoubleSolenoid::kForward);
	}
	else {
		m_brakes->Set(DoubleSolenoid::kReverse);
	}
}

void Drive::TaskPeriodic(RobotMode mode) {
	m_angleLog->LogDouble(this->GetAngle());
	m_angularRateLog->LogDouble(this->GetAngularRate());
	m_leftDistLog->LogDouble(this->GetLeftDist());
	m_leftDistRateLog->LogDouble(this->GetLeftRate());
	m_leftPowerLog->LogDouble(m_leftPower);
	m_rightPowerLog->LogDouble(m_rightPower);
}

}
