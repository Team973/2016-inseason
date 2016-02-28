/*
 * WPILib.cpp
 *
 *  Created on: Jan 21, 2016
 *      Author: andrew
 */


#include "WPILib.h"

uint64_t GetFPGATime() {
	return 0;
}

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
int gethostname(char *name, size_t len) {
	strncpy(name, "Test host", len);
	return 0;
}
#endif
