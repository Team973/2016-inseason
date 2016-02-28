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

}

void TestPoseManager::TestWrapPose() {

}

void TestPoseManager::TestRePose() {

}


cute::suite TestPoseManager::MakeSuite(){
	cute::suite s;
	s.push_back(CUTE_SMEMFUN(TestPoseManager, TestFirstPose));
	return s;
}
