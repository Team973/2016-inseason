/*
 * MockEncoder.h
 *
 *  Created on: Jan 20, 2016
 *      Author: andrew
 */

#ifndef MOCKWPILIB_MOCKENCODER_H_
#define MOCKWPILIB_MOCKENCODER_H_

#include <stdint.h>
#include "MockCounter.h"

class Encoder : public CounterBase {
public:
	Encoder(uint32_t aChannel, uint32_t bChannel, bool reverseDirection=false,
			EncodingType encoding = EncodingType::k1X);
	virtual ~Encoder();

	void SetDistancePerPulse(double distPerPulse);

	int32_t GetRaw();
	double GetDistance();
	double GetRate();
	void SetSamplesToAverage(int throwaway);

private:
	double m_distPerPulse;

};

#endif /* MOCKWPILIB_MOCKENCODER_H_ */
