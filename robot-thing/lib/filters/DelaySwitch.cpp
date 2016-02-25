/*
 * DelaySwitch.cpp
 *
 *  Created on: Nov 3, 2015
 *      Author: Andrew
 */

#include "lib/filters/DelaySwitch.h"

DelaySwitch::DelaySwitch() :
	m_prevValue(false),
	m_filter(0.9)
{
	;
}

DelaySwitch::~DelaySwitch() {
	;
}

/*
 * Create a moving average filter of all previous binary inputs... true maps
 * to 1.0 and false maps to 0.0.  A high moving average implies we've been
 * seeing a lot of trues lately, and a low moving average implies we've been
 * seeing a lot of falses lately.  If our previous output and our average
 * are in significant disagreement, change output to match moving average.
 */
bool DelaySwitch::GetValue(bool currentValue) {
	double val = m_filter.Update(currentValue? 1.0 : 0.0);

	if (m_prevValue && val < 0.25) {
		m_prevValue = false;
		return false;
	}
	else if (!m_prevValue && val > 0.75) {
		m_prevValue = true;
		return true;
	}
	else {
		return m_prevValue;
	}
}

bool DelaySwitch::GetLatestValue(void) {
	return m_prevValue;
}
