/*
 * MockShooter.h
 *
 *  Created on: Feb 27, 2016
 *      Author: andrew
 */

#ifndef SRC_MOCKSHOOTER_H_
#define SRC_MOCKSHOOTER_H_

/**
 * Mock shooter that has everything public and dead simple for test purposes
 */
namespace frc973 {

class Shooter {
public:
	enum FlywheelState {
		openLoop,
		ssControl
	};

	enum ElevatorHeight {
		wayHigh,
		midHigh,
		midLow,
		wayLow
	};

	FlywheelState m_frontFlywheelState;
	FlywheelState m_backFlywheelState;

	double m_frontFlywheelTargetSpeed;
	double m_backFlywheelTargetSpeed;
	double m_frontFlywheelSetPower;
	double m_backFlywheelSetPower;

	ElevatorHeight m_elevatorState;

	Shooter(): m_frontFlywheelState(FlywheelState::openLoop),
			m_backFlywheelState(FlywheelState::openLoop),
			m_frontFlywheelTargetSpeed(0.0),
			m_backFlywheelTargetSpeed(0.0),
			m_frontFlywheelSetPower(0.0),
			m_backFlywheelSetPower(0.0),
			m_elevatorState(ElevatorHeight::wayLow) {}
	virtual ~Shooter() {}

	void SetFrontFlywheelSSShoot(double goal) {
		m_frontFlywheelState = FlywheelState::ssControl;
		m_frontFlywheelTargetSpeed = goal;
	}
	void SetBackFlywheelSSShoot(double goal) {
		m_backFlywheelState = FlywheelState::ssControl;
		m_backFlywheelTargetSpeed = goal;
	}

	void SetFlywheelSSShoot(double goal) {
		SetFrontFlywheelSSShoot(goal);
		SetBackFlywheelSSShoot(goal);
	}

	void SetFrontFlywheelPower(double pow) {
		m_frontFlywheelState = FlywheelState::openLoop;
		m_frontFlywheelSetPower = pow;
	}
	void SetBackFlywheelPower(double pow) {
		m_backFlywheelState = FlywheelState::openLoop;
		m_backFlywheelSetPower = pow;
	}

	void SetFlywheelPower(double pow) {
		SetFrontFlywheelPower(pow);
		SetBackFlywheelPower(pow);
	}

	void SetFlywheelStop() {
		SetFlywheelPower(0.0);
	}

	void SetElevatorHeight(ElevatorHeight newHeight) {
		m_elevatorState = newHeight;
	}
};

}

#endif /* SRC_MOCKSHOOTER_H_ */
