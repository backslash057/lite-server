#include "handler.h"


void handleRequest(HttpRequest request, HttpResponse* response) {
	/*

	recuperer le path
	verifier si c'est un fichier qui est demandé
	si oui
		lire le fichier et l'ecrire dans le socket
	si non
		lire le template et ecrire


	*/

	response->httpVersion = request.httpVersion;
	response->code = 200;
} 

char* stringify(HttpResponse response) {
	char *buffer = malloc(sizeof(char)*1024);


	char* fmt =
	"\
	%s %d %s\r\n \
	Server: Live\n\
	Content-Type: text/plain\
	";

	sprintf(buffer, fmt, response.httpVersion, response.code, "OK");

	return buffer;
}

void sendRequest(HttpResponse response) {



}