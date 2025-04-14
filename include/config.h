#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#define MAX_LINE_LENGTH 1024

#include "utils.h"

typedef enum {
    INTEGER, STRING, DOUBLE
} ValueType;

typedef union value {
    char* s_value;
    int i_value;
    double d_value;
} Value;

typedef struct data {
    char* key;
    ValueType type;
    Value val;
    struct data *next;
} Data;

typedef struct section {
    char* title;
    Data* head;
    struct section *next;
} Section;

typedef Section* Config;

int parseConfig(Config* conf, char* path);

int keyExists(Config config, char* section, char* key);

char* getString(Config config, char* section, char* key);

int getInt(Config config, char* section, char* key);

double getDouble(Config config, char* section, char* key);

void printConfig(Config config);

void freeConfig(Config config);

#endif