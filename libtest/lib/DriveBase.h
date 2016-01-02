/*
 * DriveBase.h
 *
 *  Created on: Oct 29, 2015
 *      Author: Andrew
 */

#ifndef LIB_DRIVEBASE_H_
#define LIB_DRIVEBASE_H_

#include "lib/TaskMgr.h"
#include "lib/CoopTask.h"
#include "WPILib.h"

/*
 * Interface for a class can determine the angle of the robot
*/
class AngleProvider {
public:
	AngleProvider() {}
	virtual ~AngleProvider() {}
	virtual double GetAngle() = 0;
	virtual double GetAngularRate() = 0;
};

/*
 * Interface for a class that can determine the distance of the robot
*/
class DistProvider {
public:
	DistProvider() {}
	virtual ~DistProvider() {}
	virtual double GetLeftDist() = 0;
	virtual double GetRightDist() = 0;
	virtual double GetLeftRate() = 0;
	virtual double GetRightRate() = 0;
	virtual double GetDist() = 0;
	virtual double GetRate() = 0;
};

/*
 * Interface for a class that can take drive output
*/
class DriveOutput {
public:
	DriveOutput() {}
	virtual ~DriveOutput() {}
	virtual void SetDriveOutput(double left, double right) = 0;
};

/*
 * Interface for anything that can use an angle provider, a dist provider, and
 * can use those to send some drive output to the DriveOutput
 *
 * In general there will be two drive controllers: One that calculates output
 * based solely on joystick values and one that calcualtes output based solely
 * on pid.
*/
class DriveController {
public:
	DriveController() {}
	virtual ~DriveController() {}
	virtual void CalcDriveOutput(AngleProvider *angle, DistProvider *dist, DriveOutput *out) = 0;
	virtual bool OnTarget() = 0;
};

/**
 * (abstract) Base class for a robot drive.  DriveBase keeps track of one
 * DriveController and uses it to calculate drive output, then drives the
 * motors with those calculated values.
 *
 * CoopTask handles calling TaskPostPeriodic once a cycle
 * DriveOutput interface defines SetDriveOutput for the DriveController to use
 *
 */
class DriveBase :
		public CoopTask,
		public DriveOutput
{
public:
	/**
	 * Creates a new DriveBase Object.  The DriveBase object stores a drive
	 * controller (an object capable of calculating motor outputs) and uses it
	 * to calculate drive outputs, then drive those drive outputs.
	 */
	DriveBase(TaskMgr *scheduler, VictorSP *leftMotor, VictorSP *rightMotor,
			AngleProvider *angle, DistProvider *dist,
			DriveController *controller = nullptr);
	virtual ~DriveBase();

	/*
	 * Get input from the currently active DriveController.  This method comes
	 * from CoopTask and is called automatically once a cycle by m_scheduler
	 *
	 * @param mode The current operating mode of the robot
	 */
	void TaskPostPeriodic(RobotMode mode);

	/*
	 * Used by the DriveController to set motor values
	 *
	 * @param left power (from -1.0 to 1.0) for left motor
	 * @param right power (from -1.0 to 1.0) for right motor
	 */
	void SetDriveOutput(double left, double right);

	/*
	 * Change the DriveController currently active
	 *
	 * @param new controller to have active
	 */
	void SetDriveController(DriveController *controller);

	/*
	 * Checks with the current controller to see if we are on target.  If
	 * there is no controller currently selected, just return false
	 *
	 * @return whether the current controller things we are done
	 */
	bool OnTarget();
private:
	TaskMgr *m_scheduler;

	double m_leftPower;
	double m_rightPower;

	VictorSP *m_leftMotor;
	VictorSP *m_rightMotor;

	AngleProvider *m_angleProvider;
	DistProvider *m_distProvider;

	DriveController *m_controller;
};

#endif /* LIB_DRIVEBASE_H_ */
