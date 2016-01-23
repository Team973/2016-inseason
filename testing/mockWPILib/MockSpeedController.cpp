/*
 * MockSpeedController.cpp
 *
 *  Created on: Jan 20, 2016
 *      Author: andrew
 */

#include <MockSpeedController.h>

SpeedController::SpeedController()
		 : m_power(0.0)
		 , m_inverted(false)
		 , m_disabled(false) {
	;
}

SpeedController::~SpeedController() {
	;
}


void SpeedController::Set(float speed, uint8_t syncGroup) {
	m_power = speed;
}

float SpeedController::Get() {
	return m_power;
}

void SpeedController::SetInverted(bool inverted) {
	m_inverted = inverted;
}

void SpeedController::Disable() {
	m_disabled = true;
}

bool SpeedController::GetInverted() {
	return m_inverted;
}
