#include "utils.h"

// initialize the logs here (format and file_path if available)


char* getVersion() {
	static char versionBuffer[12];
	
	snprintf(
		versionBuffer, sizeof(char)*12, "%d.%d.%d",
		MAJOR_VERSION, MINOR_VERSION, PATCHLEVEL
	);
	
	return versionBuffer;
}


char* getCurrentTime() {
	static char timeStringBuffer[26];
	time_t now = time(NULL);
	struct tm *t = localtime(&now);

	strftime(timeStringBuffer, sizeof(char)*26, "%a %b %e %H:%M:%S", t);

	return timeStringBuffer;
}

void logMessage(enum logType logtype, const char* component, const char* format, ...) {
	char *logTypeMessages[4]  = {
		"INFO", "DEBUG", "WARNING", "ERROR"
	};

	
	printf("[%s] [%s] ", getCurrentTime(), logTypeMessages[logtype]);
	
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);

	printf("\n");
}