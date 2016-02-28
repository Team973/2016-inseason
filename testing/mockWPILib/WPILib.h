/*
 * WPILib.h
 *
 *  Created on: Jan 19, 2016
 *      Author: andrew
 */

#pragma once
#ifndef WPILIB_H_
#define WPILIB_H_

#include <stdint.h>
#include <string>
#include <cstring>

#include "MockTime.h"

class RobotStateInterface;
class VictorSP;

class SensorBase {
public:
	SensorBase() {}
	~SensorBase() {}
};

uint64_t GetFPGATime();

#define START_ROBOT_CLASS(x)  ;

#include "MockDriverStation.h"

#include "MockCounter.h"
#include "MockEncoder.h"

#include "MockSpeedController.h"
#include "MockVictorSP.h"

#include "MockSolenoidBase.h"
#include "MockSolenoid.h"

class IterativeRobot {

};

class Joystick {
public:
	Joystick(int chan) : m_chan(chan) {}
	virtual ~Joystick() {}
	bool GetRawButton(int btn) { return false; }
	double GetRawAxis(int axis) { return DriverStation::GetInstance().GetStickAxis(m_chan, axis); }
	int GetPOV() { return 0; }
private:
	int m_chan;
};

class Relay {
public:
	enum Value {kOff, kOn, kForward, kReverse};
	enum Direction {kBothDirections, kForwardOnly, kReverseOnly};

	Relay(int chan, Direction dir) {}
	virtual ~Relay() {}

	void Set(Value state) {}
};

class Accelerometer {
public:
	Accelerometer() {}
	virtual ~Accelerometer() {}
	double GetX() { return 0.0; }
	double GetY() { return 0.0; }
	double GetZ() { return 0.0; }

	enum Range {
		kRange_2G,
		kRange_4G,
		kRange_8G,
		kRange_16G
	};
};

class BuiltInAccelerometer : public Accelerometer {
public:
	BuiltInAccelerometer(Range range) {}
	virtual ~BuiltInAccelerometer() {}
};

class SmartDashboard {
public:
	static void init() {}

	static void PutString(std::string pos, std::string msg) {}
	static void PutNumber(std::string pos, double number) {}
};

class DigitalInput {
public:
	DigitalInput(int chan) {}
	virtual ~DigitalInput() {}

	bool Get() { return false; }
};

class SPI {
public:
	enum Port {
		kOnboardCS0, kOnboardCS1, kOnboardCS2, kOnboardCS3, kMXP
	};

	SPI(Port SPIPort) {}
	virtual ~SPI() {}

	void SetMSBFirst() {}
	int Transaction(void *to_send, void *to_receive, int numBytes) { return 0; }
	void SetClockActiveHigh() {}
	void SetSampleDataOnRising() {}
	void SetClockRate(int something) {}
	void SetChipSelectActiveLow() {}
};

class Timer {
	double m_startTime;
public:
	Timer() : m_startTime(0.0) {}
	virtual ~Timer() {}

	void Start() { m_startTime = MockTime::GetSecTime(); }
	double Get() { return MockTime::GetSecTime() - m_startTime; }
	void Reset() { Start(); }
};

class PowerDistributionPanel {
public:
	PowerDistributionPanel() {}
	virtual ~PowerDistributionPanel() {}
};

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
int gethostname(char *name, size_t len);
#endif

#endif /* WPILIB_H_ */
