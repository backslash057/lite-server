#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <time.h>

// #include <errno.h>


#define PORT 5050

int main(int argc, char const* argv[])
{

	int serverFd, clientFd;
	struct sockaddr_in servAddr;
	struct sockaddr_in clientAddr;
	socklen_t addrlen = sizeof(clientAddr);

	char clientAddress_s[INET_ADDRSTRLEN];
	int clientPort;
	time_t t;

	int readSize;
	char buffer[1024] = {0};

	// Creating the server socket
	serverFd = socket(AF_INET, SOCK_STREAM, 0);


	// binding address to server
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(PORT);
	inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr);

	if(bind(serverFd, (struct sockaddr*)&servAddr, addrlen) < 0) {
		perror("bind: ");
		exit(1);
	}

	
	// listening to connections
	if(listen(serverFd, 10)) {
		perror("listen: ");
		exit(1);
	}

	printf("Server listening on port %d\n", PORT);
	
	while(1) {
		clientFd = accept(serverFd, (struct sockaddr*)&clientAddr, &addrlen);
		if(clientFd == -1) perror("accept: ");

		inet_ntop(AF_INET, &clientAddr.sin_addr, clientAddress_s, INET_ADDRSTRLEN);
		clientPort = ntohs(clientAddr.sin_port);

		time(&t);
		printf("[%s] %s:%d Accepted\n", ctime(&t), clientAddress_s, clientPort);


		readSize = read(clientFd, buffer, 1024);

		// handle received datas
		// parseRequestLine(request, reqSize);

		printf("[%s] %s:%d %s\n", ctime(&t), clientAddress_s, clientPort, buffer);
		printf("[%s] %s:%d Closing\n", ctime(&t), clientAddress_s, clientPort);

		close(clientFd);
	}
	
	close(serverFd);

	return 0;
}
