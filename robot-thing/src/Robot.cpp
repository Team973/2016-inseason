#include "RobotInfo.h"
#include "Robot.h"

#include "lib/GreyCompressor.h"
#include "lib/logging/LogSpreadsheet.h"
#include "lib/SingleThreadTaskMgr.h"
#include "lib/SPIGyro.h"
#include "lib/WrapDash.h"

#include "subsystems/Intake.h"
#include "subsystems/Shooter.h"
#include "subsystems/Drive.h"
#include "subsystems/Arm.h"

constexpr int ONE_BALL_AUTO = 0;
constexpr int TWO_BALL_AUTO = 1;

Robot::Robot(void
	) :
	m_hiFreq(nullptr),
	m_logger(nullptr),
	m_driverJoystick(nullptr),
	m_operatorJoystick(nullptr),
	m_accel(nullptr),
	m_austinGyro(nullptr),
	m_leftDriveVictor(nullptr),
	m_rightDriveVictor(nullptr),
	m_leftDriveEncoder(nullptr),
	m_gyroEncoder(nullptr),
	m_drive(nullptr),
	m_intake(nullptr),
	m_shooter(nullptr),
	m_arm(nullptr),
	m_airPressureSwitch(nullptr),
	m_compressorRelay(nullptr),
	m_compressor(nullptr),
	m_autoState(0),
	m_autoTimer(0),
	m_selectedAutoRoutine(TWO_BALL_AUTO),
	m_battery(nullptr),
	m_time(nullptr),
	m_state(nullptr),
	m_accelCellX(nullptr),
	m_accelCellY(nullptr),
	m_accelCellZ(nullptr)
{
	m_hiFreq = new SingleThreadTaskMgr(*this, 1.0 / 200.0);

	m_driverJoystick = new ObservableJoystick(DRIVER_JOYSTICK_PORT, this, this);
	m_operatorJoystick = new ObservableJoystick(OPERATOR_JOYSTICK_PORT, this, this);

	m_accel = new BuiltInAccelerometer(Accelerometer::kRange_4G);

	m_austinGyro = new SPIGyro();

	m_leftDriveVictor = new VictorSP(DRIVE_LEFT_PWM);
	m_rightDriveVictor = new VictorSP(DRIVE_RIGHT_PWM);

	m_leftDriveEncoder = new Encoder(LEFT_DRIVE_ENCODER_A_DIN,
			LEFT_DRIVE_ENCODER_B_DIN, false, CounterBase::k4X);
	m_gyroEncoder = new Encoder(GYRO_ENCODER_A_DIN, GYRO_ENCODER_B_DIN,
			false, CounterBase::k2X);

	m_drive = new Drive(this, m_leftDriveVictor, m_rightDriveVictor,
			m_leftDriveEncoder, nullptr, m_gyroEncoder);

	m_intake = new Intake(this);

	m_arm = new Arm(this);

	m_airPressureSwitch = new DigitalInput(AIR_PRESSURE_DIN);
	m_compressorRelay = new Relay(COMPRESSOR_RELAY, Relay::kForwardOnly);
	m_compressor = new GreyCompressor(m_airPressureSwitch, m_compressorRelay, this);

	m_logger = new LogSpreadsheet(this);
	m_battery = new LogCell("Battery voltage");

	m_time = new LogCell("Time (ms)");
	m_state = new LogCell("Game State");
	m_accelCellX = new LogCell("X-acceleration");
	m_accelCellY = new LogCell("Y-acceleration");
	m_accelCellZ = new LogCell("Z-acceleration");
	m_messages = new LogCell("Robot messages", 100, true);

	m_logger->RegisterCell(m_battery);
	m_logger->RegisterCell(m_time);
	m_logger->RegisterCell(m_state);
	m_logger->RegisterCell(m_accelCellX);
	m_logger->RegisterCell(m_accelCellY);
	m_logger->RegisterCell(m_accelCellZ);
	m_logger->RegisterCell(m_messages);

	m_shooter = new Shooter(this, m_logger);
}

Robot::~Robot(void) {
}

void Robot::Initialize(void) {
	printf("initializing the roboto\n");

	//SmartDashboard::PutString("DB/String 0", "Two ball auto");

	m_logger->InitializeTable();

	SmartDashboard::init();
	//m_hiFreq->Start();
	//m_hiFreq->SetHighPriority();
}

void Robot::AllStateContinuous(void) {
	DBStringPrintf(DBStringPos::DB_LINE8, "gyro %lf", m_austinGyro->GetDegrees());

	m_battery->LogPrintf("%f", DriverStation::GetInstance().GetBatteryVoltage());

	m_time->LogPrintf("%u", GetMsecTime());
	m_state->LogPrintf("%s", GetRobotModeString());

	m_accelCellX->LogDouble(m_accel->GetX());
	m_accelCellY->LogDouble(m_accel->GetY());
	m_accelCellZ->LogDouble(m_accel->GetZ());

	/*
	SmartDashboard::PutNumber("X-Accel", m_accel->GetX());
	SmartDashboard::PutNumber("Y-Accel", m_accel->GetY());
	SmartDashboard::PutNumber("Z-Accel", m_accel->GetZ());
	*/
}

#include "Disabled.h"
#include "Autonomous.h"
#include "Teleop.h"
#include "Test.h"

START_ROBOT_CLASS(Robot);
