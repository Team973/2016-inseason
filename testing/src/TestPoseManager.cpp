/*
 * TestPoseManager.cpp
 *
 *  Created on: Feb 27, 2016
 *      Author: andrew
 */

#include "TestPoseManager.h"
#include "MockTaskMgr.h"
#include "subsystems/Arm.h"
#include "subsystems/Shooter.h"
#include "subsystems/Intake.h"
#include "PoseManager.h"

using namespace frc973;

void TestPoseManager::TestFirstPose() {
	AllPublicTaskMgr tasker = AllPublicTaskMgr();
	Arm arm = Arm(&tasker);
	Shooter shooter = Shooter(&tasker, nullptr);
	Intake intake = Intake(&tasker);
	printf("creating\n");
	PoseManager pm = PoseManager(&arm, &shooter, &intake);

	pm.AssumePose();

	ASSERT(shooter.GetFrontFlywheelState() == Shooter::FlywheelState::ssControl);
	ASSERT(shooter.GetBackFlywheelState() == Shooter::FlywheelState::openLoop);
	ASSERT(shooter.GetElevatorState() == Shooter::ElevatorHeight::midHigh);
}

void TestPoseManager::TestSecondPose() {
	AllPublicTaskMgr tasker = AllPublicTaskMgr();
	Arm arm = Arm(&tasker);
	Shooter shooter = Shooter(&tasker, nullptr);
	Intake intake = Intake(&tasker);
	printf("creating\n");
	PoseManager pm = PoseManager(&arm, &shooter, &intake);

	pm.NextPose();
	pm.AssumePose();

	ASSERT(shooter.GetFrontFlywheelState() == Shooter::FlywheelState::openLoop);
	ASSERT(shooter.GetBackFlywheelState() == Shooter::FlywheelState::openLoop);
	ASSERT(shooter.GetElevatorState() == Shooter::ElevatorHeight::wayHigh);
}

void TestPoseManager::TestWrapPose() {
	AllPublicTaskMgr tasker = AllPublicTaskMgr();
	Arm arm = Arm(&tasker);
	Shooter shooter = Shooter(&tasker, nullptr);
	Intake intake = Intake(&tasker);
	printf("creating\n");
	PoseManager pm = PoseManager(&arm, &shooter, &intake);

	pm.NextPose();
	pm.NextPose();
	pm.AssumePose();

	ASSERT(shooter.GetFrontFlywheelState() == Shooter::FlywheelState::ssControl);
	ASSERT(shooter.GetBackFlywheelState() == Shooter::FlywheelState::openLoop);
	ASSERT(shooter.GetElevatorState() == Shooter::ElevatorHeight::midHigh);
}

void TestPoseManager::TestRePose() {
	AllPublicTaskMgr tasker = AllPublicTaskMgr();
	Arm arm = Arm(&tasker);
	Shooter shooter = Shooter(&tasker, nullptr);
	Intake intake = Intake(&tasker);
	printf("creating\n");
	PoseManager pm = PoseManager(&arm, &shooter, &intake);

	pm.AssumePose();
	pm.NextPose();
	pm.AssumePose();

	ASSERT(shooter.GetFrontFlywheelState() == Shooter::FlywheelState::openLoop);
	ASSERT(shooter.GetBackFlywheelState() == Shooter::FlywheelState::openLoop);
	ASSERT(shooter.GetElevatorState() == Shooter::ElevatorHeight::wayHigh);
}


cute::suite TestPoseManager::MakeSuite(){
	cute::suite s;
	s.push_back(CUTE_SMEMFUN(TestPoseManager, TestFirstPose));
	s.push_back(CUTE_SMEMFUN(TestPoseManager, TestSecondPose));
	s.push_back(CUTE_SMEMFUN(TestPoseManager, TestWrapPose));
	s.push_back(CUTE_SMEMFUN(TestPoseManager, TestRePose));
	return s;
}
