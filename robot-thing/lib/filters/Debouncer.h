/*
 * Debouncer.h
 *
 *  Created on: Jan 17, 2016
 *      Author: Andrew
 */

#ifndef LIB_FILTERS_DEBOUNCER_H_
#define LIB_FILTERS_DEBOUNCER_H_

#include "lib/util/Util.h"

class Debouncer {
	double m_timeStart;
	double m_per;
	bool first;

public:
	Debouncer(double period) {
		m_timeStart = 0.0;
		m_per = period;
		first = false;
	}

	virtual ~Debouncer() {
		;
	}

	bool Update(bool val) {
		if (first) {
			first = false;
			m_timeStart = GetSecTime();
		}
		if (!val) {
			m_timeStart = GetSecTime();
		}
		return GetSecTime() > m_per;
	}
};

#endif /* LIB_FILTERS_DEBOUNCER_H_ */
