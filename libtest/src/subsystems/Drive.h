#ifndef DRIVE_H
#define DRIVE_H

#include "lib/DriveBase.h"

class ArcadeDriveController;
class CheesyDriveController;
class PIDDriveController;

/*
 * Drive contains within logic to talk to some drive controllers, as well as
 * logic to create some drive controllers, and some logic that makes it easy
 * to control those controllers.  Meta.
 */
class Drive :
		public DriveBase,
		public AngleProvider,
		public DistProvider,
		public DriveOutput
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

	void UpdateDriveOutput() override;
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
