/*
 * PIDDriveController.h
 *
 *  Created on: Nov 5, 2015
 *      Author: Andrew
 */

#ifndef SRC_CONTROLLERS_PIDDRIVECONTROLLER_H_
#define SRC_CONTROLLERS_PIDDRIVECONTROLLER_H_


#include "lib/DriveBase.h"

class PID;

class PIDDriveController : public DriveController {
public:
	PIDDriveController();
	virtual ~PIDDriveController() {}

	/*
	 * Calculate the motor output to achieve the most recently set setpoint.
	 * This reads in position data from the angle and dist providers and uses
	 * the pid object to decide on an ideal set of outputs
	 */
	void CalcDriveOutput(AngleProvider *angle, DistProvider *dist,
			DriveOutput *out);

	/*
	 * On CalcDriveOutput, the robot sets the internal m_onTarget flag if it
	 * is within tolerance of the target.  This method returns whether we are
	 * on target.
	 */
	bool OnTarget() { return m_onTarget; }

	/*
	 * Set the target position/heading relative to current position/heading
	 */
	void SetTarget(double throttle, double turn);

	/*
	 * Enable distance pid, do angle and dist together.
	 */
	void EnableDist() {
		m_distEnabled = true;
	}

	/*
	 * Disable distance pid, do angle only.
	 *
	 * This is a cheap hack because we only have an encoder on left drive...
	 * if we had left and right we could actually determine our forward-ness
	 */
	void DisableDist() {
		m_distEnabled = false;
	}

	void Zero() {
		m_prevDist = 0.0;
		m_prevAngle = 0.0;
		m_targetDist = 0.0;
		m_targetAngle = 0.0;
		m_onTarget = false;
	}
private:
	double m_prevDist;
	double m_prevAngle;

	double m_targetDist;
	double m_targetAngle;

	bool m_onTarget;

	PID *m_drivePID;
	PID *m_turnPID;

	bool m_distEnabled;
};


#endif /* SRC_CONTROLLERS_PIDDRIVECONTROLLER_H_ */
