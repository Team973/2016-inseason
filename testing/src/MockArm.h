/*
 * MockArm.h
 *
 *  Created on: Feb 27, 2016
 *      Author: andrew
 */

#ifndef SRC_MOCKARM_H_
#define SRC_MOCKARM_H_

namespace frc973 {

class Arm {
public:
	enum ControlMode {
		power,
		position,
		velocity
	};

	ControlMode mode;
	double m_power;
	double m_position;

	Arm() : mode(ControlMode::power),
			m_power(0.0), m_position(0.0) {}
	~Arm() {}

	void SetTargetPosition(double goal) {
		m_position = goal;
		mode = ControlMode::position;
	}

	void SetPower(double pow) {
		m_power = pow;
		mode = ControlMode::power;
	}
};
}

#endif /* SRC_MOCKARM_H_ */
