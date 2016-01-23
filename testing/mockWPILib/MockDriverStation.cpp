/*
 * MockDriverStation.cpp
 *
 *  Created on: Jan 20, 2016
 *      Author: andrew
 */

#include <MockDriverStation.h>

DriverStation::DriverStation()
		 : m_batteryVoltage(12.0){
	;
}

DriverStation::~DriverStation() {

}

void DriverStation::SetBatteryVoltage(float voltage) {
	m_batteryVoltage = voltage;
}

float DriverStation::GetBatteryVoltage() {
	return m_batteryVoltage;
}


DriverStation &DriverStation::GetInstance() {
	/*
	if (m_instance == nullptr) {
		m_instance = new DriverStation();
	}
	return *m_instance;
	*/
	return *(new DriverStation());
}
