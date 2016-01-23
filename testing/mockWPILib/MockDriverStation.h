/*
 * MockDriverStation.h
 *
 *  Created on: Jan 20, 2016
 *      Author: andrew
 */

#ifndef MOCKWPILIB_MOCKDRIVERSTATION_H_
#define MOCKWPILIB_MOCKDRIVERSTATION_H_

#include "lib/util/Util.h"

class RobotStateInterface {
public:
	RobotStateInterface(RobotMode mode) : m_mode(mode) {
		;
	}

	RobotStateInterface() : m_mode(RobotMode::MODE_DISABLED) {
		;
	}

	virtual ~RobotStateInterface() {}

	void SetRobotMode(RobotMode mode) {
		m_mode = mode;
	}

	bool IsDisabled() {
		return m_mode == RobotMode::MODE_DISABLED;
	}

	bool IsEnabled() {
		return m_mode != RobotMode::MODE_DISABLED;
	}

	bool IsOperatorControl() {
		return m_mode == RobotMode::MODE_TELEOP;
	}

	bool IsAutonomous() {
		return m_mode == RobotMode::MODE_AUTO;
	}

	bool IsTest() {
		return m_mode == RobotMode::MODE_TEST;
	}

private:
	RobotMode m_mode;
};

class DriverStation : public RobotStateInterface {
public:
	static DriverStation *m_instance;
	DriverStation();
	virtual ~DriverStation();

	void SetBatteryVoltage(float voltage);
	float GetBatteryVoltage();

	static DriverStation &GetInstance();

	uint32_t GetStickButtons(int chan) {
		return 0;
	}

private:
	float m_batteryVoltage;
};

#endif /* MOCKWPILIB_MOCKDRIVERSTATION_H_ */
