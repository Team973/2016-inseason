/*
 * PixyVisionDriveController.h
 *
 *  Created on: Apr 10, 2016
 *      Author: Kyle Andrei
 */

#ifndef SRC_CONTROLLERS_PIXYVISIONDRIVECONTROLLER_H_
#define SRC_CONTROLLERS_PIXYVISIONDRIVECONTROLLER_H_

#include "lib/DriveBase.h"

class AnalogInput;
class DigitalInput;

namespace frc973 {

class PID;

class PixyVisionDriveController: public DriveController{
public:
	PixyVisionDriveController();
	virtual ~PixyVisionDriveController();

	void CalcDriveOutput(DriveStateProvider *state,
				DriveControlSignalReceiver *out);

		/*
		 * We are on target if:
		 *  - we couldn't find a target (something went wrong) so give up
		 *  - we're stopped (not currently turning) and
		 *  - we're within a degree of the target
		 */
	bool OnTarget() {
		return Util::abs(m_prevAngleVel) < 0.25 &&
				Util::abs(m_targetAnglePos - m_prevAnglePos) < 0.25;
}

private:
	static constexpr double MAX_VELOCITY = 20.0;

	double m_leftOutput;
	double m_rightOutput;

	bool m_onTarget;

	double m_prevAngleVel;
	double m_prevAnglePos;
	double m_targetAngleVel;
	double m_targetAnglePos;

	PID *m_velPid;
	PID *m_posPid;

	AnalogInput *m_offsetInput;
	DigitalInput *m_targetFoundInput;
};


} /* namespace frc973 */

#endif /* SRC_CONTROLLERS_PIXYVISIONDRIVECONTROLLER_H_ */
