#ifndef REQUEST_H
#define REQUEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
	char* method; /* Request method */

	RequestURI uri; /* Request endpoint uri */

	char* httpVersion; /* Request type HTTP/1.1 or ... */

	/* Other request parameters */
} HttpRequest;


int parseRequestURI(RequestURI* uri, char* s);

int parseRequestLine(HttpRequest* request, char* line);

void parseRequestHead(HttpRequest* request, char* head);

void parseRequestBody(HttpRequest* request, char* body);

#endif
