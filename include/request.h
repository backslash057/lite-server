#ifndef REQUEST_H
#define REQUEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

typedef struct {
	int code;
	char* httpVersion;

	char *contentType;

	int contentSize;
	char* body;
} HttpResponse;

/* Request Methods*/
typedef enum {
	GET,
	POST,
	PUT,
	UPDATE,
	DELETE,
	PATCH,
	CONNECT,
	CREATE
} HttpMethods;

typedef struct {
	char *path; /* /path/to/ressource */

	/* Request arguments */
	char **argKeys;
	char **argValues;
	int argCount;
} RequestURI;

typedef struct {
	int badRequestFlag; // Already tell handleRequest() it is 400-Bad request

	char* method; /* Request method */

	RequestURI uri; /* Request endpoint uri */

	char* httpVersion; /* Request type HTTP/1.1 or ... */

	/* Other request parameters */
} HttpRequest;

// Request handling

//int parseRequestURI(RequestURI* uri, char* s); // private function

int parseRequest(HttpRequest* request, char* buffer);

void handleRequest(HttpRequest request, HttpResponse* response);

int parseRequestURI(RequestURI* uri, char* uriString);

int parseRequestLine(HttpRequest* request, char* line);

// Response handling
char* getReasonPhrase(int responseCode);

void sendResponse(int clientFd, HttpResponse response);

#endif
