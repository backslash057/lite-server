#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "utils.h"
#include "server.h"

#define BASE_PATH "/home/backslash057/live"

#define PORT 5050

int main(int argc, char const* argv[])
{
	// read and parse the command line arguments

	/*
	host, path and port will be changed to 
	dynamic by command line arguments later
	*/
	char *host = "127.0.0.1";
	int port = 5050;
	char* path = BASE_PATH;

	start(host, port);

	run(path);

	stop();

	return 0;
}
