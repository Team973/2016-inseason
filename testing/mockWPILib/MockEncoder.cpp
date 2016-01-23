/*
 * MockEncoder.cpp
 *
 *  Created on: Jan 20, 2016
 *      Author: andrew
 */

#include <MockEncoder.h>

Encoder::Encoder(uint32_t aChannel, uint32_t bChannel, bool rev,
		EncodingType encoding)
		 : m_distPerPulse(1.0) {
	// TODO Auto-generated constructor stub

}

Encoder::~Encoder() {
	// TODO Auto-generated destructor stub
}


void Encoder::SetDistancePerPulse(double distPerPulse) {
	m_distPerPulse = distPerPulse;
}

int32_t Encoder::GetRaw() {
	return Get();
}

double Encoder::GetDistance() {
	return Get() * m_distPerPulse;
}

double Encoder::GetRate() {
	return m_distPerPulse / GetPeriod();
}

void Encoder::SetSamplesToAverage(int garbage) {
	//Don't actually do anything because this shouldn't matter for testing
}
