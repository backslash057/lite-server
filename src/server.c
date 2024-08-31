#include "server.h"
#include "parser.h"
#include "handler.h"

#include "utils.h"

#include <unistd.h>
#include <string.h>

#include <dirent.h>

int serverFd;

char* getReasonPhrase(int responseCode) {
	switch(responseCode) {
		case 200: return "OK";
		case 404: return "Not Found";
	}
}

void sendResponse(int clientFd, HttpResponse response){
	char *fmt = "%s %d %s\r\nHello world";

	dprintf(clientFd, fmt, 
		response.httpVersion, response.code,
		getReasonPhrase(response.code)
	);
}

int start(char* host, int port) {
	struct sockaddr_in servAddr;
	socklen_t addrlen = sizeof(servAddr);

	// Creating the server socket
	serverFd = socket(AF_INET, SOCK_STREAM, 0);

	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(port);
	inet_pton(AF_INET, host, &servAddr.sin_addr);

	if(bind(serverFd, (struct sockaddr*)&servAddr, addrlen) < 0) {
		perror("bind");
		exit(1);
	}
	
	if(listen(serverFd, 10)) {
		perror("listen");
		exit(1);
	}

	printf("[%s] Live server %s (\e]8;;https://%s:%d/\e\\http://%s:%d\e]8;;\e\\) started\n",
		getCurrentTime(), getVersion(), host, port, host, port);
}	

void run(char* path) {
	struct sockaddr_in clientAddr;
	socklen_t addrlen = sizeof(clientAddr);

	int clientFd;
	char clientAddress_s[INET_ADDRSTRLEN];
	int clientPort;

	int readSize;
	char buffer[1024];
	char *line;

	while(1) {
		clientFd = accept(serverFd, (struct sockaddr*)&clientAddr, &addrlen);
		
		if(clientFd == -1) {
			perror("accept");
			continue;
		}

		inet_ntop(AF_INET, &clientAddr.sin_addr, clientAddress_s, INET_ADDRSTRLEN);
		clientPort = ntohs(clientAddr.sin_port);

		printf("[%s] %s:%d Accepted\n", getCurrentTime(), clientAddress_s, clientPort);

		readSize = read(clientFd, buffer, 1024-1);
		buffer[readSize] = '\0';
		if(readSize < 0) perror("read");
		else {
			HttpRequest request;
			HttpResponse response;

			line = strtok(buffer, "\r\n");
			if(line == NULL) printf("bad request ignored for the moment.\n");

			parseRequestLine(&request, line);

			handleRequest(request, &response);

			sendResponse(clientFd, response);

			printf("[%s] %s:%d ", getCurrentTime(), clientAddress_s, clientPort);
			printf("%s %s\n", request.method, request.uri.path);
		}

		printf("[%s] %s:%d Closing\n", getCurrentTime(), clientAddress_s, clientPort);
		close(clientFd);
	}
}

void stop() {
	close(serverFd);
	printf("[%s] Server stopped", getCurrentTime());
}
