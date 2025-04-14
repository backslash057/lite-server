#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#include "request.h"


int startServer(char* host, int PORT);

void runServer(char* path);

void stopServer();

void userCancel(int sig);

#endif