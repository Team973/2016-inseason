/*
 * RobotInfo.h
 *
 *  Created on: Oct 9, 2015
 *      Author: Andrew
 */

#ifndef ROBOTINFO_H_
#define ROBOTINFO_H_

#include "lib/util/Util.h"

/**
 * Drive subsystem... all units in inches
 */
#define		DRIVE_LEFT_PWM					0
#define		DRIVE_RIGHT_PWM					1

#define		GYRO_ENCODER_A_DIN				2
#define		GYRO_ENCODER_B_DIN				3

#define		LEFT_DRIVE_ENCODER_A_DIN		4
#define		LEFT_DRIVE_ENCODER_B_DIN		5

constexpr double DRIVE_WHEEL_DIAMETER = 4.0;
constexpr double DRIVE_TICKS_PER_REVOLUTION = 360.0;
constexpr double DRIVE_GEAR_RATIO = 1.0;
constexpr double DRIVE_DIST_PER_REVOLUTION = DRIVE_WHEEL_DIAMETER *
		Constants::PI;
constexpr double DRIVE_DIST_PER_CLICK = DRIVE_DIST_PER_REVOLUTION *
		DRIVE_TICKS_PER_REVOLUTION;

/**
 * Intake subsystem
 */
#define		INTAKE_EXTEND_SOL_CHANNEL		6
#define		INTAKE_ROLLER_LEFT_PWM			2

#define		INTAKE_FORWARD_SPEED			1.0
#define		INTAKE_REVERSE_SPEED			-1.0

/**
 * Shooter subsystem
 */
#define		FLYWHEEL_ONE_PWM				6
#define		FLYWHEEL_TWO_PWM				5
#define		CHEATER_PWM						7
#define		FLYWHEEL_BANNERSENSOR_DIN		1

#define		READY_LIGHT_RELAY				1

#define		FLYWHEEL_RUN_SPEED				1.0

#define		FEED_FORWARD_SPEED				-0.3
#define		FEED_REVERSE_SPEED				0.3

/**
 * Compressor
 */
#define		AIR_PRESSURE_DIN				0
#define		COMPRESSOR_RELAY				0

#endif /* SRC_ROBOTINFO_H_ */
