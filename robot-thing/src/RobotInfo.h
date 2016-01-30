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
 * PWM pin-out
 */
constexpr int DRIVE_LEFT_PWM = 0;
constexpr int DRIVE_RIGHT_PWM = 1;

constexpr int ARM_MOTOR_PWM = 2;

constexpr int FLYWHEEL_TWO_PWM = 5;
constexpr int FLYWHEEL_ONE_PWM = 6;

constexpr int INTAKE_ROLLER_LEFT_PWM = 7;

constexpr int INTAKE_TRIGGER_PWM = 9;

/**
 * DIN pin-out
 */
constexpr int AIR_PRESSURE_DIN = 0;

constexpr int FLYWHEEL_BANNERSENSOR_DIN = 1;

constexpr int GYRO_ENCODER_A_DIN = 2;
constexpr int GYRO_ENCODER_B_DIN = 3;

constexpr int LEFT_DRIVE_ENCODER_A_DIN = 4;
constexpr int LEFT_DRIVE_ENCODER_B_DIN = 5;

constexpr int ARM_ENCODER_A_DIN = 6;
constexpr int ARM_ENCODER_B_DIN = 7;

constexpr int ARM_ZERO_STOP_SWITCH_DIN = 8;

/**
 * Relay pin-out
 */
constexpr int COMPRESSOR_RELAY = 0;
constexpr int READY_LIGHT_RELAY = 1;

/**
 * Solenoid channels
 */
constexpr int INTAKE_EXTEND_SOL_CHANNEL = 6;

/**
 * USB port-out (driver-station)
 */
constexpr int DRIVER_JOYSTICK_PORT = 0;
constexpr int OPERATOR_JOYSTICK_PORT = 1;

/**
 * Distance (in inches) of the drive per click of the encoder
 */
constexpr double DRIVE_WHEEL_DIAMETER = 4.0;
constexpr double DRIVE_CLICKS_PER_REVOLUTION = 360.0;
constexpr double DRIVE_GEAR_RATIO = 1.0;
constexpr double DRIVE_DIST_PER_REVOLUTION = DRIVE_WHEEL_DIAMETER *
		Constants::PI;
constexpr double DRIVE_DIST_PER_CLICK = DRIVE_DIST_PER_REVOLUTION *
		DRIVE_CLICKS_PER_REVOLUTION;

/**
 * Distance (in degrees) of the arm per click of the encoder (after
 * compensating for sampling)
 */
constexpr double ARM_DIST_PER_CLICK = 1.0;

#endif /* SRC_ROBOTINFO_H_ */
