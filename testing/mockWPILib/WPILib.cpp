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

int gethostname(char *name, size_t len) {
	strncpy(name, "Test host", len);
	return 0;
}
