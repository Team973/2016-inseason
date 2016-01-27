/*
 * MockDriverStation.cpp
 *
 *  Created on: Jan 20, 2016
 *      Author: andrew
 */

#include <MockDriverStation.h>
#include <math.h>
#include <stdio.h>

DriverStation *DriverStation::s_instance = nullptr;

DriverStation::DriverStation()
		 : m_batteryVoltage(12.0){
	s_instance = this;

	for (int i = 0; i < NUM_CHANS; i++) {
		m_joyStates[i].buttons = 0;
		for (int j = 0; j < NUM_AXIS; j++) {
			m_joyStates[i].axises[j] = 0.0;
		}
	}
}

DriverStation::~DriverStation() {

}

void DriverStation::SetBatteryVoltage_T(float voltage) {
	m_batteryVoltage = voltage;
}

float DriverStation::GetBatteryVoltage() {
	return m_batteryVoltage;
}


DriverStation &DriverStation::GetInstance() {
	printf("WARNING: DriverStation::GetInstance() was called in test mode.  "\
			"Returning the most recently instantiated DriverStation object.\n");
	return *s_instance;
}

uint32_t DriverStation::GetStickButtons(int chan) {
	if (chan >= 0 && chan < NUM_CHANS) {
		return m_joyStates[chan].buttons;
	}
	else {
		printf("Tried to access a non-extant joystick chan=%d\n", chan);
		return 0;
	}
}

void DriverStation::SetStickButtons_T(int chan, uint32_t btns) {
	if (chan >= 0 && chan < NUM_CHANS) {
		m_joyStates[chan].buttons = btns;
	}
	else {
		printf("Tried to axis a non-extant joystick Chan=%d\n",
				chan);
	}
}

float DriverStation::GetStickAxis(int chan, int axis) {
	if (chan >= 0 && chan < NUM_CHANS && axis >= 0 && axis < NUM_AXIS) {
		return m_joyStates[chan].axises[axis];
	}
	printf("Tried to access a non-extant joystick Chan=%d axis=%d\n",
			chan, axis);
	return NAN;
}

void DriverStation::SetStickAxis_T(int chan, int axis, float val) {
	if (chan >= 0 && chan < NUM_CHANS && axis >= 0 && axis < NUM_AXIS) {
		m_joyStates[chan].axises[axis] = val;
	}
	else {
		printf("Tried to access a non-extant joystick Chan=%d axis=%d\n",
				chan, axis);
	}
}
