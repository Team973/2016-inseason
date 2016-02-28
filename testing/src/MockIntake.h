/*
 * MockIntake.h
 *
 *  Created on: Feb 27, 2016
 *      Author: andrew
 */

#ifndef SRC_MOCKINTAKE_H_
#define SRC_MOCKINTAKE_H_

namespace frc973 {

class Intake {
public:
	enum IntakePosition {
		extended,
		retracted
	};
	IntakePosition position;

	Intake() : position(IntakePosition::retracted) {}
	~Intake() {}

	void SetIntakePosition(IntakePosition newPos) {
		position = newPos;
	}
};

}

#endif /* SRC_MOCKINTAKE_H_ */
