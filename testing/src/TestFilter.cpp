/*
 * TestMedianFilter.cpp
 *
 *  Created on: Jan 21, 2016
 *      Author: andrew
 */

#include "TestFilter.h"

#include "lib/util/Util.h"
#include "lib/filters/MedianFilter.h"
#include "lib/filters/MovingAverageFilter.h"

#define ASSERT_CLOSE(expected,actual)\
	ASSERTM(#expected " expected but was " #actual, Util::close(expected,actual))

void TestFilters::TestMedianFilter() {
	MedianFilter f = MedianFilter();

	ASSERTM("Start value should be 0.0",
			Util::close(f.GetLast(), 0.0));
	ASSERT_CLOSE(f.Update(1.0), 0.0);
	ASSERT_CLOSE(f.Update(2.0), 0.0);
	ASSERT_CLOSE(f.Update(3.0), 1.0);
	ASSERT_CLOSE(f.Update(4.0), 2.0);
	ASSERT_CLOSE(f.GetLast(), 2.0);
	ASSERT_CLOSE(f.Update(4.0), 3.0);
	ASSERT_CLOSE(f.Update(999.0), 4.0);
	ASSERT_CLOSE(f.Update(4.0), 4.0);
	ASSERT_CLOSE(f.Update(0.0), 4.0);
}

void TestFilters::TestMedianOf3Filter() {
	MedianFilter f = MedianFilter(3);

	ASSERTM("Start value should be 0.0",
			Util::close(f.GetLast(), 0.0));
	ASSERT_CLOSE(f.Update(1.0), 0.0);
	ASSERT_CLOSE(f.Update(2.0), 1.0);
	ASSERT_CLOSE(f.Update(3.0), 2.0);
	ASSERT_CLOSE(f.Update(4.0), 3.0);
	ASSERT_CLOSE(f.GetLast(), 3.0);
	ASSERT_CLOSE(f.Update(4.0), 4.0);
	ASSERT_CLOSE(f.Update(999.0), 4.0);
	ASSERT_CLOSE(f.Update(4.0), 4.0);
	ASSERT_CLOSE(f.Update(0.0), 4.0);
}

void TestFilters::TestMovingAverageFilter() {
	MovingAverageFilter f = MovingAverageFilter(0.8);

	ASSERT_CLOSE(f.GetLatestValue(), 0.0);
	ASSERT_CLOSE(f.GetValue(1.0), 0.2);
	ASSERT_CLOSE(f.GetLatestValue(), 0.2);
	ASSERT_CLOSE(f.GetValue(1.0), 0.36);
	ASSERT_CLOSE(f.GetValue(1.0), 0.488);
	ASSERT_CLOSE(f.GetValue(1.0), 0.5904);
}

cute::suite TestFilters::MakeSuite(){
	cute::suite s;

	s.push_back(CUTE_SMEMFUN(TestFilters, TestMedianFilter));
	s.push_back(CUTE_SMEMFUN(TestFilters, TestMedianOf3Filter));
	s.push_back(CUTE_SMEMFUN(TestFilters, TestMovingAverageFilter));

	return s;
}

