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
	RobotStateInterface(frc973::RobotMode mode) : m_mode(mode) {
		;
	}

	RobotStateInterface() : m_mode(frc973::RobotMode::MODE_DISABLED) {
		;
	}

	virtual ~RobotStateInterface() {}

	void SetRobotMode(frc973::RobotMode mode) {
		m_mode = mode;
	}

	bool IsDisabled() {
		return m_mode == frc973::RobotMode::MODE_DISABLED;
	}

	bool IsEnabled() {
		return m_mode != frc973::RobotMode::MODE_DISABLED;
	}

	bool IsOperatorControl() {
		return m_mode == frc973::RobotMode::MODE_TELEOP;
	}

	bool IsAutonomous() {
		return m_mode == frc973::RobotMode::MODE_AUTO;
	}

	bool IsTest() {
		return m_mode == frc973::RobotMode::MODE_TEST;
	}

private:
	frc973::RobotMode m_mode;
};


/*
 * Mock DriverStation for unit tests.  Simulates 5 joysticks and battery
 * voltage.
 *
 * GetInstance() returns the most recently created DriverStation.  I made
 * it that way for now but long term this is very bad form.  Avoid using
 * DriverStation::GetInstance() in code, prefer passing pointers to it, and
 * if possible, prefer to use wpilib classes that don't call DriverStation::
 * GetInstance().
 */
class DriverStation : public RobotStateInterface {
	static DriverStation *s_instance;
public:
	static constexpr int NUM_CHANS = 5;
	static constexpr int NUM_AXIS = 6;

	DriverStation();
	virtual ~DriverStation();

	void SetBatteryVoltage_T(float voltage);
	float GetBatteryVoltage();

	static DriverStation &GetInstance();

	uint32_t GetStickButtons(int chan);

	void SetStickButtons_T(int chan, uint32_t btns);

	float GetStickAxis(int chan, int axis);

	void SetStickAxis_T(int chan, int axis, float val);
private:
	struct JoystickState {
		uint32_t buttons;
		float axises[NUM_AXIS];
	};

	float m_batteryVoltage;
	JoystickState m_joyStates[NUM_CHANS];
};

#endif /* MOCKWPILIB_MOCKDRIVERSTATION_H_ */
