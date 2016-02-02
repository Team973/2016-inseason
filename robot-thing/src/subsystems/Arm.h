/*
 * Arm.h
 *
 *  Created on: Jan 29, 2016
 *      Author: Andrew
 */

#ifndef SRC_SUBSYSTEMS_ARM_H_
#define SRC_SUBSYSTEMS_ARM_H_

#include "lib/CoopTask.h"

class PID;
class VictorSP;
class Encoder;
class DigitalInput;

class Arm:
	public CoopTask{
public:
	Arm(TaskMgr *scheduler);
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
	 * Some preset positions (in degrees) for other subsystems (or teleop)
	 * to reference if they want to send the arm somewhere.
	 */
	static constexpr double ARM_POS_UP = 0.0;
	static constexpr double ARM_POS_DOWN = 90.0;

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

	void TaskPostPeriodic(RobotMode mode) override;
private:
	/**
	 * Whenever the zero-stop gets clicked, this should get called to zero
	 * the encoder and adjust any offset state
	 */
	void Zero();

	enum ArmMode {
		position_control,
		velocity_control
	};

	TaskMgr *m_scheduler;

	uint32_t m_lastTimeSec;

	VictorSP *m_armMotor;
	Encoder *m_armEncoder;
	DigitalInput *m_armZeroSwitch;

	ArmMode m_mode;
	double m_targetSpeed;
	double m_targetPos;
	PID *m_pid;

	double m_zeroOffsetDeg;
public:
	/**
	 * maximum speed that the arm should go when the user presses full
	 * forward or full back on joystick.  Units degrees/sec.
	 */
	static constexpr double MAX_ARM_SPEED = 90.0;

	/**
	 * The angle that the arm starts at in degrees (because it doesn't
	 * start at zero degrees).
	 */
	static constexpr double ARM_OFFSET = 17.0;

	/**
	 * Software stop the arm if it tries to go further back than this
	 */
	static constexpr double ARM_SOFT_MIN_POS = -45.0;

	/**
	 * Software stop the arm if it tries to go further forward than this
	 */
	static constexpr double ARM_SOFT_MAX_POS = 110.0;

	static constexpr double ARM_PID_KP = 0.1;
	static constexpr double ARM_PID_KI = 0.0001;
	static constexpr double ARM_PID_KD = 0.02;
};

#endif /* SRC_SUBSYSTEMS_ARM_H_ */