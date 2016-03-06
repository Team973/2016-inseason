#include "lib/CoopMTRobot.h"
#include "lib/JoystickHelper.h"
#include "RobotInfo.h"

class VictorSP;
class Accelerometer;
//class ADXRS450_Gyro;
class Encoder;
class DigitalInput;
class PowerDistributionPanel;

namespace frc973 {

class LogSpreadsheet;
class SingleThreadTaskMgr;
class Drive;
class Intake;
class Shooter;
class Arm;
class GreyCompressor;
class LogCell;
class SPIGyro;
class PoseManager;

class Robot:
		public CoopMTRobot,
		public JoystickObserver
{
private:
    static void* runServer(void*);

	SingleThreadTaskMgr *m_hiFreq;
	LogSpreadsheet *m_logger;

	PowerDistributionPanel *m_pdp;

	/**
	 * Inputs (joysticks, sensors, etc...)
	 */
	ObservableJoystick		*m_driverJoystick;
	ObservableJoystick		*m_operatorJoystick;

	Accelerometer 	*m_accel;

#ifdef PROTO_BOT_PINOUT
	Encoder *m_collinGyro;
#else
	SPIGyro *m_austinGyro;
#endif
	//ADXRS450_Gyro *m_spiGyro;

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
	Arm				*m_arm;

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

	PoseManager *m_poseManager;

public:
	/**
	 * Defined in Robot.cpp
	 */
	Robot(void);
	~Robot(void);
	void Initialize(void) override;

	/**
	 * Defined in Disabled.h
	 */
	void DisabledStart(void) override;
	void DisabledStop(void) override;
	void DisabledContinuous(void) override;

	/**
	 * Defined in Autonomous.h
	 */
	void AutonomousStart(void) override;
	void AutonomousStop(void) override;
	void AutonomousContinuous(void) override;

	void TurnTest(void);
	void Flappers(void);

	/**
	 * Defined in Teleop.h
	 */
	void TeleopStart(void) override;
	void TeleopStop(void) override;
	void TeleopContinuous(void) override;

	void ObserveJoystickStateChange(uint32_t port, uint32_t button,
			bool newState) override;

	/**
	 * Defined in Test.h
	 */
	void TestStart(void) override;
	void TestStop(void) override;
	void TestContinuous(void) override;

	/**
	 * Defined in Robot.cpp
	 */
	void AllStateContinuous(void) override;

	void PrintState();
};

}
