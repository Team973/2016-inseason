/*
 * PoseManager.h
 *
 *  Created on: Feb 26, 2016
 *      Author: andrew
 */

#ifndef SRC_POSEMANAGER_H_
#define SRC_POSEMANAGER_H_

#include "lib/json/json.h"

namespace frc973 {

class Arm;
class Shooter;
class Intake;

class PoseManager {
public:
	/**
	 * Initialize the PoseManager and select the first pose in the list
	 * of poses.
	 *
	 * @param arm reference to the arm subsystem
	 * @param shooter reference to the shooter subsystem
	 * @param intake reference to the intake subsystem
	 */
	PoseManager(Arm *arm, Shooter *shooter, Intake *intake);
	virtual ~PoseManager();

	/**
	 * Reload the list of poses from the configuration file.
	 * Forget whatever pose was selected and go to the first one
	 */
	void ReloadConfiguration();

	/**
	 * Move down the list of poses and select the next one.  If we've reached
	 * the end of the list, loop back to the top.
	 */
	void NextPose();

	/**
	 * Assume the currently selected pose.  This actually sends commands to
	 * the subsystems (before, they were just doing whatever they were doing
	 * before)
	 */
	void AssumePose();

	/**
	 * Turn the subsystems off as appropriate.
	 * This probably means turn the shooter flywheel and the arm off, but
	 * leave the shooter at the height it was at before because it's scary
	 * when it drops.
	 */
	void Chill();
private:
	Arm *m_arm;
	Shooter *m_shooter;
	Intake *m_intake;

	Json::Value m_configRoot;
	int m_currPose = 0;
};

} /* namespace frc973 */

#endif /* SRC_POSEMANAGER_H_ */
