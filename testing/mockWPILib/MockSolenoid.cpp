/*
 * MockSolenoid.cpp
 *
 *  Created on: Jan 20, 2016
 *      Author: andrew
 */

#include <MockSolenoid.h>

Solenoid::Solenoid(int chan)
		 : m_pos(false) {

}

Solenoid::~Solenoid() {
	// TODO Auto-generated destructor stub
}

void Solenoid::Set(bool pos) {
	m_pos = pos;
}

bool Solenoid::Get() {
	return m_pos;
}
