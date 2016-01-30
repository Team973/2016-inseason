#ifndef DRIVE_H
#define DRIVE_H

#include "lib/DriveBase.h"

class ArcadeDriveController;
class CheesyDriveController;
class PIDDriveController;

/*
 * Drive provides an interface to control the drive-base (to do both
 * teleoperated and autonomous movements).  To do this, it makes
 * a bunch of DriveControllers (autonomous pid, autonomous trap,
 * teleop arcade, maybe someday a state space drive controller).  When
 * a command is issued (one of these routines is called), Drive determines
 * which controller is best suited to service that command and makes it
 * the "active" controller.
 *
 *  * DriveBase... calls on the active controller to calculate motor output
 *  * DriveStateProvider... provides the controller with position/angle/speed etc
 *  * DrivecontrolSignalReceiver... translates controller output signal to motor
 *  		input signal
 */
class Drive :
		public DriveBase,
		public DriveStateProvider,
		public DriveControlSignalReceiver
{
public:
    Drive(TaskMgr *scheduler, VictorSP *left, VictorSP *right,
    		Encoder *leftEncoder, Encoder *rightEncoder, Encoder *gyro);

    virtual ~Drive() {}

    /**
     * Zero encoders and gyroscope.
     */
    void Zero();

    /*
     * Sets drive to use the cheesy drive controller if it doesn't already.
     * Also sets the input for the cheesy drive controller.
     *
     * @param throttle forward-backwards-ness to drive cheesy with
     * @param wheel turn value to drive cheesy with
     */
    void CheesyDrive(double throttle, double wheel);

    /*
     * Sets the state of quickturn in the cheesy controller to the value given
     *
     * @param quickturn whether quickturn is active
     */
    void SetCheesyQuickTurn(bool quickturn);

    /*
     * Sets drive to use standard arcade drive controller if it doesn't already
     * Also sets the input for the arcade drive controller.
     *
     * @param throttle forward-backwards-ness to drive with
     * @param wheel turn value to drive with
     */
    void ArcadeDrive(double throttle, double turn);

    void PIDDrive(double dist);

    void PIDTurn(double angle);

    /**
     * All distances given in inches
     * All velocities given in inches/second
     */
	double GetLeftDist() override;
	double GetRightDist() override;
	double GetLeftRate() override;
	double GetRightRate() override;
	double GetDist() override;
	double GetRate() override;

	/**
	 * All angles given in degrees
	 * All angular rates given in degrees/second
	 */
	double GetAngle() override;
	double GetAngularRate() override;

	/*
	 * Used by the DriveController to set motor values
	 *
	 * @param left power (from -1.0 to 1.0) for left motor
	 * @param right power (from -1.0 to 1.0) for right motor
	 */
	void SetDriveOutput(double left, double right) override;
private:
	Encoder *m_leftEncoder;
	Encoder *m_rightEncoder;
	Encoder *m_gyro;

	double m_leftPower;
	double m_rightPower;

	VictorSP *m_leftMotor;
	VictorSP *m_rightMotor;

    ArcadeDriveController *m_arcadeDriveController;
    CheesyDriveController *m_cheesyDriveController;
    PIDDriveController *m_pidDriveController;
};

#endif
