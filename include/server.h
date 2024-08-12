#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <sys/socket.h>

int start(char* host, int PORT);

void run(char* path);

void stop();


#endif