/*
 * VisionDriveController.h
 *
 *  Created on: Feb 25, 2016
 *      Author: Andrew
 */

#ifndef SRC_CONTROLLERS_VISIONDRIVECONTROLLER_H_
#define SRC_CONTROLLERS_VISIONDRIVECONTROLLER_H_
#include "lib/filters/RampedOutput.h"
#include "lib/DriveBase.h"
#include "lib/SocketServer.hpp"

namespace frc973 {

class PID;
class Debouncer;

class VisionDriveController
		 : public DriveController
		 , public SocketServer::SocketListener {
public:
	VisionDriveController();
	virtual ~VisionDriveController();
	/*
	 * Calculate motor output given the most recent joystick commands.
	 * In this case just return the most recent joystick commands.
	 */
	void CalcDriveOutput(DriveStateProvider *state,
			DriveControlSignalReceiver *out);

	/*
	 * We are on target if:
	 *  - we see the target and
	 *  - we're stopped (not currently turning) and
	 *  - we're within a degree of the target
	 */
	bool OnTarget() {
		return
				m_targetFound &&
				Util::abs(m_prevAngleVel) < 1.0 &&
				Util::abs(m_targetAngle - m_prevAngle) < 1.0;
	}

	void Start() {
		//m_targetAngle = 90.0 + m_prevAngle;
	}

	void OnValueChange(std::string name, std::string newValue) override;
private:
	double m_leftOutput;
	double m_rightOutput;

	double m_targetAngle;
	double m_prevAngle;
	double m_prevAngleVel;

	PID *m_anglePid;
	bool m_onTarget;
	bool m_targetFound;

	bool m_readyForFrame;
	Debouncer *m_readyFilter;
	RampedOutput *m_linprof;
};

} /* namespace frc973 */

#endif /* SRC_CONTROLLERS_VISIONDRIVECONTROLLER_H_ */
