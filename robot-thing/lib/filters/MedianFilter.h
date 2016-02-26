/*
 * MedianFilter.h
 *
 *  Created on: Jan 17, 2016
 *      Author: Andrew
 */

#ifndef LIB_FILTERS_MEDIANFILTER_H_
#define LIB_FILTERS_MEDIANFILTER_H_

namespace frc973 {

class MedianFilter {
public:
	MedianFilter(int buffSize = 5);
	virtual ~MedianFilter();

	double Update(double in);
	double GetLast();
private:
	int m_buffSize;
	double *m_samples;
	int m_idx;
	double m_last;
};

}

#endif /* LIB_FILTERS_MEDIANFILTER_H_ */
