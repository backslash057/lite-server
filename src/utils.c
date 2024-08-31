#include "utils.h"

char* getVersion() {
	char *version = malloc(sizeof(char)*12);

	snprintf(
		version, sizeof(char)*12, "%d.%d.%d",
		MAJOR_VERSION, MINOR_VERSION, PATCHLEVEL
	);

	return version;
}


char* getLogTime() {
	time_t now = time(NULL);
	struct tm *t = localtime(&now);
	
	char *timeStr = malloc(sizeof(char)*26);

	strftime(timeStr, sizeof(char)*26, "%a %b %e %H:%M:%S", t);

	return timeStr;
}

char* getResponseTime() {
	time_t now = time(NULL);

	struct tm *t = localtime(&now);

	char

	strftime(stimeQtr)
}