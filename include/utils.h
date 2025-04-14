#ifndef UTILS_H
#define UTILS_H

#include <time.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#define MAJOR_VERSION 1
#define MINOR_VERSION 0
#define PATCHLEVEL 0

enum logType {
    LOG_INFO,
    LOG_DEBUG,
    LOG_WARNING,
    LOG_ERROR
};

char* getVersion();

char* getCurrentTime();

void logMessage(enum logType logtype, const char* component, const char* format, ...);


#endif