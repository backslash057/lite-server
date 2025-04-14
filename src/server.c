#include "server.h"
#include "request.h"

#include "utils.h"

#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#define BUF_SIZE 4096

int serverFd;


int startServer(char* host, int port) {
	struct sockaddr_in servAddr;
	socklen_t addrlen = sizeof(servAddr);

	serverFd = socket(AF_INET, SOCK_STREAM, 0);

	int opt = 1;
	setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(port);
	inet_pton(AF_INET, host, &servAddr.sin_addr);

	if(bind(serverFd, (struct sockaddr*)&servAddr, addrlen) < 0) {
		logMessage(LOG_ERROR, "Server", "Failed to bind to socket (reason: %s)", strerror(errno));
		return -1;
	}
	
	if(listen(serverFd, 10)) {
		logMessage(LOG_ERROR, "Server", "Failed to listen on %s:%d (reason: %s)", host, port, strerror(errno));
		return -1;
	}

	logMessage(LOG_INFO, "Server", "Lite server %s (\e]8;;https://%s:%d/\e\\http://%s:%d\e]8;;\e\\) started",
		getVersion(), host, port, host, port);
}	

void runServer(char* path) {
	struct sockaddr_in clientAddr;
	socklen_t addrlen = sizeof(clientAddr);

	int clientFd;
	char clientAddress_s[INET_ADDRSTRLEN];
	int clientPort;

	int readSize;
	char buffer[BUF_SIZE];
	char *line;

	while(1) {
		clientFd = accept(serverFd, (struct sockaddr*)&clientAddr, &addrlen);
		
		if(clientFd == -1) {
			logMessage(LOG_ERROR, "Server", "Connection refused (reason: %s)", strerror(errno));
			continue;
		}

		inet_ntop(AF_INET, &clientAddr.sin_addr, clientAddress_s, INET_ADDRSTRLEN);
		clientPort = ntohs(clientAddr.sin_port);

		logMessage(LOG_INFO, "Server", "%s:%d Accepted", clientAddress_s, clientPort);

		bzero(buffer, BUF_SIZE);
		readSize = read(clientFd, buffer, BUF_SIZE-1);
		

		if(readSize < 0) {
			logMessage(LOG_ERROR, "Server", "Failed to read request (reason: %s)", strerror(errno));
		}
		else {
			// TODO: move this to a function and send with in a pthread_create
			HttpRequest request;
			HttpResponse response;

			// TODO: Error handling
			int ret = parseRequest(&request, buffer);
			if(ret < 0) request.badRequestFlag = 1;

			handleRequest(request, &response);

			sendResponse(clientFd, response);

			logMessage(LOG_INFO, "Server", "%s:%d [%d] %s %s",
				clientAddress_s, clientPort,response.code,
				request.method, request.uri.path
			);
		}

		logMessage(LOG_INFO, "Server", "%s:%d Closing", clientAddress_s, clientPort);
		close(clientFd);
	}
}

void stopServer() {
	close(serverFd);

	if(fcntl(serverFd, F_GETFD) != -1 || errno != EBADF) {
		logMessage(LOG_ERROR, "Server", "Failed to stop server\n");
	}
	else logMessage(LOG_INFO, "Server", "Server stopped");
}

void userCancel(int sig) {
	logMessage(LOG_INFO, "Server", "Server stopped by user");
	stopServer();
	exit(0);
}