/*
 * VisionDriveController.h
 *
 *  Created on: Feb 25, 2016
 *      Author: Andrew
 */

#ifndef SRC_CONTROLLERS_VISIONDRIVECONTROLLER_H_
#define SRC_CONTROLLERS_VISIONDRIVECONTROLLER_H_

#include "lib/DriveBase.h"
#include "lib/SocketClient.h"

namespace frc973 {

class PID;

class VisionDriveController
		 : public DriveController
		 , public SocketClient::SocketListener {
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
	 * This controller is open-loop so OnTarget doesn't make sense here...
	 * just return false I guess...
	 */
	bool OnTarget() {
		return Util::abs(m_targetAngle - m_prevAngle) < 1.0;
	}

	void OnValueChange(std::string name, std::string newValue) override;
private:
	double m_leftOutput;
	double m_rightOutput;

	double m_targetAngle;
	double m_prevAngle;

	PID *m_anglePid;
	bool m_onTarget;
	bool m_targetFound;
};

} /* namespace frc973 */

#endif /* SRC_CONTROLLERS_VISIONDRIVECONTROLLER_H_ */
