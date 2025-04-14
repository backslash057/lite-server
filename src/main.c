#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // for getcwd
#include <string.h>
#include <signal.h>

#define PATH_MAX 4960

#include "utils.h"
#include "config.h"
#include "server.h"


int main(int argc, char const* argv[])
{
	// Default configuration values
	char host[20]; 
	strcpy(host, "127.0.0.1");

	int port = 5050;

	char documentRoot[PATH_MAX];
    if (getcwd(documentRoot, sizeof(documentRoot)) == NULL) {
        logMessage(LOG_ERROR, "Config", "getcwd() failed, unable to load from current directory");
    }


	Config config = NULL;
	// TODO: define a fix system level config file path
	parseConfig(&config, "config.ini");

	if(keyExists(config, "server", "host")) strcpy(host, getString(config, "server", "host"));
	if(keyExists(config, "server", "port")) port = getInt(config, "server", "port");
	if(keyExists(config, "target", "document_root")) strcpy(documentRoot, getString(config, "target", "document_root"));

	// TODO: send the log file path to utils.c here(if specified), and the log format
	
	signal(SIGINT, userCancel);

	int ret = startServer(host, port);
	if(ret < 0) return EXIT_FAILURE;


	runServer(documentRoot);

	stopServer();

	return EXIT_SUCCESS;
}
