/*
 * Arm.h
 *
 *  Created on: Jan 29, 2016
 *      Author: Andrew
 */

#ifndef SRC_SUBSYSTEMS_ARM_H_
#define SRC_SUBSYSTEMS_ARM_H_

#include "lib/CoopTask.h"
#include <stdio.h>

class VictorSP;
class Encoder;
class DigitalInput;
class PowerDistributionPanel;

namespace frc973 {

class PID;

class Arm:
	public CoopTask{
public:
	Arm(TaskMgr *scheduler, PowerDistributionPanel* pdp);
	virtual ~Arm();

	/**
	 * Set the target speed of the arm... good for teleop when you want the
	 * driver to be able to control the position using the joystick.
	 *
	 * @param speed specifies the speed the arm should go at (from -1.0 to 1.0
	 * 			units meaningless)
	 */
	void SetTargetSpeed(double speed);

	/**
	 * Set the target position for the arm... good for auto when you want to
	 * go to a preprogrammed position.
	 *
	 * @param position specifies the position goal for the arm in degrees
	 */
	void SetTargetPosition(double position);

	/**
	 * Set the power sent to the arm... good for teleop open loop control
	 *
	 * @param power to send to the arm
	 */
	void SetPower(double power);

	/**
	 * Some preset positions (in degrees) for other subsystems (or teleop)
	 * to reference if they want to send the arm somewhere.
	 */
	static constexpr double ARM_POS_SHOOT = 40.0;
	static constexpr double ARM_POS_CHIVAL_UP = 18.0;
	static constexpr double ARM_POS_CHIVAL_DOWN = 8.0;
	static constexpr double ARM_POS_PORTCULLIS = 0.0;

	/**
	 * Get the current angle (in degrees) of the arm
	 *
	 * @return returns the angle (in degrees) the arm is currently at
	 */
	double GetArmAngle();

	/**
	 * Get the current angular velocity (in degrees/sec) of the arm
	 *
	 * @return returns the angular velocity (in deg/sec) of the arm
	 */
	double GetArmVelocity();

	/**
	 * Ask the power distribution panel for the current going through the arm
	 */
	double GetArmCurrent();

	void TaskPeriodic(RobotMode mode) override;

	/**
	 * Go into zeroing mode...
	 * run constant low power until certain current is drawn
	 */
	void StartZero();
	void EndZero();

	void PrintStats() {
		printf("Arm position: %lf\n", GetArmAngle());
	}
private:

	enum ArmMode {
		position_control,
		velocity_control,
		openLoop_control,
		zeroing
	};

	TaskMgr *m_scheduler;
	PowerDistributionPanel *m_pdp;

	uint32_t m_lastTimeSec;

	VictorSP *m_armMotor;
	Encoder *m_armEncoder;

	ArmMode m_mode;
	double m_targetSpeed;
	double m_targetPos;
	double m_armPow;
	PID *m_pid;

	double m_zeroOffsetDeg;
	bool m_encoderZerod;
public:
	/**
	 * maximum speed that the arm should go when the user presses full
	 * forward or full back on joystick.  Units degrees/sec.
	 * (only used in velocity_control mode... position mode is as fast as possible)
	 */
	static constexpr double MAX_ARM_SPEED = 40.0;

	/**
	 * The angle that the arm starts at in degrees (because it doesn't
	 * start at zero degrees).
	 *
	 * To zero, we send |ZEROING_POWER| until current > |STALL_CURRENT|
	 * then assume that's good enough
	 */
	static constexpr double ARM_OFFSET = 70.0;
	static constexpr double ZEROING_POWER = -0.2;
	static constexpr double STALL_CURRENT = 3.0;

	/**
	 * Software stop the arm if it tries to go further back than this
	 */
	static constexpr double ARM_SOFT_MIN_POS = 0.0;

	/**
	 * Software stop the arm if it tries to go further forward than this
	 */
	static constexpr double ARM_SOFT_MAX_POS = 90.0;

	static constexpr double ARM_PID_KP = 0.04;
	static constexpr double ARM_PID_KI = 0.0000;
	static constexpr double ARM_PID_KD = 0.00;
};

}

#endif /* SRC_SUBSYSTEMS_ARM_H_ */
