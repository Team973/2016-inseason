/*
 * VisionDriveController.h
 *
 *  Created on: Feb 25, 2016
 *      Author: Andrew
 */

#ifndef SRC_CONTROLLERS_VISIONDRIVECONTROLLER_H_
#define SRC_CONTROLLERS_VISIONDRIVECONTROLLER_H_
#include "lib/DriveBase.h"

#include "VisionTask.h"
#include "stdio.h"

namespace frc973 {

class PID;
class Debouncer;

class VisionDriveController
		 : public DriveController
		 , public VisionDataReceiver
		   {
public:
	enum VisionState {
		WAITING,
		TURNING,
		FAILING
	};

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
	 *  - we couldn't find a target (something went wrong) so give up
	 *  - we're stopped (not currently turning) and
	 *  - we're within a degree of the target
	 */
	bool OnTarget() {
		return m_state == FAILING ||
				(m_state == TURNING &&
				 Util::abs(m_prevAngleVel) < 0.5 &&
				 Util::abs(m_targetAngle - m_prevAngle) < 0.4);
	}

	void Start() override {
		m_state = WAITING;
		m_visionTask->GrabOffset(this);
		printf("turning on vision mode\n");
	}

	void Stop() override {
		m_visionTask->KillTask();
		printf("turning off vision mode\n");
	}

	void VisionReceiveDistance(double distance) override{
		;
	}

	void VisionReceiveXAngle(double angle) override;

	void VisionReceiveFailure() override {
		printf("Received signal for vision failure\n");
		m_state = FAILING;
	}
private:
	double m_leftOutput;
	double m_rightOutput;

	double m_targetAngle;
	double m_prevAngle;
	double m_prevAngleVel;

	PID *m_anglePid;

	VisionState m_state;
	VisionTask *m_visionTask;
};

} /* namespace frc973 */

#endif /* SRC_CONTROLLERS_VISIONDRIVECONTROLLER_H_ */
