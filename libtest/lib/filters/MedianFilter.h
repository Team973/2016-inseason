/*
 * MedianFilter.h
 *
 *  Created on: Jan 17, 2016
 *      Author: Andrew
 */

#ifndef LIB_FILTERS_MEDIANFILTER_H_
#define LIB_FILTERS_MEDIANFILTER_H_

class Median5Filter {
public:
	double samples[5];
	int idx;

	Median5Filter() : idx(0) {
		for (int i = 0; i < 5; i++) {
			samples[i] = 0.0;
		}
	}

	virtual ~Median5Filter() {
		;
	}

	double Update(double in) {
		samples[idx] = in;
		idx = (idx + 1) % 5;

		double sorted[5];

		for (int i = 0; i < 5; i++) {
			sorted[i] = samples[i];
		}

		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 4; j++) {
				if (sorted[j] > sorted[j + 1]) {
					double tmp = sorted[j];
					sorted[j] = sorted[j+1];
					sorted[j+1] = tmp;
				}
			}
		}

		return sorted[2];
	}
};

#endif /* LIB_FILTERS_MEDIANFILTER_H_ */
