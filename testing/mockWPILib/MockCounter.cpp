/*
 * MockCounter.cpp
 *
 *  Created on: Jan 20, 2016
 *      Author: andrew
 */

#include <MockCounter.h>

CounterBase::CounterBase()
		 : m_count(0)
		 , m_period(0.0)
		 , m_direction(true)
		 , m_maxPeriod(DEFAULT_MAX_PERIOD) {
}

CounterBase::~CounterBase() {
}

int32_t CounterBase::Get() {
	return m_count;
}

void CounterBase::Reset() {
	m_count = 0;
	m_period = 0.0;
	m_direction = true;
}
double CounterBase::GetPeriod() {
	return m_period;
}

void CounterBase::SetMaxPeriod(double maxPeriod) {
	m_maxPeriod = maxPeriod;
}

bool CounterBase::GetStopped() {
	return m_period < m_maxPeriod;
}

bool CounterBase::GetDirection() {
	return m_direction;
}

void CounterBase::SetTestData(uint32_t count, double period,
		bool direction) {
	m_count = count;
	m_period = period;
	m_direction = direction;
}
