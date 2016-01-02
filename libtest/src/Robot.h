#include <subsystems/Drive.h>
#include "WPILib.h"

#include "../lib/CoopMTRobot.h"
#include "../lib/GreyCompressor.h"
#include "../lib/LogSpreadsheet.h"

#include "subsystems/Intake.h"
#include "subsystems/Shooter.h"

class Robot : public CoopMTRobot
{
private:
	Joystick		*m_driverJoystick;
	Joystick		*m_operatorJoystick;

	Accelerometer 	*m_accel;

	VictorSP		*m_leftDriveVictor;
	VictorSP		*m_rightDriveVictor;
	Encoder			*m_leftDriveEncoder;
	Encoder			*m_gyroEncoder;
	Drive			*m_drive;

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
	LogSpreadsheet *m_logger;
	LogCell *m_battery;
	LogCell *m_time;
	LogCell *m_state;
	LogCell *m_accelCellX;
	LogCell *m_accelCellY;
	LogCell *m_accelCellZ;
	LogCell *m_messages;

public:
	Robot(void);
	~Robot(void);
	void Initialize(void);

	void DisabledStart(void);
	void DisabledStop(void);
	void DisabledContinuous(void);

	void AutonomousStart(void);
	void AutonomousStop(void);
	void AutonomousContinuous(void);

	void TwoBallAuto(void);
	void OneBallAuto(void);

	void TeleopStart(void);
	void TeleopStop(void);
	void TeleopContinuous(void);

	void TestStart(void);
	void TestStop(void);
	void TestContinuous(void);

	void AllStateContinuous(void);
};
