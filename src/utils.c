#include "utils.c"

char* getVersion() {
	char version[12];

	snprintf(
		version, sizeof(version), "%d.%d.%d",
		MAJOR_VERSION, MINOR_VERSION, PATCHLEVEL
	);

	return version;
}


char* getCurrentTime() {
	time_t now = time(NULL);
	struct tm *t = localtime(now);
	char buffer[12];

	strftime(buffer, sizeof(buffer), "%a %b %e %H:%M:%S", t);

	return buffer;
}