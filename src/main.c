#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "utils.h"
#include "server.h"


#define PORT 5050

int main(int argc, char const* argv[])
{

	// read and parse the commande line arguments

	// host and port will be changes to dinamic
	char *host = "127.0.0.1";
	int port = 5050;


	start(host, port);
	printf("[%s] Server started on port %d", getCurrentTime(), port);

	return 0;
}
