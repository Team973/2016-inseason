/*
 * MovingAverageFilter.cpp
 *
 *  Created on: Oct 18, 2015
 *      Author: Andrew
 */

#include <lib/MovingAverageFilter.h>

MovingAverageFilter::MovingAverageFilter(double weight, double initial):
	m_weight(weight),
	m_prevValue(initial)
{
}

MovingAverageFilter::~MovingAverageFilter() {
}


double MovingAverageFilter::GetValue(double currentValue) {
	m_prevValue = (m_weight) * m_prevValue + (1.0 - m_weight) * currentValue;
	return m_prevValue;
}

double MovingAverageFilter::GetLatestValue(void) {
	return m_prevValue;
}
