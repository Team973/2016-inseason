#include <cstdarg>
#include "WrapDash.h"
#include "wpilib.h"

#define DBSTRING_MAX_LENGTH		21

const char *positionStrings[] = {
		"DB/String 0",
		"DB/String 1",
		"DB/String 2",
		"DB/String 3",
		"DB/String 4",
		"DB/String 5",
		"DB/String 6",
		"DB/String 7",
		"DB/String 8",
		"DB/String 9",
};

void DBStringPrintf(DBStringPos position, const char *formatstring, ...) {
	char msg[DBSTRING_MAX_LENGTH + 1];
	va_list args;

	va_start(args, formatstring);
	vsnprintf(msg, DBSTRING_MAX_LENGTH - 1, formatstring, args);
	va_end(args);

	SmartDashboard::PutString(positionStrings[position], msg);
}
