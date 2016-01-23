/*
 * MockCounter.h
 *
 *  Created on: Jan 20, 2016
 *      Author: andrew
 */

#ifndef MOCKWPILIB_MOCKCOUNTER_H_
#define MOCKWPILIB_MOCKCOUNTER_H_

#include <stdint.h>

class CounterBase {
public:
	CounterBase();
	virtual ~CounterBase();

	virtual int32_t Get();
	virtual void Reset();
	virtual double GetPeriod();
	virtual void SetMaxPeriod(double maxPeriod);
	virtual bool GetStopped();
	virtual bool GetDirection();

	/**
	 * Set the count and the period of the latest sample.
	 */
	virtual void SetTestData(uint32_t count, double period = 0.0,
			bool direction = true);

	enum EncodingType {k1X, k2X, k4X};

	static constexpr double DEFAULT_MAX_PERIOD = 0.05;
private:
	uint32_t m_count;
	double m_period;
	bool m_direction;

	double m_maxPeriod;
};

class Counter : public CounterBase {
public:
	Counter(int din) {}
	virtual ~Counter() {}
};

#endif /* MOCKWPILIB_MOCKCOUNTER_H_ */
