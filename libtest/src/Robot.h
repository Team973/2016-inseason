#include "lib/CoopMTRobot.h"

class VictorSP;
class Accelerometer;
class Encoder;
class DigitalInput;
class LogSpreadsheet;
class SingleThreadTaskMgr;
class Drive;
class Intake;
class Shooter;
class GreyCompressor;
class LogCell;

class Robot : public CoopMTRobot
{
private:
	LogSpreadsheet *m_logger;

	/**
	 * Inputs (joysticks, sensors, etc...)
	 */
	Joystick		*m_driverJoystick;
	Joystick		*m_operatorJoystick;

	Accelerometer 	*m_accel;

	/**
	 * Outputs (motors, solenoids, etc...)
	 */
	VictorSP		*m_leftDriveVictor;
	VictorSP		*m_rightDriveVictor;
	Encoder			*m_leftDriveEncoder;
	Encoder			*m_gyroEncoder;
	Drive			*m_drive;

	/**
	 * Subsystems
	 */
	Intake			*m_intake;
	Shooter			*m_shooter;

	/*
	 * Compressor
	 */
	DigitalInput	*m_airPressureSwitch;
	Relay			*m_compressorRelay;
	GreyCompressor	*m_compressor;

	/**
	 * Auto
	 */
	int m_autoState;
	uint32_t m_autoTimer;
	uint32_t m_selectedAutoRoutine;

	/**
	 * Logging
	 */
	LogCell *m_battery;
	LogCell *m_time;
	LogCell *m_state;
	LogCell *m_accelCellX;
	LogCell *m_accelCellY;
	LogCell *m_accelCellZ;
	LogCell *m_messages;

public:
	/**
	 * Defined in Robot.cpp
	 */
	Robot(void);
	~Robot(void);
	void Initialize(void);

	/**
	 * Defined in Disabled.h
	 */
	void DisabledStart(void);
	void DisabledStop(void);
	void DisabledContinuous(void);

	/**
	 * Defined in Autonomous.h
	 */
	void AutonomousStart(void);
	void AutonomousStop(void);
	void AutonomousContinuous(void);

	void TwoBallAuto(void);
	void OneBallAuto(void);

	/**
	 * Defined in Teleop.h
	 */
	void TeleopStart(void);
	void TeleopStop(void);
	void TeleopContinuous(void);

	/**
	 * Defined in Test.h
	 */
	void TestStart(void);
	void TestStop(void);
	void TestContinuous(void);

	/**
	 * Defined in Robot.cpp
	 */
	void AllStateContinuous(void);
};
