/*
 * Compressor.cpp
 *
 *  Created on: Nov 3, 2015
 *      Author: Andrew
 */

/*
 * Compressor.h
 *
 *  Created on: Nov 3, 2015
 *      Author: Andrew
 */

#include "GreyCompressor.h"

GreyCompressor::GreyCompressor(DigitalInput *pressureSwitch, Relay *compressor,
		TaskMgr *scheduler) :
				m_enabled(true),
				m_airPressureSwitch(pressureSwitch),
				m_compressor(compressor),
				m_scheduler(scheduler)
{
	m_airPressureSwitch = pressureSwitch;
	m_compressor = compressor;
	m_scheduler = scheduler;

	this->m_scheduler->RegisterTask("Shooter", this, TASK_PERIODIC);
}

GreyCompressor::~GreyCompressor() {
	this->m_scheduler->UnregisterTask(this);
}

void GreyCompressor::Enable() {
	m_enabled = true;
}

void GreyCompressor::Disable() {
	m_enabled = false;
}

void GreyCompressor::TaskPeriodic(RobotMode mode) {
	if (!m_airPressureSwitch->Get() && m_enabled) {
		m_compressor->Set(Relay::kOn);
	}
	else {
		m_compressor->Set(Relay::kOff);
	}
}
