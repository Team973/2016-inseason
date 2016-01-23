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
	Joystick(int chan) {}
	virtual ~Joystick() {}
	bool GetRawButton(int btn) { return false; }
	double GetRawAxis(int axis) { return 0.0; }
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
};

class DigitalInput {
public:
	DigitalInput(int chan) {}
	virtual ~DigitalInput() {}

	bool Get() { return false; }
};
#endif /* WPILIB_H_ */
