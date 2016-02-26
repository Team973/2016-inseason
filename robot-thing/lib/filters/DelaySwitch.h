/*
 * DelaySwitch.h
 *
 *  Created on: Nov 3, 2015
 *      Author: Andrew
 *
 * DelaySwitch filters a binary filter that may be noisy and creates a binary
 * signal that is definitely not noisy but may be a little delayed.
 *
 * Does this by creating a MovingAverageFilter of all previous binary inputs
 * and doesn't change output until the moving average of input is way above or
 * way below 0.5.
 */

#ifndef LIB_DELAYSWITCH_H_
#define LIB_DELAYSWITCH_H_

#include "MovingAverageFilter.h"

namespace frc973 {

class DelaySwitch {
public:
	DelaySwitch();
	virtual ~DelaySwitch();

	/**
	 * Calculate the filtered value given the original datapoint.
	 *
	 * @parm currentValue  the current data point that needs to be filtered
	 *
	 * @return result of filtering calculation.
	 */
	bool GetValue(bool currentValue);

	/**
	 * Remember the latest value calculated by filtering
	 *
	 * @return result of previous filtering calcuation
	 */
	bool GetLatestValue(void);

private:
	bool m_prevValue;
	MovingAverageFilter m_filter;
};

}

#endif /* LIB_DELAYSWITCH_H_ */
