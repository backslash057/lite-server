#ifndef HANDLER_H
#define HANDLER_H

#include "parser.h"

typedef struct {
	int code;
	char* httpVersion;

	char *contentType;

	int contentSize;
	char* body;
} HttpResponse;


void handleRequest(HttpRequest request, HttpResponse* response);

char* stringify(HttpResponse response);

#endif